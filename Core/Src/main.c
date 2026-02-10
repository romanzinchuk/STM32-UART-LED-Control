#include "stm32f4xx.h"

void UART_SendChar(char c) {
    // Wait for TXE (Transmit Data Register Empty)
    while (!(USART2->SR & (1 << 7))); 
    USART2->DR = c;
}

void UART_SendString(char* str) {
    while (*str) UART_SendChar(*str++);
}

int main(void) {
    // --- Hardware Configuration ---

    // 1. Enable Clocks: GPIOA (AHB1) and USART2 (APB1)
    RCC->AHB1ENR |= (1 << 0);
    RCC->APB1ENR |= (1 << 17);

    // 2. Configure PA2 (TX) and PA3 (RX) for Alternate Function
    GPIOA->MODER  &= ~((3U << 4) | (3U << 6));  // Clear mode bits
    GPIOA->MODER  |=  ((2U << 4) | (2U << 6));  // Set AF mode (Binary 10)
    
    // Set AF7 (USART2) for Pin 2 and Pin 3 in Alternate Function Low Register
    GPIOA->AFR[0] |=  ((7 << 8) | (7 << 12)); 

    // 3. Configure PA5 (LED) as Output
    GPIOA->MODER &= ~(3U << 10);
    GPIOA->MODER |=  (1U << 10);

    // 4. Configure USART2
    // Baud Rate Calculation: 16 MHz / (16 * 115200) ~= 8.68 -> Mantissa=8, Fraction=11 -> 0x8B
    USART2->BRR = 0x008B; 
    
    // Enable UART (UE), Transmitter (TE), Receiver (RE)
    USART2->CR1 = (1 << 13) | (1 << 3) | (1 << 2);

    // --- Main Loop ---
    UART_SendString("\r\n--- STM32 UART Controller Ready ---\r\n");

    while(1) {
        // Check RXNE (Read Data Register Not Empty)
        if (USART2->SR & (1 << 5)) {
            char cmd = USART2->DR; // Read received byte

            if (cmd == '1') {
                GPIOA->ODR |= (1 << 5);  // LED ON
                UART_SendString(" -> LED ON\r\n");
            }
            else if (cmd == '0') {
                GPIOA->ODR &= ~(1 << 5); // LED OFF
                UART_SendString(" -> LED OFF\r\n");
            }
        }
    }
}