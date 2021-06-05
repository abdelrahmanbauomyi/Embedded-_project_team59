#include "stdint.h"
#include "math.h"
#include "C:\Keil_v5\Labware\inc\tm4c123gh6pm.h"
#define wake_up 0x30
#define eight_bits_data 0x38
#define clear 0x01
#define display_on 0x0F
#define increment_cursor 0x06


void init(void){
SYSCTL_RCGCGPIO_R |= 0x03; //Activate port A,B
while( (SYSCTL_PRGPIO_R&0x03)==0)  { };
GPIO_PORTB_AMSEL_R &= ~0xFF;
GPIO_PORTB_AFSEL_R &= ~0xFF;
GPIO_PORTB_DEN_R |= 0xFF;
GPIO_PORTB_PCTL_R &= ~0xFFFFFFFF;
GPIO_PORTB_DIR_R |= 0XFF;


GPIO_PORTA_AMSEL_R &= ~0xFF;
GPIO_PORTA_AFSEL_R &= ~0xFF;
GPIO_PORTA_DEN_R |= 0xFF;
GPIO_PORTA_PCTL_R &= ~0xFFFFFFFF;
GPIO_PORTA_DIR_R |= 0XFF;
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
// Function to convert degrees into radians
double rad(double z)
    {
    return z* 3.14159265359 / 180;
    }
		//Distance function using cosine laws of spheres
int distanceCalc(double x1,double y1,double x2,double y2)
{
    double W;
    double sdy;
    double cdy;
    double sx1;
    double cx1;
    double sx2;
    double cx2;
    double V;

    W=rad(y1-y2);
    sdy = sin(W);
    cdy = cos(W);
    x1= rad(x1);
    x2 = rad(x2);
    sx1 = sin(x1);
    cx1 = cos(x1);
    sx2 = sin(x2);
    cx2 = cos(x2);
    W= (cx1 * sx2) - (sx1 * cx2 * cdy);
    W= (W* W);
    W+= (cx2 * sdy) * (cx2 * sdy);
    W= sqrt(W);
    V= (sx1 * sx2) + (cx1 * cx2 * cdy);
    W= atan2(W,V);
    return W* 6372795;
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

