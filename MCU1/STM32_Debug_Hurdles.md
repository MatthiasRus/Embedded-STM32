# STM32 Bare Metal Debug Hurdles — Personal Reference (LVU hexdump)
**Built during ADC + USART driver development on STM32L476RG**

---

## Hurdle 1 — WWDG Infinite Loop During Debug Stepping

**Symptom:** Stepping over any line in CubeIDE immediately jumps to `WWDG_IRQHandler` → `Infinite_Loop` in startup assembly. Code never reaches the next line.

**Root Cause:** The debugger halts the CPU while stepping, but hardware timers keep running. The Window Watchdog timer expires while the CPU is paused, triggering the IRQ. Since no handler is defined, it falls through to `Default_Handler` → `Infinite_Loop`.

**Fix 1 — Freeze watchdog during debug halt:**
```c
// Add to debug configuration OR first line of main
*((volatile uint32_t*)0xE0042008) |= (1 << 9);  // DBGMCU_APB1FZR1, bit 9 = WWDG freeze
```

**Fix 2 — CubeIDE debug configuration:**
Run → Debug Configurations → Debugger tab → Enable "Stop watchdog counters while halted"

**Lesson:** The WWDG does not start itself. Something else enables it. Use `grep -r "WWDG"` in your project to find it. The `Default_Handler` infinite loop is the fallback for ANY unhandled exception — it is not always a watchdog.

---

## Hurdle 2 — Float Variables Showing 0 or Garbage in Debugger

**Symptom:** `float cal1 = TS_CAL1` → cal1 shows 0 in Variables panel. Expressions panel shows TS_CAL1 = 1031. Values are correct in raw memory but wrong after assignment.

**Root Cause:** FPU (Floating Point Unit) not enabled. The Cortex-M4 has a hardware FPU but it is disabled by default after reset. Any float operation without enabling it causes a UsageFault → hardfault → Default_Handler. The debugger shows stale/zero values because the assignment never executed.

**Fix — Enable FPU as the absolute first thing in main:**
```c
int main(void){
    // MUST be first — before SysTick, before anything
    *((volatile uint32_t*)0xE000ED88) |= (0xF << 20);  // enable CP10, CP11
    __asm volatile("DSB");
    __asm volatile("ISB");
    // everything else after this
}
```

**Secondary fix:** Store calibration values as `uint16_t` first, then cast to float in math:
```c
uint16_t cal1 = TS_CAL1;   // correct type — matches the pointer type
uint16_t cal2 = TS_CAL2;
float temperature = ((80.0f / (float)(cal2 - cal1)) * ((float)adcValue - (float)cal1)) + 30.0f;
```

**Lesson:** Never use `float` to store hardware register values directly. Match the storage type to the register width first, convert to float only for math.

---

## Hurdle 3 — TS_CAL Values Wrong / ADC Reading Below CAL1

**Symptom:** adcValue = 914, cal1 = 1031. adcValue < cal1 → temperature formula gives negative result.

**Root Cause:** Factory calibration values (TS_CAL1, TS_CAL2) were measured at VDDA = 3.0V. If your board runs at 3.3V, all ADC readings are scaled higher relative to calibration. The raw ADC value appears lower than expected because the reference is different.

**Verify calibration values in Memory panel:**
Navigate to address `0x1FFF75A8` in the Memory tab. Read the bytes in little-endian order.
- TS_CAL1 address: `0x1FFF75A8` (30°C calibration)
- TS_CAL2 address: `0x1FFF75CA` (110°C calibration)

**Fix — Apply voltage correction factor:**
```c
float adcCorrected = (float)adcValue * (3.0f / 3.3f);
float temperature = ((80.0f / (cal2 - cal1)) * (adcCorrected - cal1)) + 30.0f;
```

**Lesson:** Always check the conditions under which factory calibration was performed. For STM32L4 internal temp sensor: TS_CAL measured at VDDA=3.0V, 12-bit resolution.

---

## Hurdle 4 — System Running on Default MSI Clock (4MHz) Instead of Expected Clock

**Symptom:** UART sends only `?` characters. SysTick delays are 20x too fast. BRR calculation is completely wrong.

**How to check:** In SFR viewer, find RCC → CFGR → SWS field (bits [3:2]). If SWS = 00, system clock is MSI (default ~4MHz). If nothing configures the clock in your code, this is always the state after reset.

**Fix — Switch to HSI16 (16MHz, simple and stable):**
```c
RCC->CR |= (1 << 8);              // HSION — turn on HSI16
while(!(RCC->CR & (1 << 10)));    // wait HSIRDY
RCC->CFGR &= ~(3 << 0);          // clear SW bits
RCC->CFGR |= (1 << 0);           // SW = 01 → HSI16
while((RCC->CFGR & (3 << 2)) != (1 << 2));  // wait SWS confirms switch
```

**Update SysTick for new clock:**
```c
SYSTICK_LOAD = 16000 - 1;  // 1ms at 16MHz (was 80000-1 for 80MHz)
```

**Update BRR for new clock:**
```
BRR = 16,000,000 / 115,200 = 138.88 → 139 (use 0x8B)
```

**Lesson:** Never assume the system clock. Always verify RCC CFGR SWS before debugging any timing-sensitive peripheral. Check it first, assume nothing.

---

## Hurdle 5 — USART Sending Garbled / Partial Bytes (`?` characters)

**Symptom:** Terminal shows `?` for every byte, or only some bytes correct, or UTF-8 style garbage.

**Cause A — Baud rate wrong (see Hurdle 4):** BRR calculated for wrong clock frequency.

**Cause B — Parity accidentally enabled:**
If `NO_PARITY = 0` (same as uninitialized memory), your init code may accidentally enable PCE (parity control enable, CR1 bit 10). Terminal expects 8N1 but gets 8E1. Every byte has an extra parity bit → all characters corrupted.

**Fix — Use non-zero value for NO_PARITY:**
```c
#define NO_PARITY    0   // BAD — matches uninitialized memory
// Change to:
#define NO_PARITY    2   // GOOD — 0 never accidentally matches this
#define EVEN_PARITY  0
#define ODD_PARITY   1
```
Always explicitly set parity in your handle:
```c
usart.USARTx_Config.USART_Parity = NO_PARITY;
```

**Cause C — BRR register ORed instead of assigned:**
```c
pUSARTx->BRR |= calculated_value;  // WRONG — ORs with previous garbage
pUSARTx->BRR  = calculated_value;  // CORRECT — always assign BRR fresh
```

**Lesson:** Default macro values of 0 are dangerous for hardware config. Uninitialized struct members are 0. If your NO_PARITY or NO_FLOW_CONTROL macros equal 0, they silently match uninitialized fields and enable hardware features you didn't ask for.

---

## Hurdle 6 — USART Sending Fragmented Output (Hello → Hel + lo on separate lines)

**Symptom:** Output is mostly correct but occasionally split mid-string. Visible in terminal as broken words on different lines.

**Cause A — Using `screen` as terminal:** `screen` has its own buffering and rendering that corrupts embedded serial output. Not a UART bug.

**Fix:** Use `cat` or `minicom` instead:
```bash
cat /dev/ttyACM0           # simplest, works
minicom -D /dev/ttyACM0 -b 115200  # better for interactive use
```
Never debug UART output with `screen`.

**Cause B — `Delay_ms` inside TXE wait loop:** Calling `Delay_ms(1)` while waiting for TXE allows SysTick interrupt to fire mid-transmission, disrupting timing.

**Fix — Busy-wait only inside SendData, no Delay_ms:**
```c
void USART_SendData(...){
    __asm volatile("CPSID I");      // disable interrupts during TX
    for(uint32_t i = 0; i < Len; i++){
        while(!(pUSARTx->ISR & (1 << 7)));  // TXE busy-wait only
        pUSARTx->TDR = pTxBuffer[i];
    }
    while(!(pUSARTx->ISR & (1 << 6)));     // wait TC after last byte
    __asm volatile("CPSIE I");      // re-enable interrupts
}
```

**Cause C — TC flag already set from previous call:** TC = 1 at the start of the next call exits the end-wait immediately before shift register clears.

**Fix — Clear TC flag at start of each transmission:**
```c
pUSARTx->ICR |= (1 << 6);  // W1C — write 1 to clear TC in ICR
```

---

## Hurdle 7 — RCC Struct Misalignment (Wrong Register Written)

**Symptom:** Enabling a peripheral clock appears to do nothing. SFR viewer shows the wrong register being modified.

**Root Cause:** Missing `uint32_t RESERVED[]` arrays between non-contiguous register blocks in `RCC_RegDef_t`. If AHB3ENR and APB1ENR1 have a gap in the reference manual but your struct doesn't, every register after the gap is at the wrong offset.

**How to verify:**
In SFR viewer, check `RCC->APB1ENR1` address. It must equal `0x40021058`.
If it shows a different address → struct is misaligned.

**Fix:** Always count bytes between registers in RM0351 register map. Fill gaps with:
```c
uint32_t RESERVED0[N];  // where N = gap_bytes / 4
```

**Lesson:** The reference manual register map is the only source of truth. Count every offset. Gaps are real and must be represented in the struct.

---

## Hurdle 8 — sprintf Causing Hardfault / Callstack Shows memchr / _svfprintf_r

**Symptom:** Calling `sprintf` causes a crash. Callstack shows `memchr`, `_svfprintf_r`, `sprintf` — no source available.

**Root Cause:** `sprintf` pulls in newlib's full printf implementation which requires heap, semihosting, or syscall stubs. On bare metal without these, it crashes.

**Fix — Don't use sprintf. Write your own integer/string helpers:**
```c
void USART_SendChar(USART_Handle_t *h, char c){
    while(!(h->USARTx->ISR & (1 << 7)));
    h->USARTx->TDR = (uint8_t)c;
}

void USART_SendString(USART_Handle_t *h, char *str){
    while(*str) USART_SendChar(h, *str++);
    while(!(h->USARTx->ISR & (1 << 6)));  // wait TC
}

void USART_SendNumber(USART_Handle_t *h, int num){
    if(num < 0){ USART_SendChar(h, '-'); num = -num; }
    char buf[10];
    int i = 0;
    do { buf[i++] = '0' + (num % 10); num /= 10; } while(num > 0);
    for(int j = i-1; j >= 0; j--) USART_SendChar(h, buf[j]);
}
```

Then send temperature as:
```c
USART_SendString(&usart, "T: ");
USART_SendNumber(&usart, (int)temperature);
USART_SendChar(&usart, '.');
USART_SendNumber(&usart, (int)((temperature - (int)temperature) * 100));
USART_SendString(&usart, " C\r\n");
```

---

## Quick Diagnostic Checklist

| Symptom | First thing to check |
|---|---|
| Infinite loop on step | WWDG suspend in debug config |
| float = 0 in debugger | FPU enabled? `0xE000ED88 \|= 0xF << 20` |
| Temperature negative | adcValue < cal1 → voltage correction needed |
| UART shows only `?` | RCC CFGR SWS — what clock is actually running? |
| UART garbled/partial | Using screen? Switch to cat or minicom |
| Wrong peripheral clock | RCC struct misaligned — verify register addresses in SFR |
| sprintf crashes | No stdlib on bare metal — write your own helpers |
| Peripheral does nothing | BRR/config register used `\|=` instead of `=` |

---

## Memory Addresses Reference (STM32L476RG)

| Item | Address | Notes |
|---|---|---|
| TS_CAL1 | `0x1FFF75A8` | Factory temp cal at 30°C, VDDA=3.0V |
| TS_CAL2 | `0x1FFF75CA` | Factory temp cal at 110°C, VDDA=3.0V |
| DBGMCU_APB1FZR1 | `0xE0042008` | Bit 9 = freeze WWDG during halt |
| FPU CPACR | `0xE000ED88` | Bits [23:20] = 0xF to enable FPU |
| SysTick CTRL | `0xE000E010` | |
| SysTick LOAD | `0xE000E014` | 16000-1 for 1ms @ 16MHz |
| ADC1 base | `0x50040000` | |
| ADC Common base | `0x50040300` | CCR at +0x08 |
| USART2 base | `APB1_BASE + 0x4400` | |
| RCC APB1ENR1 | `0x40021058` | Verify this in SFR to check struct alignment |
