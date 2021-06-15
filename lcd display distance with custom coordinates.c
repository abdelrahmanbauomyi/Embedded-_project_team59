#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "math.h"
#include <stdlib.h>     /* atof */
#define wake_up 0x30
#define eight_bits_data 0x38
#define clear 0x01
#define display_on 0x0F
#define increment_cursor 0x06
void SystemInit() {}
void init(void){
SYSCTL_RCGCUART_R |= 0x02;    //Activate UART1
SYSCTL_RCGCGPIO_R |= 0x27;         //Activate port A,B,C,F
while(!(SYSCTL_PRGPIO_R&0x27));
//init port A
GPIO_PORTA_LOCK_R=0x4C4F434B;
GPIO_PORTA_CR_R =0xFF;
GPIO_PORTA_AMSEL_R &=~ 0xFF;
GPIO_PORTA_AFSEL_R &=~ 0xE0;
GPIO_PORTA_DEN_R |= 0xE0;
GPIO_PORTA_PCTL_R &= ~0xFFF00000;
GPIO_PORTA_DIR_R |= 0XE0;
//init port B
GPIO_PORTB_LOCK_R=0x4C4F434B;
GPIO_PORTB_CR_R =0xFF;
GPIO_PORTB_AMSEL_R &= ~0xFF;
GPIO_PORTB_AFSEL_R &= ~0xFF;
GPIO_PORTB_DEN_R |= 0xFF;
GPIO_PORTB_PCTL_R &= ~0xFFFFFFFF;
GPIO_PORTB_DIR_R |= 0XFF;
//init port F
GPIO_PORTF_LOCK_R=0x4C4F434B;
GPIO_PORTF_CR_R =0x1F;
GPIO_PORTF_AFSEL_R=0;
GPIO_PORTF_PCTL_R=0;
GPIO_PORTF_AMSEL_R=0;
GPIO_PORTF_DIR_R=0x0E;
GPIO_PORTF_DEN_R=0x1F;
GPIO_PORTF_PUR_R=0x11;

}
void UART1_Init(void){
    SYSCTL_RCGCUART_R |= 0x02;
    while((SYSCTL_PRUART_R & 0x02) == 0);
    SYSCTL_RCGCGPIO_R |= 0x04;
        while((SYSCTL_PRGPIO_R & 0x04) == 0);
        GPIO_PORTC_AMSEL_R &= ~0x30;
    GPIO_PORTC_AFSEL_R |= 0x30;
    GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R &0xFF00FFFF) | (GPIO_PCTL_PC4_U1RX | GPIO_PCTL_PC5_U1TX);
    GPIO_PORTC_DEN_R |= 0x30;
    GPIO_PORTC_DIR_R |= 0x20;
    GPIO_PORTC_DIR_R &= ~0x10;

    UART1_CTL_R &= ~UART_CTL_UARTEN;
    //set buad rate devider
    UART1_IBRD_R = 104;
    UART1_FBRD_R = 11;
      UART1_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
    UART1_CTL_R |= (UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE);
}


char UART0_read(void){
    while((UART0_FR_R&0x10) == 0x10);
    return UART0_DR_R & 0xFF;
}

void UART0_write(char c){
    while((UART0_FR_R & UART_FR_TXFF) != 0);
    UART0_DR_R = c;
}


char UART1_read(void){
    while((UART1_FR_R & 0x10) == 0x10);
    return UART1_DR_R & 0xFF;
}
void delay(int x) //delay for x milliseconds.
{
int i,j;
for( i =0 ; i<x ; i++){for( j=0 ; j<3180 ; j++){}}
}

// Function to convert degrees into radians
double rad(double z)
    {
    return z* 3.14159265359 / 180;
    }
		//Distance function using cosine laws of spheres
double distanceCalc(double x1,double y1,double x2,double y2)
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


void lcd_command( unsigned char command ){
    GPIO_PORTA_DATA_R&=0x1F; //Rs,Rw,E =0
    GPIO_PORTB_DATA_R=command;
    GPIO_PORTA_DATA_R|=0x80; //E =1
    delay(0);
    GPIO_PORTA_DATA_R&=0x1F;//Rs,Rw,E =0
    delay(2);
}

void lcd_data( unsigned char data ) {
    GPIO_PORTA_DATA_R&=0x3F; //Rw,E =0
    GPIO_PORTA_DATA_R|=0x20;//Rs=1 
    GPIO_PORTB_DATA_R=data;
    GPIO_PORTA_DATA_R|=0x80; //E=1
    delay(0);
    GPIO_PORTA_DATA_R&=0x1F; //Rs,Rw,E =0
    delay(2);
}

void display_distance(int distance){
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
    lcd_command(0x80);
    delay(500);
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
    delay(500);
}

double conv(double c){
int x;
	double y;
	double diff;
	x = (int)(c/100);
	y = c/100;
	diff = y - (double) x;
	diff = diff*100;
	diff = diff/60;
	return (x+diff);
}

void checkDistance(int distance){
    if (distance >= 100){
    GPIO_PORTF_DATA_R = 0X02;
    }

}

int main(){
		
	double lats[6]={30.287586,30.287561,30.28748,30.287414,30.287319,30.287180};
	double lngs[6]={31.748293,31.748124,31.747900,31.747595,31.747295,31.746638};
	int i=1;
		double lng1,latt1,lng2,latt2,distance=0,lng,latt,disch;
init();	
	UART1_Init();
		display_distance(distance);
		
	lng1 = lngs[0];
	latt1 = lats[0];
	//display_distance((int)distance);

while (1){
if(i>5){
	break;
}    
		
		  
	// read from gps and put data in 2 arrays 
	
	lng2 = lngs[i];
	latt2 = lats[i];

	
	
		distance += distanceCalc(lng1,latt1,lng2,latt2);
		lng1=lng2;
		latt1=latt2;
	
	i++;

	delay(800);
display_distance((int)distance);
checkDistance(distance);
	
	}
display_distance((int)distance);


}