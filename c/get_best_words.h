#include "word_holder.h"

typedef struct best_holder_t best_holder_t;

best_holder_t* get_best_words(word_holder_t* holder, size_t number);

size_t best_holder_size(best_holder_t* self);
word_stat_t* best_holder_get_word(best_holder_t* self, size_t id);
void best_holder_deinit(best_holder_t* self);
