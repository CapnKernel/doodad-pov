// Primitive USB Doodad persistence-of-vision (POV) tool
// Mitch Davis, Dec 2010
// Released into the public domain so you can do what you like with it
//
// TODO: Add code which listens to a motion sensor, measures how long
// it takes the user to wave the Doodad, and adjust the timing to suit.

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

struct column {
    unsigned char h;
    unsigned char l;
};

static const struct column image[] PROGMEM = 
{
#include IMG
};

const int columns = sizeof(image) / sizeof(image[0]);

/*
 * There are three 8-bit I/O ports on the ATmega328P, called
 * PORTB, PORTC and PORTD.  The bits in the I/O ports are
 * mapped to pins on the chip.  Some of the pins are used
 * for special functions, and some (16) of them are connected
 * to the Doodad's 16 light emitting diodes.
 *
 * The pins which drive the LEDs are:
 *
 * PORT C pins 0-5: LEDs 11-16
 * PORT B pins 0-5: LEDs 5-10
 * PORT D pins 4-7: LEDs 1-4
 *
 * Let us consider two eight-bit values h and l, which contain the
 * upper and lower 8 bits for the LEDs.  Another way of writing the
 * mapping is:
 *
 *    hhhhhh   hhllll llll
 *    765432   107654 3210
 *  cccccccc bbbbbbbb dddddddd
 *  76543210 76543210 76543210
 *
 * This routine takes those two bytes, and slices and dices the
 * bits to map onto the three ports, while preserving the values
 * on the non-LED bits of the ports.
 *
 * TODO: Does using C bitfields generate tighter code?
 */

static void setLeds(const unsigned char h, const unsigned char l)
{
    // Feed the upper six bits of H into the lower 6 bits of PORTC.
    PORTC &= ~0b00111111;    
    PORTC |= h >> 2;
    // Feed the lower two bits and the upper four bits of l into the
    // lower six biths of PORTB.
    PORTB &= ~0b00111111;    
    PORTB |= (h & 0b11) << 4 | l >> 4;
    // Feed the lower four bits of l into the upper four bits of PORTD.
    PORTD &= ~0b11110000;    
    PORTD |= l << 4;
}

int main()
{
    // Set all the pins of the ports which control LEDs to be outputs.
    DDRC |= 0b00111111;
    DDRB |= 0b00111111;
    DDRD |= 0b11110000;

    // mode: 1=left-to-right, -1=right-to-left    	
    char mode = 1;
    unsigned char column = 0; // Which column we're displaying
	while (1)
	{
        // Read the column data from program memory.
        unsigned char h = pgm_read_byte(&image[column].h);
        unsigned char l = pgm_read_byte(&image[column].l);
        setLeds(h, l);
      	_delay_us(4000);

        // If we're at the end, change direction.
        if (mode == 1 && column == columns - 1)
        {
            mode = -mode;

            continue;
        }
        if (mode == -1 && column == 0)
        {
            mode = -mode;

            continue;
        }

        // If not, move across one more column
        column += mode;
	}
}

