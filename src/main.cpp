#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <serial.h>
#include <ledbutton.h>

int main(void) {
//Funktion för led o knapp
    //buttonlogic();

    uart_init(9600); // Init UART med baudhastighet
    DDRB |= LED_PIN;
    char array[RX_BUFFER_SIZE]; // Buffert för att lagra ev inkommen rad

    while (1) {
        uart_rec_str(array, RX_BUFFER_SIZE); // Läs in en rad
        parse_led_command(array); // Parsar cmd och checka led 
    }

//Skicka ett tecken
    //uart_putchar('A');

    //Skicka en sträng
    //uart_transmit_string("Hello, world!\n");

    //Ta emot tecken och skickar tbx samma tecken
    //uart_echo();

    //Läs in en rad tills \n (line ending viktigt)
   // char received_str[RX_BUFFER_SIZE];
    //uart_rec_str(received_str, RX_BUFFER_SIZE);
   // uart_transmit_string("Received string: ");
   // uart_transmit_string(received_str);

    return 0;
}
