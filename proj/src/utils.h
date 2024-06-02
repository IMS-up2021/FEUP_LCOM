#ifndef UTILS_H
#define UTILS_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "i8042_kbd.h"
#include "kbd.h"

bool is_char(char ch);
char get_key(uint8_t keycode);

#endif
