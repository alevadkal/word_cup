// Copyright © Alexander Kaluzhnyy

#include "word_holder.h"
#include "log.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>

struct word_stat_t {
    char* word;
    size_t index;
};

struct word_holder_t {
    size_t size;
    word_stat_t* words;
};

word_holder_t* word_holder_init(void)
{
    word_holder_t* self = (word_holder_t*)calloc(1, sizeof(word_holder_t));
    if (self == NULL) {
        LOG("calloc error:%s(%i)", strerror(errno), errno)
        return NULL;
    }
    return self;
}

void word_holder_deinit(word_holder_t* self)
{
    if (self == NULL) {
        return;
    }
    for (size_t i = 0; i < self->size; i++) {
        free(self->words[i].word);
        self->words[i].word = NULL;
    }
    free(self->words);
    self->words = NULL;
    free(self);
}

static size_t word_holder_count_or_get_id_to_insert(word_holder_t* self, char* word)
{
    size_t idx = self->size / 2;
    for (size_t step_value = self->size / 2 + self->size % 2;
         step_value != 0;
         step_value = step_value / 2 + step_value % 2) {
        int result = strcmp(word, self->words[idx].word);
        if (result == 0) {
            self->words[idx].index++;
            return (size_t)-1;
        } else if (result > 0) {
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

int word_holder_count_word(word_holder_t* self, char* word)
{
    if (self == NULL) {
        LOG("error: word holder is null")
        return -1;
    }
    if (word == NULL) {
        LOG("error: word is null")
        return -1;
    }
    if (!word[0]) {
        return 0;
    }
    size_t idx = word_holder_count_or_get_id_to_insert(self, word);
    if (idx == (size_t)-1) {
        return 0;
    }
    word_stat_t* new_words = realloc(self->words, sizeof(word_stat_t) * self->size + 1);
    if (new_words == NULL) {
        LOG("error: realloc:%s(%i)", strerror(errno), errno)
        return -1;
    }
    self->words = new_words;
    char* new_word = strdup(word);
    if (new_word == NULL) {
        LOG("error: strdup:%s(%i)", strerror(errno), errno);
        return -1;
    }
    memcpy(&self->words[idx], &self->words[idx + 1], (self->size - idx) * sizeof(word_stat_t));
    self->words[idx].word = new_word;
    self->words[idx].index = 1;
    self->size++;
    return 0;
}

word_stat_t* word_holder_get_word(word_holder_t* self, size_t id)
{
    if (self == NULL) {
        LOG("error: word holder is null")
        return NULL;
    }
    if (id >= self->size) {
        LOG("error: id out of range")
        return NULL;
    }
    return &self->words[id];
}
size_t word_holder_get_size(word_holder_t* self)
{
    if (self == NULL) {
        return 0;
    }
    return self->size;
}

const char* word_stat_word(word_stat_t* self)
{
    if (self == NULL) {
        LOG("error: word holder is null")
        return NULL;
    }
    return self->word;
}
size_t word_stat_count(word_stat_t* self)
{
    if (self == NULL) {
        LOG("error: word holder is null")
        return 0;
    }
    return self->index;
}
