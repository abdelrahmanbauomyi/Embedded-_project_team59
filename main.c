#include "stdint.h"
#include "C:\Keil_v5\Labware\inc\tm4c123gh6pm.h"
#define wake_up 0x30
#define eight_bits_data 0x38
#define clear 0x01
#define display_on 0x0F
#define increment_cursor 0x06

void init(void){

}
void delay(int x){
int i,j;
for( i =0 ; i<x ; i++){for( j=0 ; j<x ; i++){}}
}


void lcd_command( char command ){
	GPIO_PORTA_DATA_R&=0x1F;
	GPIO_PORTB_DATA_R=command;
	GPIO_PORTA_DATA_R|=0x80;
	delay(50);
	GPIO_PORTA_DATA_R&=0x1F;
}

void lcd_data( char data ) {
	GPIO_PORTA_DATA_R&=0x3F;
	GPIO_PORTA_DATA_R|=0x20;
	GPIO_PORTB_DATA_R=data;
	GPIO_PORTA_DATA_R|=0x80;
	delay(50);
	GPIO_PORTA_DATA_R&=0x1F;
}




void lcd_display_distance(int distance){
  char word[10] ={'D','i','s','t','a','n','c','e',':',' '}; 
  char c1,c2,c3;
  unsigned int i;
  int x1 ;
	
	
  x1= distance/100;
  c1 = 48+x1;
  distance = distance - x1 * 100;
  x1 = distance/10; 
  c2 = 48 + x1;
  distance = distance - x1*10;
  c3 = 48+distance;
	
	
	lcd_command(wake_up);
	lcd_command(eight_bits_data);
	lcd_command(clear);
	lcd_command(display_on);
	
	for (i=0;i<10; i++){
	lcd_data(word[i]);
	lcd_command(increment_cursor);
	}
	lcd_data(c1);
	lcd_command(increment_cursor);
	lcd_data(c2);
	lcd_command(increment_cursor);
	lcd_data(c3);
	lcd_command(increment_cursor);
	
}
void checkDistance(int distance){
    if (distance > 100){
    GPIO_PORTF_DATA_R |= 0X02;
    }
}
int main()
{
	init();		
	}

