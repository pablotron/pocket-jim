#include <Adafruit_SSD1306.h>
#include <jim.h>

// TODO: use internal pullup resistors 
// reorganize code

#define UNUSED(a) ((void) (a))

// OLED I2C address
#define OLED_I2C_ADDR 0x3C

// delay (ms) for debounce detection
#define DEBOUNCE_DELAY 20

// button pins
#define B1_PIN 3
#define B2_PIN 4
#define B3_PIN 5
#define B4_PIN 6

typedef enum {
  STATE_INIT,
  STATE_SHOW,
  STATE_MAYBE_DOWN,
  STATE_BUTTON_DOWN,
  STATE_MAYBE_UP,
  STATE_LAST,
} state_t;

// OLED display handle
static Adafruit_SSD1306 display(-1);  // -1 = no reset pin

// UI state
static state_t state;

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
 * (Callback for jim_sentence_emit()).
 */
static void on_text(
  const jim_sentence_t * const sentence,
  const char * const text
) {
  UNUSED(sentence);
  // write text to serial (debug)
  Serial.print(text);

  // write text to display
  display.print(text);
}

void setup() {
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

  // set default state
  state = STATE_INIT;
}

static void
change_text() {
  // clear display
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, 0);

  // init sentence
  jim_sentence_t s;
  jim_sentence_init(&s, jim_random, NULL);

  // generate sentence
   char buf[JIM_BUF_LEN];
  jim_sentence_emit(&s, buf, on_text);

  // print trailing newline to serial
  Serial.println();

  // refresh display
  display.display();
}

void loop() {
  switch (state) {
  case STATE_INIT:
    // change text
    change_text();
    
    // update state
    state = STATE_SHOW;

    break;
  case STATE_SHOW:
    // disable LED
    digitalWrite(LED_BUILTIN, LOW);
    
    if (digitalRead(B1_PIN) == LOW) {
      // set debounce delay
      timer = millis() + DEBOUNCE_DELAY;
      state = STATE_MAYBE_DOWN;
    } else if (digitalRead(B2_PIN) == LOW) {
      display.print(F("B2")); // FIXME: test b2
      display.display();
      state = STATE_MAYBE_DOWN;
    } else if (digitalRead(B3_PIN) == LOW) {
      display.print(F("B3")); // FIXME: test b3
      display.display();
      state = STATE_MAYBE_DOWN;
    } else if (digitalRead(B4_PIN) == LOW) {
      display.print(F("B4")); // FIXME: test b4
      display.display();
      state = STATE_MAYBE_DOWN;
    }

    break;
  case STATE_MAYBE_DOWN:
    if (millis() > timer) {
      // if the timer expired, check for button press
      if (digitalRead(B1_PIN) == LOW) {
        // legit button press, handle it
        
        // change text
        change_text();

        // set release debounce delay
        timer = millis() + DEBOUNCE_DELAY;

        // set state
        state = STATE_BUTTON_DOWN;
      } else {
        // false alarm, go back to previous state
        state = STATE_SHOW;
      }
    }

    break;
  case STATE_BUTTON_DOWN:
    // button is down, enable LED
    digitalWrite(LED_BUILTIN, HIGH);
    
    if (millis() > timer) {
      // if the timer has expired, then start checking for
      // button release
      if (digitalRead(B1_PIN) == HIGH) {
        // set debounce delay
        timer = millis() + DEBOUNCE_DELAY;
        state = STATE_MAYBE_UP;
      }
    }
   
    break;
  case STATE_MAYBE_UP:
    // if the timer has expired, then check for button release
    if (millis() > timer) {
      // set state
      state = (digitalRead(B1_PIN) == HIGH) ? STATE_SHOW : STATE_BUTTON_DOWN;
    }
    
    break;
  default:
    // unknown state, do nothing
    Serial.print("unknown state: ");
    Serial.print(state);
    Serial.println();
  }

  // delay for 10ms
  delay(10);
}
