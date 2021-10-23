// Copyright © Alexander Kaluzhnyy

#include <stddef.h>

typedef struct word_stat_t word_stat_t;

const char* word_stat_word(word_stat_t* self);
size_t word_stat_count(word_stat_t* self);

typedef struct word_holder_t word_holder_t;

word_holder_t* word_holder_init(void);
void word_holder_deinit(word_holder_t* self);
size_t word_holder_get_size(word_holder_t* self);
word_stat_t* word_holder_get_word(word_holder_t* self, size_t id);
int word_holder_count_word(word_holder_t* self, char* word);
