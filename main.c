#include "stdint.h"
#include "math.h"
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
	GPIO_PORTA_DATA_R&=0x1F; //Rs,Rw,E =0
	GPIO_PORTB_DATA_R=command;
	GPIO_PORTA_DATA_R|=0x80; //E =1
	delay(50);
	GPIO_PORTA_DATA_R&=0x1F;//Rs,Rw,E =0
}

void lcd_data( char data ) {
	GPIO_PORTA_DATA_R&=0x3F; //Rw,E =0
	GPIO_PORTA_DATA_R|=0x20;//Rs=1 
	GPIO_PORTB_DATA_R=data;
	GPIO_PORTA_DATA_R|=0x80; //E=1
	delay(50);
	GPIO_PORTA_DATA_R&=0x1F; //Rs,Rw,E =0
}




void lcd_display_distance(int distance){
  char word[10] ={'D','i','s','t','a','n','c','e',':',' '}; 
  char c1,c2,c3;
  unsigned int i;
  int x1 ;
	
  // getting distance devided into 3 integers and casting them into char	
  x1= distance/100;
  c1 = 48+x1; // 48 is the ascii code of zero
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
		else GPIO_PORTF_DATA_R=0x00;
}
int main()
{
	init();		
	}

#include <C:\Keil\ARM\INC\TI\TM4C123\tm4c123gh6pm.h>
#include "C:\Keil\ARM\RV31\LIB\Luminary\inc\hw_ints.h"
#include "C:\Keil\ARM\RV31\LIB\Luminary\inc\hw_types.h"
#include <C:\Keil\ARM\ARMCC\include\stdint.h>
#include <C:\Keil\ARM\ARMCC\include\stdbool.h>
#include <C:\Keil\ARM\ARMCC\include\stdio.h>
#include <C:\Keil\ARM\ARMCC\include\stdlib.h>

// Intialization of Port A

#define SYSCTL_RCGCGPIO_R                     (*((volatile unsigned long *)0x400FE608))
#define SYSCTL_PRGPIO_R                          (*((volatile unsigned long *)0x400FEA08))
#define GPIO_PORTA_PCTL_R                    (*((volatile unsigned long *)0x4000452C))
#define GPIO_PORTA_AFSEL_R                  (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_DEN_R                     (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_AMSEL_R                 (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_DIR_R                       (*((volatile unsigned long *)0x40004400))
void SystemInit () {};
void PORTA_init(void) {
SYSCTL_RCGCGPIO_R |= 0x01; //Activate port A
while( (SYSCTL_PRGPIO_R&0x01)==0)  { };
GPIO_PORTA_AMSEL_R &= ~0xFF;
GPIO_PORTA_AFSEL_R &= ~0xFF;
GPIO_PORTA_DEN_R |= 0xFF;
GPIO_PORTA_PCTL_R &= ~0xFFFFFFFF;
GPIO_PORTA_DIR_R |= 0XFF;
}
