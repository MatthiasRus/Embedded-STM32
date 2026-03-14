# STM32L476RG Bare Metal Driver Library
**No HAL. No CubeMX generated code. Every register written by hand.**

Built from scratch using RM0351 reference manual and STM32L476RG datasheet.

---

## What This Is

A complete bare metal peripheral driver library for the STM32L476RG Nucleo board, built as a learning project to understand embedded systems from the register level up. Every driver was written by reading the reference manual, understanding the hardware, and implementing from scratch.

**Peripherals implemented:**
- GPIO — input, output, alternate function, interrupt modes
- ADC — internal temperature sensor, calibration, single conversion
- USART — TX/RX, polling, custom string/number helpers
- TIM2/TIM3 — 6-channel PWM for servo control
- I2C — master mode, polling, PCA9685 servo driver on top

**Full pipeline:**
```
ROS2 JointState topic → serial → STM32 UART RX → parse → I2C → PCA9685 → 6x PWM → servos
```

---

## Hardware

- STM32L476RG Nucleo-64 board
- PCA9685 16-channel PWM driver (I2C)
- Tower Pro MG995 servos (6x)
- Logic analyzer (Saleae clone, 8ch 24MHz)

---

## Memory Map Reference

| Peripheral | Base Address | Bus |
|---|---|---|
| GPIOA | 0x48000000 | AHB2 |
| GPIOB | 0x48000400 | AHB2 |
| GPIOC | 0x48000800 | AHB2 |
| ADC1 | 0x50040000 | AHB2 |
| ADC Common | 0x50040300 | AHB2 |
| TIM2 | 0x40000000 | APB1 |
| TIM3 | 0x40000400 | APB1 |
| USART2 | 0x40004400 | APB1 |
| I2C3 | 0x40005C00 | APB1 |

**Factory calibration addresses:**
- TS_CAL1 (30°C): `0x1FFF75A8`
- TS_CAL2 (110°C): `0x1FFF75CA`

**Debug registers:**
- FPU enable: `0xE000ED88` — bits [23:20] = 0xF
- DBGMCU freeze WWDG: `0xE0042008` — bit 9

---

## Pin Mapping

| Function | Pin | AF |
|---|---|---|
| USART2 TX | PA2 | AF7 |
| USART2 RX | PA3 | AF7 |
| TIM2 CH1 | PA0 | AF1 |
| TIM2 CH2 | PA1 | AF1 |
| TIM3 CH1 | PA6 | AF2 |
| TIM3 CH2 | PA7 | AF2 |
| TIM3 CH3 | PB0 | AF2 |
| TIM3 CH4 | PB1 | AF2 |
| I2C3 SCL | PC0 | AF4 |
| I2C3 SDA | PC1 | AF4 |

---

## Clock Configuration

Default after reset: MSI at 4MHz. All timing calculations use HSI16 at 16MHz.

```c
// Switch to HSI16
RCC->CR |= (1 << 8);
while(!(RCC->CR & (1 << 10)));
RCC->CFGR &= ~(3 << 0);
RCC->CFGR |= (1 << 0);
while((RCC->CFGR & (3 << 2)) != (1 << 2));
```

**Verify clock:** SFR viewer → RCC → CFGR → SWS field. Must show 01 for HSI16. If 00 = MSI = all timing wrong.

**SysTick at 16MHz:**
```c
SYSTICK_LOAD = 16000 - 1;  // 1ms tick
```

---

## ADC — Internal Temperature Sensor

**Channel:** CH17 (internal temp sensor)
**Sampling time:** 640.5 cycles (SMPx = 7)
**Resolution:** 12-bit

**Temperature formula:**
```c
float adcCorrected = adcValue * (3.0f / 3.3f);  // voltage correction
float temperature = ((80.0f / (cal2 - cal1)) * (adcCorrected - cal1)) + 30.0f;
```

Factory cal values measured at VDDA=3.0V. Running at 3.3V requires correction factor.

**ADC clock:** CKMODE=01 in ADC Common CCR — uses AHB/1 clock.

**Init sequence:**
1. Enable ADC clock — AHB2ENR bit 13
2. Set CKMODE in ADC1_COMMON->CCR bits [17:16] = 01
3. Clear DEEPPWD (CR bit 29)
4. Set ADVREGEN (CR bit 28) — wait 20µs minimum
5. Run calibration — set ADCAL (CR bit 31), wait until clears
6. Configure resolution, channel, sampling time
7. Set ADEN (CR bit 0), wait ADRDY (ISR bit 0)
8. Enable CH17SEL in CCR bit 23, wait 10µs temp sensor startup

---

## USART2 — Serial Communication

**Baud rate:** 115200
**Format:** 8N1
**Protocol:** `$CH:PULSE_US\n`

**BRR calculation at 16MHz:**
```
BRR = 16,000,000 / 115,200 = 139
```

**Command format for servo control:**
```
$1:1500\n  → channel 1, 1500µs pulse
$6:2000\n  → channel 6, 2000µs pulse
```

**Send from Linux terminal:**
```bash
stty -F /dev/ttyACM0 115200
python3 -c "import serial; s=serial.Serial('/dev/ttyACM0',115200); s.write(b'\$1:1500\n')"
```

**Monitor output:**
```bash
cat /dev/ttyACM0 &   # use cat, never screen
minicom -D /dev/ttyACM0 -b 115200
```

---

## TIM2/TIM3 — PWM Servo Control

**Frequency:** 50Hz (20ms period)
**Pulse range:** 1000µs (0°) to 2000µs (180°)
**Neutral:** 1500µs

**Timer math at 16MHz:**
```
PSC = 15  → timer clock = 16MHz / 16 = 1MHz → 1 tick = 1µs
ARR = 19999  → period = 20000µs = 20ms = 50Hz
CCR = pulse_us  → direct microsecond value
```

**Channel mapping:**
- CH1-2: TIM2 (PA0, PA1)
- CH3-6: TIM3 (PA6, PA7, PB0, PB1)

**CCMR1 PWM mode 1:**
- CH1: bits [6:4] = 110, preload bit 3
- CH2: bits [14:12] = 110, preload bit 11

**CCMR2 for CH3/CH4:** same pattern in CCMR2 register.

**CCER output enable:**
- CH1: bit 0, CH2: bit 4, CH3: bit 8, CH4: bit 12

---

## I2C3 — PCA9685 Communication

**Pins:** PC0 (SCL), PC1 (SDA), AF4, open drain, pullup
**Speed:** 400kHz fast mode
**TIMINGR:** `0x00300617` (16MHz HSI, from ST timing tool)

**Why open drain:** Multiple devices share the same wire. Open drain means devices can only pull LOW — the pullup resistor passively pulls HIGH. No short circuit if two devices drive simultaneously.

**PCA9685 address:** 0x40

**I2C transaction structure:**
Every `I2C_MasterSendData` call sends:
- Slave address (who to talk to)
- data[0] = register address inside PCA9685
- data[1..n] = values to write

**Critical — NBYTES must be set BEFORE START:**
```c
pI2Cx->CR2 |= (Len << 16);   // NBYTES first
pI2Cx->CR2 |= (1 << 13);     // START after
```
Setting START before NBYTES causes TXIS to never set — bus lockup.

**PCA9685 init sequence:**
1. Write 0x10 to MODE1 — sleep
2. Write 0x79 to PRE_SCALE — 50Hz PWM frequency
3. Write 0x00 to MODE1 — wake up
4. Wait 500µs
5. Write 0xA0 to MODE1 — enable auto-increment

**PRE_SCALE calculation:**
```
prescale = round(25MHz / (4096 * 50Hz)) - 1 = 121 = 0x79
```

**SetPulse calculation:**
```c
uint16_t ticks = pulse_us * 4096 / 20000;
// 1000us = 204 ticks, 1500us = 307 ticks, 2000us = 409 ticks
```

---

## ROS2 Integration

**Package:** `arm_ros2`
**Topic:** `/arm/joint_state` — `sensor_msgs/JointState`
**Positions:** radians, converted to pulse_us in ServoController node

**Conversion:**
```python
pulse = int(1000 + (pos / math.pi) * 1000)
pulse = max(1000, min(2000, pulse))
```

**Send command:**
```bash
python3 ~/send_servo.py 1 1500
```

---

## Debug Hurdles

### WWDG Infinite Loop During Stepping
Watchdog fires while debugger has CPU halted. Fix: CubeIDE Run → Debug Configurations → Debugger tab → enable "Stop watchdog counters while halted". Or add `*((volatile uint32_t*)0xE0042008) |= (1 << 9)` as first line of main.

### Float Variables Show 0 or Garbage
FPU not enabled. Must be absolute first thing in main:
```c
*((volatile uint32_t*)0xE000ED88) |= (0xF << 20);
__asm volatile("DSB");
__asm volatile("ISB");
```

### ADC Reading Below TS_CAL1
Factory calibration at VDDA=3.0V, board runs at 3.3V. Apply correction: `adcValue * (3.0f / 3.3f)`.

### UART Shows Only `?` Characters
Check RCC CFGR SWS — if 00, system clock is MSI 4MHz not HSI16. BRR calculated for wrong clock = garbage output.

### UART Garbled Output
Never use `screen` for embedded serial — it corrupts output. Use `cat /dev/ttyACM0 &` or `minicom`.

### Parity Accidentally Enabled
`NO_PARITY = 0` matches uninitialized struct fields. Use `NO_PARITY = 2` or always explicitly set parity in handle config.

### BRR Register Corruption
Always use `=` not `|=` for BRR. ORing onto previous value corrupts baud rate.

### sprintf Crashes on Bare Metal
No stdlib on bare metal. Write custom helpers: `USART_SendChar`, `USART_SendString`, `USART_SendNumber`.

### RCC Struct Misalignment
Missing reserved arrays between non-contiguous registers. Verify: RCC->APB1ENR1 must be at `0x40021058` in SFR viewer. If wrong — count every offset in RM0351 and add `uint32_t RESERVED[N]` gaps.

### I2C Bus Lockup — SCL Stuck Low
Failed transaction leaves peripheral holding SCL low. Bus recovery:
```c
GPIOC->MODER &= ~(3 << 0);
GPIOC->MODER |= (1 << 0);   // PC0 output
GPIOC->ODR |= (1 << 0);     // drive HIGH
for(volatile int i = 0; i < 10000; i++);
I2C3_REG_RESET();
for(volatile int i = 0; i < 1000; i++);
I2C_Init(&handle);
```

### TXIS Never Sets on I2C
Either: (1) NBYTES set after START — reorder, NBYTES must come first. (2) Slave not connected — no ACK = TXIS never sets. (3) Wrong I2C address.

### Logic Analyzer Shows Nothing
Check ground connection first — always. Common ground between LA, STM32, and target device. Without common ground, signals are measured relative to different references = garbage.

---

## How to Verify Any Peripheral

1. **Clock enabled?** SFR → RCC → relevant ENR register — bit must be 1
2. **GPIO configured?** SFR → GPIOx → MODER — must show AF mode (10), check AFR for correct AF number
3. **Peripheral enabled?** Check PE/UE/CEN/ADEN bit in CR1
4. **Right clock?** SFR → RCC → CFGR → SWS — must be 01 for HSI16
5. **Signals present?** Logic analyzer on output pins before connecting target hardware

---

## Quick Diagnostic Table

| Symptom | Check First |
|---|---|
| Infinite loop on step | WWDG suspend in debug config |
| float = 0 in debugger | FPU enabled? |
| Temperature negative | adcValue < cal1 → add voltage correction |
| UART only `?` | RCC CFGR SWS — actual clock running? |
| UART garbled | Using screen? Switch to cat |
| Peripheral does nothing | BRR/config used `\|=` instead of `=` |
| I2C SCL stuck low | Bus lockup — manual recovery needed |
| TXIS never sets | NBYTES before START? Slave connected? |
| Servo doesn't move | Common ground? V+ powered separately? |
