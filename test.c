#include <time.h> // time()
#include <stdio.h> // printf()
#include <stdlib.h> // rand()
#include "jim.h"

#define UNUSED(a) ((void)(a))

static uint16_t
random_int(
  jim_sentence_t * const sentence,
  const uint16_t min,
  const uint16_t max
) {
  UNUSED(sentence);
  return min + (rand() % (max - min));
}

static void
on_text(
  const jim_sentence_t * const sentence,
  const char * const s
) {
  UNUSED(sentence);
  printf("%s", s);
}

int main(int argc, char *argv[]) {
  const int num_sentences = (argc > 1) ? atoi(argv[1]) : 1;
  char buf[JIM_BUF_LEN];
  srand(time(NULL));

  for (int i = 0; i < num_sentences; i++) {
    jim_sentence_t s;
    jim_sentence_init(&s, random_int, NULL);
    jim_sentence_emit(&s, buf, on_text);
    printf("\n");
  }

  return 0;
}
