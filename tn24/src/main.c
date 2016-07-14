#include "main.h"

uint16_t RCounter;
uint16_t GCounter;
uint16_t BCounter;

int main(void)
{
    DDRA = 0;
    DDRB = 0;
    PORTA = 0;
    PORTB = 0;

    D_LEDR |= (1 << P_LEDR);
    D_LEDG |= (1 << P_LEDG);
    D_LEDB |= (1 << P_LEDB);
    O_LEDR |= (1 << P_LEDR);

    _delay_ms(50);

    O_LEDR &= ~(1 << P_LEDR);

    WDTCSR = (1 << WDIE) | (1 << WDP1);

    RCounter = 10;
    GCounter = 35;
    BCounter = 20;

    sei();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();

    while(1)
    {
    }

    return 1;
}

ISR(WATCHDOG_vect)
{
    if(O_LEDR & (1 << P_LEDR))
    {
        O_LEDR &= ~(1 << P_LEDR);
        RCounter = rand() % MAX_DELAY;
    }
    else
    {
        RCounter--;
        if(RCounter == 0)
        {
            O_LEDR |= (1 << P_LEDR);
        }
    }

    if(O_LEDG & (1 << P_LEDG))
    {
        O_LEDG &= ~(1 << P_LEDG);
        GCounter = rand() % MAX_DELAY;
    }
    else
    {
        GCounter--;
        if(GCounter == 0)
        {
            O_LEDG |= (1 << P_LEDG);
        }
    }
    
    if(O_LEDB & (1 << P_LEDB))
    {
        O_LEDB &= ~(1 << P_LEDB);
        BCounter = rand() % MAX_DELAY;
    }
    else
    {
        BCounter--;
        if(BCounter == 0)
        {
            O_LEDB |= (1 << P_LEDB);
        }
    }
}

uint16_t y = 132457;

uint16_t rand()
{
    y ^= y << 13;
    y ^= y >> 7;
    y ^= y << 5;
    return y;
}
