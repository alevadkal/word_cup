// Copyright © Alexander Kaluzhnyy

#include "read_word.h"
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <errno.h>
#include <wctype.h>
#include <stdlib.h>
#include <locale.h>
#include "log.h"

static void fill_symbols(wint_t symbol, size_t symbol_index, char* data)
{
    mbstate_t ps;
    memset(&ps, 0, sizeof(ps));
    char sequaence[MB_CUR_MAX + 1];
    size_t symbol_size = wcrtomb(sequaence, (wchar_t)symbol, &ps);
    sequaence[symbol_size] = '\0';
    strcpy(&data[symbol_index], sequaence);
}

static char* read_word_internal(FILE* file, size_t symbol_index)
{
    static mbstate_t ps;
    wint_t symbol = fgetwc(file);
    if (symbol == WEOF) {
        if (ferror(file)) {
            LOG("fgetwc error:%s(%i)", strerror(errno), errno);
        }
    }
    if (iswalpha(symbol)) {
        symbol = towupper(symbol);
        char* data = read_word_internal(file, symbol_index + wcrtomb(NULL, (wchar_t)symbol, &ps));
        if (data == NULL) {
            LOG("read_word_internal error:%s(%i)", strerror(errno), errno);
            return NULL;
        }
        fill_symbols(symbol, symbol_index, data);
        return data;
    } else {
        char* data = (char*)calloc(symbol_index + 1, sizeof(char));
        if (data == NULL) {
            LOG("fgetc error:%s(%i)", strerror(errno), errno);
            return NULL;
        }
        data[symbol_index] = 0;
        return data;
    }
}

static void skipp_not_alpha(FILE* file)
{
    for (
        wint_t symbol = fgetwc(file);
        (!iswalpha(symbol)) || feof(file) || ferror(file);
        symbol = fgetwc(file)) { }
}

char* fread_word(FILE* file)
{
    if (file == NULL) {
        LOG("error: file is null");
        return NULL;
    }
    size_t symbol_index = 0;
    skipp_not_alpha(file);
    char* word = read_word_internal(file, symbol_index);
    if (word == NULL && ferror(file)) {
        LOG("read_word_internal error:%s(%i)", strerror(errno), errno);
        return NULL;
    }
    return word;
}
