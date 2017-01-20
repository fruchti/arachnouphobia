#ifndef MAIN_H_
#define MAIN_H_

#include <stdlib.h>
#include <stdbool.h>
#include "stm32f030x6.h"
#include "pinning.h"
#include "buildid.h"
#include "ws2812b_spi.h"

int main(void);
void ADC_Init(void);

void TIM14_IRQHandler(void);
void ADC1_IRQHandler(void);

#endif
