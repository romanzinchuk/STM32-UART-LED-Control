# STM32 UART LED Controller (Register Level)

## Overview
This project demonstrates **Bare Metal** embedded programming on STM32. 
It implements a serial command interface to control hardware peripherals directly via registers, without using high-level libraries like HAL or LL.

The firmware listens for ASCII commands over UART and toggles the on-board LED accordingly.

## Key Features
- **Direct Register Access:** All peripherals (RCC, GPIO, USART) are configured by manipulating memory-mapped registers directly.
- **UART Communication:** Implements a driver for **USART2** (115200 baud, 8N1).
- **Command Parsing:** Real-time processing of incoming byte stream.
- **Efficiency:** Zero overhead compared to HAL-based implementations.

## Hardware Required
* **MCU Board:** STM32F4 Discovery (or any STM32F4 series)
* **Interface:** USB-to-TTL Converter (or on-board ST-Link VCP)
* **Terminal Software:** PuTTY / TeraTerm / Serial Monitor

## Pinout Configuration
| Pin | Function | Mode | Description |
| :--- | :--- | :--- | :--- |
| **PA2** | USART2_TX | Alternate Function (AF7) | Transmit Data |
| **PA3** | USART2_RX | Alternate Function (AF7) | Receive Data |
| **PA5** | LD2 (Green) | Output Push-Pull | User LED |

## Technical Implementation details
### 1. Baud Rate Calculation
The baud rate is manually calculated for **115200 bps** at **16 MHz** system clock.
- `USART_DIV` = 16,000,000 / (16 * 115200) = 8.68
- **Mantissa:** 8 (`0x8`)
- **Fraction:** 0.68 * 16 ≈ 11 (`0xB`)
- **Register Value (BRR):** `0x8B`

### 2. GPIO Alternate Functions
Demonstrates complex pin configuration by setting **AF7** in the `AFR[]` registers to connect GPIO pins to the USART2 peripheral.

## How to Test
1. Connect the board to PC.
2. Open a Serial Terminal (Baud: 115200).
3. Send commands:
    * Type `1` -> LED turns **ON**
    * Type `0` -> LED turns **OFF**

## Code Snippet
```c
// Example of direct register manipulation for atomic bit setting
if (cmd == '1') {
    GPIOA->ODR |= (1 << 5);  // Set bit 5 (LED ON)
} 
else if (cmd == '0') {
    GPIOA->ODR &= ~(1 << 5); // Clear bit 5 (LED OFF)
}
