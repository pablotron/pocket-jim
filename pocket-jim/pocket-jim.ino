#include <avr/wdt.h> // for wdt_enable(), wdt_disable(), and wdt_reset()
#include <ProTrinketKeyboard.h>
#include <cmdline_defs.h>
#include <usbconfig.h>
#include <ProTrinketKeyboardC.h> // for TrinketKeyboard
#include <Adafruit_SSD1306.h> // for Adafruit_SSD1306
#include <jim.h> // for jim_sentence_init(), jim_sentence_emit()

// TODO:
// [x] b2: emulate HID keyboard (see https://learn.adafruit.com/pro-trinket-keyboard/library)
// [x] b3: about screen
// [ ] streaming mode
// [ ] code cleanup
//     [ ] remove debounce
//     [ ] clean up function and state names

#define UNUSED(a) ((void) (a))

// OLED I2C address
#define OLED_I2C_ADDR 0x3C

// delay (ms) for debounce detection
#define DEBOUNCE_DELAY 20

// delay (ms) for sleep
#define SLEEP_DELAY 60000

// button pins
#define B1_PIN 3 // change text
#define B2_PIN 4 // type text via USB
#define B3_PIN 5 // about screen
#define B4_PIN 6 // clear screen

// valid system states
typedef enum {
  STATE_INIT,
  STATE_SHOW,
  STATE_B1_MAYBE_DOWN,
  STATE_B1_DOWN,
  STATE_B1_MAYBE_UP,
  STATE_B2_DOWN,
  STATE_B3_DOWN,
  STATE_B4_DOWN,
  STATE_SLEEP,
  STATE_WAKE,
  STATE_LAST,
} state_t;

// OLED display handle
static Adafruit_SSD1306 display(-1);  // -1 = no reset pin

// current UI state
static state_t state;

// current sentence
static jim_sentence_t sentence;

// delay timer (used for debounce, etc)
static unsigned long timer;

/*
 * Return a random number between @min and @max.
 * (Callback for jim_sentence_init()).
 */
static uint16_t jim_random(
  jim_sentence_t * const sentence,
  const uint16_t min,
  const uint16_t max
) {
  UNUSED(sentence);
  return random(min, max);
}

/*
 * Text fragment handler.
 * (Callback for jim_sentence_emit() to draw text to screen).
 */
static void oled_on_text(
  const jim_sentence_t * const sentence,
  const char * const text
) {
  UNUSED(sentence);

  // write text to serial (debug)
  Serial.print(text);

  // write text to display
  display.print(text);
}

/*
 * Text fragment handler.
 * (callback for jim_sentence_emit() to emit text as fake keyboard via USB).
 */
static void usb_on_text(
  const jim_sentence_t * const sentence,
  const char * const text
) {
  UNUSED(sentence);

  // write text to keyboard
  TrinketKeyboard.print(text);
}

static void
draw_sentence() {
  // clear display
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, 0);

  // emit sentence
  char buf[JIM_BUF_LEN];
  jim_sentence_emit(&sentence, buf, oled_on_text);

  // print trailing newline to serial
  Serial.println();

  // refresh display
  display.display();
}

/*
 * Generate new sentence and draw it to the screen.
 */
static void
change_text() {
  // generate new soentence
  jim_sentence_init(&sentence, jim_random, NULL);

  // draw sentence to screen
  draw_sentence();
}

/*
 * Type current text via USB.
 */
static void
type_text_via_usb() {
  // guard in case usb is not connected
  // FIXME: this does not work if USB is connected
  // and then disconnected
  if (!TrinketKeyboard.isConnected())
    return;

  // clear display
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, 0);

  // write status message to screen
  display.print(F("Sending via USB..."));

  // refresh display
  display.display();

  // generate sentence
  char buf[JIM_BUF_LEN];
  jim_sentence_emit(&sentence, buf, usb_on_text);

  // send trailing period and spaces
  TrinketKeyboard.print(F(".  "));

  // restore sentence to screen
  draw_sentence();
}

/*
 * About screen text.
 */
static const char ABOUT_TEXT[] PROGMEM =
  "Pocket Jim!\n"
  "\n"
  "Corporate BS, on the go.\n"
  "\n"
  "pmdn.org/pocket-jim";

/*
 * Draw about screen.
 */
static void
draw_about_screen() {
  // clear screen
  display.clearDisplay();
  display.setCursor(0, 0);

  // copy about text to buffer
  char buf[64];
  strcpy_P(buf, ABOUT_TEXT);

  // draw about text
  display.print(buf);

  // refresh
  display.display();
}

void setup() {
  // enable the watchdog timer with an 8 second timeout
  // (prevent lockup if the usb keyboard is unplugged)
  wdt_disable();
  wdt_enable(WDTO_8S);

  // init A0 (used as RNG seed)
  pinMode(A0, INPUT);

  // init button pins and enable internal pull-up resistor
  for (int i = 3; i < 7; i++) {
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }

  // init LED_BUILTIN pin
  pinMode(LED_BUILTIN, OUTPUT);

  // init serial (for debugging)
  Serial.begin(9600);

  // get seed, seed RNG
  const long seed = analogRead(0);
  randomSeed(seed);

  // log seed
  Serial.print(F("random seed = "));
  Serial.println(seed);

  // init oled
  display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR);
  display.clearDisplay();
  display.display();

  // init usb keyboard
  TrinketKeyboard.begin();

  // set default state
  state = STATE_INIT;
}

void loop() {
  // poll for usb (must be called every ~10ms)
  TrinketKeyboard.poll();

  switch (state) {
  case STATE_INIT:
    // change text
    change_text();
    
    // update state
    timer = millis() + SLEEP_DELAY;
    state = STATE_SHOW;

    break;
  case STATE_SHOW:
    // disable LED
    digitalWrite(LED_BUILTIN, LOW);
    
    if (digitalRead(B1_PIN) == LOW) {
      // set debounce delay
      timer = millis() + DEBOUNCE_DELAY;
      state = STATE_B1_MAYBE_DOWN;
    } else if (digitalRead(B2_PIN) == LOW) {
      // emit current sentence via usb
      type_text_via_usb();

      // set state
      timer = millis() + DEBOUNCE_DELAY;
      state = STATE_B2_DOWN;
    } else if (digitalRead(B3_PIN) == LOW) {
      // button 3 pressed: show about screen
      draw_about_screen();

      // set state
      timer = millis() + DEBOUNCE_DELAY;
      state = STATE_B3_DOWN;
    } else if (digitalRead(B4_PIN) == LOW) {
      // button 4 pressed: clear screen

      // clear display
      display.clearDisplay();
      display.display();

      // set state
      timer = millis() + DEBOUNCE_DELAY;
      state = STATE_B4_DOWN;
    }

    // check sleep timer
    if (millis() > timer) {
      // clear display
      display.clearDisplay();
      display.display();

      // set sleep state
      state = STATE_SLEEP;
    }

    break;
  case STATE_B1_MAYBE_DOWN:
    if (millis() > timer) {
      // debounce timer expired, check for button press
      if (digitalRead(B1_PIN) == LOW) {
        // legit button press, handle it
        
        // change text
        change_text();

        // set release debounce delay
        timer = millis() + DEBOUNCE_DELAY;

        // set state
        state = STATE_B1_DOWN;
      } else {
        // false alarm, go back to previous state
        state = STATE_SHOW;
      }
    }

    break;
  case STATE_B1_DOWN:
    // button is down, enable LED
    digitalWrite(LED_BUILTIN, HIGH);
    
    if (millis() > timer) {
      // if the timer has expired, then start checking for
      // button release
      if (digitalRead(B1_PIN) == HIGH) {
        // set debounce delay
        timer = millis() + DEBOUNCE_DELAY;
        state = STATE_B1_MAYBE_UP;
      }
    }
   
    break;
  case STATE_B1_MAYBE_UP:
    // if the timer has expired, then check for button release
    if (millis() > timer) {
      // set state
      if (digitalRead(B1_PIN) == HIGH) {
        // button is up, reset sleep delay and go back to STATE_SHOW
        timer = millis() + SLEEP_DELAY;
        state = STATE_SHOW;
      } else {
        // false alarm, button is still down
        state = STATE_B1_DOWN;
      }
    }
    
    break;
  case STATE_B2_DOWN:
    if (millis() > timer) {
      // if the debounce timer has expired, then start checking for
      // button release
      if (digitalRead(B2_PIN) == HIGH) {
        // go back to show state
        timer = millis() + SLEEP_DELAY;
        state = STATE_SHOW;
      }
    }

    break;
  case STATE_B3_DOWN:
    if (millis() > timer) {
      // if the debounce timer has expired, then start checking for
      // button release
      if (digitalRead(B3_PIN) == HIGH) {
        // go back to show state
        timer = millis() + SLEEP_DELAY;
        state = STATE_SHOW;
      }
    }

    break;
  case STATE_B4_DOWN:
    if (millis() > timer) {
      // if the debounce timer has expired, then start checking for
      // button release
      if (digitalRead(B4_PIN) == HIGH) {
        state = STATE_SLEEP;
      }
    }

    break;
  case STATE_SLEEP:
    // check and see if any buttons have been pressed
    if (
      digitalRead(B1_PIN) == LOW ||
      digitalRead(B2_PIN) == LOW ||
      digitalRead(B3_PIN) == LOW ||
      digitalRead(B4_PIN) == LOW
    ) {
      // draw current sentence
      draw_sentence();

      // set debounce delay
      timer = millis() + DEBOUNCE_DELAY;

      // set state
      state = STATE_WAKE;
    }

    break;
  case STATE_WAKE:
    if (millis() > timer) {
      // make sure all all buttons are released
      if (
        digitalRead(B1_PIN) == HIGH &&
        digitalRead(B2_PIN) == HIGH &&
        digitalRead(B3_PIN) == HIGH &&
        digitalRead(B4_PIN) == HIGH
      ) {
        timer = millis() + SLEEP_DELAY;
        state = STATE_SHOW;
      }
    }

    break;
  default:
    // unknown state, do nothing
    Serial.print("unknown state: ");
    Serial.print(state);
    Serial.println();
  }

  // reset watchdog timer
  wdt_reset();

  // delay for 9ms
  // NOTE: this must be <10ms for USB polling
  delay(9);
}
