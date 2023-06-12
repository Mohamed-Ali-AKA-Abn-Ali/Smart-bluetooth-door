#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define F_CPU 1000000UL  // Clock Speed
#define BAUD 9600        // BAUDRATE - 9600
#define MYUBRR F_CPU/16/BAUD-1  // asynchronous Normal mode in USART
#define Blue_LED_ON    49
#define Blue_LED_OFF   50
#define Yellow_Toggle  51
volatile uint8_t UART_Buffer;  // Declare UART_Buffer as a global variable

void UART_RX_Init() {
    SPBRGH = 0;  // Set For High-Speed Baud Rate
    SPBRG = 25;  // Set The Baud Rate To Be 9600 bps
    // Enable The Asynchronous Serial Port
    SYNC = 0;
    SPEN = 1;
    // Set The RX-TX Pins to be in UART mode (not io)
    TRISC6 = 1;  // As stated in the datasheet
    TRISC7 = 1;  // As stated in the datasheet
    //--[ Enable UART Receiving Interrupts ]--
    RCIE = 1;  // UART Receiving Interrupt Enable Bit
    PEIE = 1;  // Peripherals Interrupt Enable Bit
    GIE = 1;   // Global Interrupt Enable Bit
    //------------------
    CREN = 1;  // Enable Data Continuous Reception
}

ISR(USART_RXC_vect) {  // Define the ISR function for USART RX Complete
    UART_Buffer = UDR;  // Read the received data buffer

    // This could have been done within the main loop. Since it's not
    // Excessive processing, so it's OK to do it here below
    if (UART_Buffer == Blue_LED_ON) {
        RB0 = 1;  // Blue LED ON
    }
    if (UART_Buffer == Blue_LED_OFF) {
        RB0 = 0;  // Blue LED OFF
    }
    if (UART_Buffer == Yellow_Toggle) {
        RB1 = ~RB1;  // Toggle Yellow LED
    }

    // Clear The Interrupt Flag
    USART_RXC_vect = 0;
}

int main(void) {
    UART_RX_Init();

    while (1) {
        // Do other stuff here
        _delay_ms(100);
    }

    return 0;
}
