#include "stm32f4xx.h"

// --- ФУНКЦІЇ UART (ПИСАТИ І ЧИТАТИ) ---

void UART_SendChar(char c) {
    // Чекаємо, поки буфер передачі (TXE) стане пустим
    while (!(USART2->SR & (1 << 7))); // TXE bit is 7
    USART2->DR = c;
}

void UART_SendString(char* str) {
    while (*str) UART_SendChar(*str++);
}

int main(void) {
    // --- 1. НАЛАШТУВАННЯ (SETUP) ---

    // Вмикаємо тактування GPIOA (порт ніжок) і USART2 (порт зв'язку)
    RCC->AHB1ENR |= (1 << 0);  // GPIOA EN
    RCC->APB1ENR |= (1 << 17); // USART2 EN

    // Налаштовуємо PA2 (TX) і PA3 (RX) на режим "Alternate Function" (10)
    // PA2: біти 4,5 -> ставимо 10
    GPIOA->MODER &= ~(3U << (2 * 2));
    GPIOA->MODER |=  (2U << (2 * 2));
    // PA3: біти 6,7 -> ставимо 10
    GPIOA->MODER &= ~(3U << (3 * 2));
    GPIOA->MODER |=  (2U << (3 * 2));

    // Вказуємо, що Alternate Function - це саме AF7 (USART2)
    // AFR[0] (Low register) відповідає за ніжки 0-7
    GPIOA->AFR[0] |= (7 << (2 * 4)); // Для PA2
    GPIOA->AFR[0] |= (7 << (3 * 4)); // Для PA3

    // Налаштовуємо PA5 (LED) на режим Output (01)
    GPIOA->MODER &= ~(3U << (5 * 2));
    GPIOA->MODER |=  (1U << (5 * 2));

    // Налаштовуємо швидкість UART (115200 бод при 16 МГц)
    USART2->BRR = 0x008B; // 139

    // Вмикаємо UART: TE (Transmit), RE (Receive), UE (Module Enable)
    USART2->CR1 = (1 << 3) | (1 << 2) | (1 << 13);

    // --- ПРИВІТАННЯ ---
    UART_SendString("\r\n--- SYSTEM STARTED ---\r\n");
    UART_SendString("Type '1' to LED ON\r\n");
    UART_SendString("Type '0' to LED OFF\r\n");

    // --- 2. ГОЛОВНИЙ ЦИКЛ (LOOP) ---
    while(1) {
        // Перевіряємо, чи прийшов байт (RXNE bit 5)
        if (USART2->SR & (1 << 5)) {
            char data = USART2->DR; // Читаємо байт

            if (data == '1') {
                GPIOA->ODR |= (1 << 5); // Запалити LED
                UART_SendString("LED IS ON!\r\n");
            }
            else if (data == '0') {
                GPIOA->ODR &= ~(1 << 5); // Погасити LED
                UART_SendString("LED IS OFF!\r\n");
            }
        }
    }
}
