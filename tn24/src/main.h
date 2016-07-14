#ifndef MAIN_H_
#define MAIN_H_

#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <stdint.h>
#include <util/delay.h>

#include "pinning.h"

int main(void);

uint16_t rand();

#endif
