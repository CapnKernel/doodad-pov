#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

struct row {
    unsigned char h;
    unsigned char l;
};

static const struct row image[] PROGMEM = 
{
#if 0
  { 0b00000000, 0b00000001 },
  { 0b00000000, 0b00000010 },
  { 0b00000000, 0b00000100 },
  { 0b00000000, 0b00001000 },
  { 0b00000000, 0b00010000 },
  { 0b00000000, 0b00100000 },
  { 0b00000000, 0b01000000 },
  { 0b00000000, 0b10000000 },
  { 0b00000001, 0b00000000 },
  { 0b00000010, 0b00000000 },
  { 0b00000100, 0b00000000 },
  { 0b00001000, 0b00000000 },
  { 0b00010000, 0b00000000 },
  { 0b00100000, 0b00000000 },
  { 0b01000000, 0b00000000 },
  { 0b10000000, 0b00000000 },
  { 0b00000000, 0b00000000 },
  { 0b00000001, 0b10000000 },
  { 0b00000010, 0b01000000 },
  { 0b00000100, 0b00100000 },
  { 0b00001000, 0b00010000 },
  { 0b00010000, 0b00001000 },
  { 0b00100000, 0b00000100 },
  { 0b01000000, 0b00000010 },
  { 0b10000000, 0b00000001 },
  { 0b01000000, 0b00000010 },
  { 0b00100000, 0b00000100 },
  { 0b00010000, 0b00001000 },
  { 0b00001000, 0b00010000 },
  { 0b00000100, 0b00100000 },
  { 0b00000010, 0b01000000 },
  { 0b00000001, 0b10000000 },
#else
#include IMG
#endif

};

const int rows = sizeof(image) / sizeof(image[0]);

#if 0
hhhhhhhhllllllll
7654321076543210
ccccccbbbbbbdddd
5432105432107654

  hhhhhh   hhllll llll
  765432   107654 3210
cccccccc bbbbbbbb dddddddd
77543210 76543210 76543210

c0-5 b0-5 d4-7
#endif

static void setLeds(unsigned char h, unsigned char l)
{
    PORTC &= ~0b00111111;    
    PORTC |= h >> 2;
    PORTB &= ~0b00111111;    
    PORTB |= (h & 0b11) << 4 | l >> 4;
    PORTD &= ~0b11110000;    
    PORTD |= l << 4;
#ifdef LINUX
    printf(" H="); tobin(h);
    printf(" L="); tobin(l);
    printf(" PORTC="); tobin(PORTC);
    printf(" PORTB="); tobin(PORTB);
    printf(" PORTD="); tobin(PORTD);
    printf("\n");
#endif
}

int main()
{
    // unsigned char b = pgm_read_byte(&image[0].h);

    DDRC |= 0b00111111;
    DDRB |= 0b00111111;
    DDRD |= 0b11110000;
	
    char mode = 1;
    unsigned char row = 0;
	while (1)
	{
        unsigned char h = pgm_read_byte(&image[row].h);
        unsigned char l = pgm_read_byte(&image[row].l);
		// setLeds(h, l | (mode == 1 ? 1: 0));
        setLeds(h, l);
      	// _delay_ms(1);
      	_delay_us(4000);

        if (mode == 1 && row == rows - 1)
        {
            mode = -mode;

            continue;
        }
        if (mode == -1 && row == 0)
        {
            mode = -mode;

            continue;
        }
        row += mode;
	}
}

