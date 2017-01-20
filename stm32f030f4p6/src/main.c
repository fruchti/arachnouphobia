#include "main.h"

#define SCARCITY 900

volatile uint16_t ADC_Reading;

int main(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

    GPIOA->PUPDR |= (1 << (PIN_DIP_1 << 1)) | (1 << (PIN_DIP_2 << 1)) | (1 << (PIN_DIP_3 << 1)) | (1 << (PIN_DIP_4 << 1));

    LED_Init();
    ADC_Init();

    for(int i = 0; i < LED_COUNT; i++)
    {
    }

    TIM14->PSC = 10000;
    TIM14->ARR = 100;
    TIM14->CNT = 0;
    TIM14->DIER = TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM14_IRQn);
    TIM14->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN;

    for(;;);

    return 0;
}

void ADC_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;

    ADC1->CFGR1 = ADC_CFGR1_CONT | ADC_CFGR1_OVRMOD;
    ADC1->SMPR = 1;
    ADC1->CHSELR = ADC_CHSELR_CHSEL0;

    // // ADC Calibration
    // ADC1->CR |= ADC_CR_ADDIS;
    // while(ADC1->CR & ADC_CR_ADEN);
    // ADC1->CR |= ADC_CR_ADCAL;
    // while(ADC1->CR & ADC_CR_ADCAL);

    // Enable interrupt
    ADC1->IER = ADC_IER_EOCIE;
    NVIC_EnableIRQ(ADC1_IRQn);

    // Enable the ADC
    ADC1->CR |= ADC_CR_ADEN;
    while(~ADC1->ISR & ADC_ISR_ADRDY);

    // GPIO configuration
    GPIOA->MODER |= (3 << (PIN_SENSOR << 1));
    GPIOA->PUPDR |= (1 << (PIN_SENSOR << 1));

    // And start the first conversion
    ADC1->CR |= ADC_CR_ADSTART;
}

bool IsOn(void)
{
    // Always on?
    if(~GPIOA->IDR & (1 << PIN_DIP_1))
        return true;
    // No -> check if it's dark
    return ADC_Reading > 240;

}

uint32_t GetFactor(void)
{
    uint8_t setting = 0;
    if(~GPIOA->IDR & (1 << PIN_DIP_4))
        setting |= 1;
    if(~GPIOA->IDR & (1 << PIN_DIP_3))
        setting |= 2;
    if(~GPIOA->IDR & (1 << PIN_DIP_2))
        setting |= 4;
    return 1 << setting;
}

void TIM14_IRQHandler(void)
{
    if(TIM14->SR & TIM_SR_UIF)
    {
        uint32_t threshold = RAND_MAX / LED_COUNT / SCARCITY * GetFactor();
        if(!IsOn())
            threshold = 0;

        for(unsigned int i = 0; i < LED_COUNT; i++)
        {
            if(rand() < threshold)
                LED_Buffer[i].R = 255;
            else
                LED_Buffer[i].R = 0;

            if(rand() < threshold)
                LED_Buffer[i].G = 255;
            else
                LED_Buffer[i].G = 0;

            if(rand() < threshold)
                LED_Buffer[i].B = 255;
            else
                LED_Buffer[i].B = 0;
        }

        LED_Refresh();

        TIM14->SR &= ~TIM_SR_UIF;
    }
    NVIC_ClearPendingIRQ(TIM14_IRQn);
}

void ADC1_IRQHandler(void)
{
    ADC_Reading = ADC1->DR;
    ADC1->ISR |= ADC_ISR_EOC;
    NVIC_ClearPendingIRQ(ADC1_IRQn);
}
