#ifndef UTILS_H
#define UTILS_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "drivers/i8042_kbd.h"
#include "drivers/kbd.h"

bool is_char (char ch);
void get_keys();

#endif