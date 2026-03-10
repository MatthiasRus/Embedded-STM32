#include <stdint.h>

#define SYSTICK_BASE    0xE000E010UL
#define SYSTICK_CTRL    (*(volatile uint32_t*)(SYSTICK_BASE + 0x00))
#define SYSTICK_LOAD    (*(volatile uint32_t*)(SYSTICK_BASE + 0x04))
#define SYSTICK_VAL     (*(volatile uint32_t*)(SYSTICK_BASE + 0x08))

void SysTick_Init(void) {
    SYSTICK_LOAD = 80000 - 1;
    SYSTICK_VAL  = 0;
    SYSTICK_CTRL = (1 << 2) | (1 << 1) | (1 << 0);
}


static volatile uint32_t msTick = 0;


void SysTick_Handler(void) {
    msTick++;
}

uint32_t GetTick(void) {
    return msTick;
}

void Delay_ms(uint32_t ms) {
    uint32_t start = GetTick();
    while ((GetTick() - start) < ms);
}
