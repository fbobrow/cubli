#include "mbed.h"
#include "cubli.h"

// Objects
DigitalOut led(LED1);
Serial pc(SERIAL_TX, SERIAL_RX, NULL, 115200);
Hall hall(M1_SPEED);
Ticker tic_blink, tic_print;

// Interrupt flags and callback functions
bool flag_blink = false;
bool flag_print = false;
void callback_blink() { flag_blink = true; }
void callback_print() { flag_print = true; }

// Main program
int main() 
{
    // Initializations
    tic_blink.attach_us(&callback_blink, dt_blink_us);
    tic_print.attach_us(&callback_print, dt_print_us);
    // Endless loop
    while (true) 
    {
        if (flag_blink) 
        {
            flag_blink = false;
            led = !led;
        }
        if (flag_print) 
        {
            flag_print = false;
            hall.read();
            pc.printf("Speed [rad/s]: %6.2f\n",hall.omega);
        }
    }
}