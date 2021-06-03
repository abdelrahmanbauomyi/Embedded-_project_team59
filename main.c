#include "stdint.h"
#include "C:\Keil_v5\Labware\inc\tm4c123gh6pm.h"
#define Red 0x02
#define blue 0x04
#define green 0x08
void init(void)
{

}
void delay(int x)
{

for(int i =0 ; i<x ; i++)
{
for( int j=0 ; j<x ; i++)
{
}
}
}
void lcd_command( char command )
{
	GPIO_PORTA_DATA_R&=0x1F;
	GPIO_PORTB_DATA_R=command;
	GPIO_PORTA_DATA_R|=0x80;
	delay(50);
	GPIO_PORTA_DATA_R&=0x1F;
}

void lcd_data( char data ) 
{
	GPIO_PORTA_DATA_R&=0x3F;
	GPIO_PORTA_DATA_R|=0x20;
	GPIO_PORTB_DATA_R=data;
	GPIO_PORTA_DATA_R|=0x80;
	delay(50);
	GPIO_PORTA_DATA_R&=0x1F;
}
void lcd_number_display(int x)
{
	
}
void lcd_display(int distance)
{
	lcd_command(0x30);
	lcd_command(0x38);
	lcd_command(0x01);
	lcd_command(0x0F);
	lcd_data('d');
	lcd_command(0x06);
	lcd_data('i');
	lcd_command(0x06);
	lcd_data('s');
	lcd_command(0x06);
	lcd_data('t');
	lcd_command(0x06);
	lcd_data('a');
	lcd_command(0x06);
	lcd_data('n');
	lcd_command(0x06);
	lcd_data('c');
	lcd_command(0x06);
	lcd_data('e');
	lcd_command(0x06);
	lcd_data(':');
	lcd_command(0x06);
	lcd_data(distance);
	lcd_command(0x06);
}
int main()
{
	init();

		
	}

