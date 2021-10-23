// Copyright © Alexander Kaluzhnyy

#include <stdio.h>

#define LOG(format, ...) fprintf(stderr, "%s:%i:" format "\n", __FILE__, __LINE__, ##__VA_ARGS__);
