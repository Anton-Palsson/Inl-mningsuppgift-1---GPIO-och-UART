#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#define BUTTON_PIN PB1
#define SERIAL_8N1 ((1 << UCSZ00) | (1 << UCSZ01))
#define LED_PIN (1 << PB0)

void uart_init(unsigned long baud) {
    // Beräkna baud_setting för önskad baudhastighet
    uint16_t baud_setting = (F_CPU / 8 / baud - 1) / 2;

    // Aktivera dubbel hastighetsläge
    UCSR0A = (1 << U2X0);

    // UBRRH och UBRRL
    UBRR0H = baud_setting >> 8;
    UBRR0L = baud_setting;

    // 8N1
    UCSR0C = SERIAL_8N1;

    // Aktivera Rx och Tx
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

// Skicka ett enskilt tecken över UART
void uart_putchar(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

//Ta emot ett enskilt tecken över UART
char uart_getchar(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

// Funktion för att skicka en sträng över UART
void uart_transmit_string(const char* str) {
    //loop - null-terminering
    while (*str != '\0') {
        uart_putchar(*str);
        str++;
    }
}

// Ta emot en sträng över UART
void uart_receive_string(char* str, int max_length) {
    int i = 0;
    char received_char;
    do {
        received_char = uart_getchar();
        str[i++] = received_char;
    } while (received_char != '\0' && i < max_length - 1); // Maxlängd och null-tecknets hantering
    str[i] = '\0'; // Lägg till null-terminator
}

// Vänta på ett inkommande tecken (uart_getchar) och sedan skicka tillbaka samma tecken
void uart_echo() {
    char received_char = uart_getchar();
    uart_putchar(received_char);
    _delay_ms(50);
}

// Läs in en rad tills \n (line feed)
void uart_rec_str(char* str, int max_length) {
    int i = 0;
    char received_char;
    do {
        received_char = uart_getchar();
        str[i++] = received_char;
    } while (received_char != '\n' && i < max_length - 1);
    str[i] = '\0';
}

// Parsa kommandot för att kontrollera LED-tillståndet
void parse_led_command(const char* cmd) {
    int value;
    if (sscanf(cmd, "ledpower %d", &value) == 1) {
        if (value > 127) {
            PORTB |= LED_PIN; // Tänd LED
        } else {
            PORTB &= ~LED_PIN; // Släck LED
        }
    }
}