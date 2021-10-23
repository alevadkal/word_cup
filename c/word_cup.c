// Copyright © Alexander Kaluzhnyy

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <errno.h>
#include <wctype.h>
#include <stdlib.h>
#include <locale.h>
#include "read_word.h"
#include "word_holder.h"
#include "log.h"

int main(int argc, char** argv)
{
    FILE* file = NULL;
    setlocale(LC_ALL, "en_US.utf8");
    if (argc < 2) {
        file = stdin;
        LOG("Use stdin");
    } else {
        file = fopen(argv[1], "r");
        if (file == NULL) {
            LOG("fopen error:%s(%i)", strerror(errno), errno);
            return -1;
        }
    }
    word_holder_t* word_holder = word_holder_init();
    if (word_holder == NULL) {
        LOG("word_holder_init error:%s(%i)", strerror(errno), errno);
        return -1;
    }
    do {
        char* word = fread_word(file);
        if (word == NULL) {
            LOG("read_word error:%s(%i)", strerror(errno), errno);
            break;
        }
        word_holder_count_word(word_holder, word);
        free(word);
    } while (1);

    return 0;
}
