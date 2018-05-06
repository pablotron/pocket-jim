#ifndef JIM_H
#define JIM_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

// buffer length (must be > max_word_len + 1)
#define JIM_BUF_LEN 32

typedef struct {
  _Bool has_adverb; /* range: [0, 2) */
  uint16_t adverb; /* position in ADVERBS */

  uint16_t verb; /* position in VERBS */

  uint8_t num_adjectives; /* range: [0, 5) */
  uint16_t adjectives[5]; /* positions in ADJECTIVES */

  uint16_t noun; /* position in NOUNS */
} jim_phrase_t;

typedef struct {
  void *user_data;

  uint8_t num_phrases; /* range: [1, 4) */
  jim_phrase_t phrases[3];

  uint16_t joins[2]; /* range: num_phrases - 1 */
} jim_sentence_t;

void
jim_sentence_init(
  jim_sentence_t * const,
  uint16_t (*random_int)(jim_sentence_t * const, uint16_t, uint16_t),
  void *user_data
);

void
jim_sentence_emit(
  const jim_sentence_t * const,
  char * const,
  void (*text_cb)(const jim_sentence_t * const, const char * const)
);

void *
jim_sentence_get_data(
  const jim_sentence_t * const
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* JIM_H */
