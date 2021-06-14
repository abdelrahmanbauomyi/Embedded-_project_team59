#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "stdlib.h"     /* atof /
#include <stdio.h>      / printf, NULL /
#include <stdlib.h>     / strtod */
#include "math.h"

void SystemInit() {}

void UART0_Init(void){
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;

    UART0_CTL_R &= ~UART_CTL_UARTEN;
    //set buad rate devider
    UART0_IBRD_R = 104;
    UART0_FBRD_R = 11;
        UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
    UART0_CTL_R |= (UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE);

    GPIO_PORTA_AFSEL_R |= 0x03;
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & ~0xFF) | (GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX);
    GPIO_PORTA_DEN_R |= 0x03;
}

char UART0_read(void){
    while((UART0_FR_R&0x10) == 0x10);
    return UART0_DR_R & 0xFF;
}

void UART0_write(char c){
    while((UART0_FR_R & UART_FR_TXFF) != 0);
    UART0_DR_R = c;
}
void delay(int x) //delay for x milliseconds.
{
int i,j;
for( i =0 ; i<x ; i++){for( j=0 ; j<3180 ; j++){}}
}

void UART1_Init(void){
    SYSCTL_RCGCUART_R |= 0x02;
    while((SYSCTL_PRUART_R & 0x02) == 0);
    SYSCTL_RCGCGPIO_R |= 0x02;
        while((SYSCTL_PRGPIO_R & 0x02) == 0);

      GPIO_PORTB_CR_R |= 0x1F;
    GPIO_PORTB_AMSEL_R &= ~0x1F;
    GPIO_PORTB_AFSEL_R |= 0x03;
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & ~0xFF) | (GPIO_PCTL_PB0_U1RX | GPIO_PCTL_PB1_U1TX);
    GPIO_PORTB_DEN_R |= 0x1F;
    GPIO_PORTB_DIR_R |= 0x1E;
    GPIO_PORTB_DIR_R &= ~0x01;

    UART1_CTL_R &= ~UART_CTL_UARTEN;
    //set buad rate devider
    UART1_IBRD_R = 104;
    UART1_FBRD_R = 11;
      UART1_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
    UART1_CTL_R |= (UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE);
}

char UART1_read(void){
    while((UART1_FR_R & 0x10) == 0x10);
    return UART1_DR_R & 0xFF;
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
  distance = distance - x1 *10;
  c3 = 48+distance;
    for (i=0;i<10;i++){
    UART0_write(word[i]);
    }
    UART0_write(c1);
    UART0_write(c2);
    UART0_write(c3);
}
double rad(double z)
    {
    return z * 3.14159265359 / 180;
    }
unsigned long distanceCalc(unsigned long x1,unsigned long y1,unsigned long x2,unsigned long y2)
{
    unsigned long W;
    unsigned long sdy;
    unsigned long cdy;
    unsigned long sx1;
    unsigned long cx1;
    unsigned long sx2;
    unsigned long cx2;
    unsigned long V;

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
        W= W* 6372795;
    return W;
}
int main(){
  char lon1[7]={'6','9','.','2','5','0','4'};
      char lat1[7]={'1','3','.','2','5','0','4'};
  char lon2[7]={'4','5','.','2','5','0','4'};
          char lat2[7]={'4','6','.','2','5','0','4'};

    int i=0;
            unsigned long distance = 0;
  double r;
  char temp[]=" ";
    int num1 = 0;
    int num2 = 0;
    char buffer [256];
  unsigned long u1 ,u2 , u3 ,u4;
      UART0_Init();
  UART1_Init();
      display_distance(69);
u1 = strtoul (lon1, NULL, 0);
u2 = strtoul (lat1, NULL, 0);
u3 = strtoul (lon2, NULL, 0);
u4 = strtoul (lat2, NULL, 0);



  while(1){ 

  display_distance((int) u1);
          display_distance((int) u2);
  display_distance((int) u3);
  display_distance((int) u4);
  delay(800);
  UART0_write('\n');
        distance +=distanceCalc(u1 , u2 ,u3 ,u4);
  }
}