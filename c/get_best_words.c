#include "get_best_words.h"
#include <stdlib.h>
#include <string.h>

struct best_holder_t {
    size_t size;
    word_stat_t** words;
};
typedef struct best_holder_internal_t {
    best_holder_t* words;
    size_t worst;
} best_holder_internal_t;

void best_holder_deinit(best_holder_t* self)
{
    if (self == NULL) {
        return;
    }
    free(self->words);
    memset(self, 0, sizeof(*self));
    free(self);
}
static size_t get_add_word_id(best_holder_t* self, word_stat_t* stat)
{
    size_t idx = self->size / 2;
    for (size_t step_value = self->size / 2 + self->size % 2;
         step_value != 0;
         step_value = step_value / 2 + step_value % 2) {
        size_t count_obj = word_stat_count(self->words[idx]);
        size_t count_val = word_stat_count(stat);
        if (count_obj > count_val) {
            idx += step_value;
            if (idx == self->size) {
                break;
            }
        } else {
            if (idx == 0) {
                break;
            }
            idx -= step_value;
        }
    }
    return idx;
}

static void add_best_word(best_holder_internal_t* self, word_stat_t* stat)
{
    size_t current_count = word_stat_count(stat);
    best_holder_t* best_holder = self->words;
    if (current_count <= self->worst) {
        return;
    }
    self->worst = current_count;
    size_t id = get_add_word_id(self->words, stat);
    memmove(&best_holder->words[id], &best_holder->words[id + 1], best_holder->size - id);
    best_holder->words[id] = stat;
}

static int sort_by_count(word_stat_t** left, word_stat_t** right)
{
    size_t left_count = word_stat_count(*left);
    size_t right_count = word_stat_count(*right);
    if (left_count == right_count) {
        return 0;
    } else if (left_count > right_count) {
        return 1;
    } else {
        return -1;
    }
}

static best_holder_t* init_best_words(word_holder_t* holder, size_t number)
{
    best_holder_t* self = calloc(1, sizeof(*self));
    if (self == NULL) {
        return NULL;
    }
    size_t holder_size = word_holder_get_size(holder);
    size_t max_idx = holder_size < number ? holder_size : number;
    self->words = calloc(max_idx, sizeof(self->words[0])); //NOLINT(bugprone-sizeof-expression)
    if (self->words == NULL) {
        free(self);
        return NULL;
    }
    for (size_t i = 0; i < max_idx; i++) {
        self->words[i] = word_holder_get_word(holder, i);
    }
    self->size = max_idx;
    qsort(self->words, self->size, sizeof(self->words[0]), (__compar_fn_t)sort_by_count); //NOLINT(bugprone-sizeof-expression)
    return self;
}
best_holder_t* get_best_words(word_holder_t* holder, size_t number)
{
    if (holder == NULL) {
        return NULL;
    }
    if (number == 0) {
        return NULL;
    }
    best_holder_t* words_top = init_best_words(holder, number);
    if (words_top == NULL) {
        return NULL;
    }
    if (words_top->size != number) {
        return words_top;
    }
    best_holder_internal_t internal
        = {
              0,
          };
    internal.words = words_top;
    for (size_t i = number; i < word_holder_get_size(holder); i++) {
        add_best_word(&internal, word_holder_get_word(holder, i));
    }
    return internal.words;
}

size_t best_holder_size(best_holder_t* self)
{
    if (self == NULL) {
        return 0;
    }
    return self->size;
}
word_stat_t* best_holder_get_word(best_holder_t* self, size_t id)
{
    if (self == NULL) {
        return NULL;
    }
    if (self->size >= id) {
        return NULL;
    }
    return self->words[id];
}
