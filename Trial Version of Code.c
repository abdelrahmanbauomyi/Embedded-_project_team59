#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "math.h"
#include <stdlib.h>     /* atof */
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
	for (i=0;i<10;i++){
	UART0_write(word[i]);
	}
	UART0_write(c1);
	UART0_write(c2);
	UART0_write(c3);
}





int main(){
		char lat[10]="";
		char lon[10]="";
		char inputstring[] = " " ;
		int i=0;
		char latw[]={'l','a','t','=',' '};
		char space=' ';
		char lonw[]={'l','o','n','=',' '};
		double lng1,latt1,lng2,latt2,distance=0;
		
UART0_Init();
UART1_Init();
		
		while(inputstring[i]!='\n')
		{ 
			inputstring[i]=UART1_read();	
			i++;
			if(i==6){
			if(inputstring[3]!='R'){
			for(i=0;i<7;i++){
			inputstring[i]=' ';
			}
			i=0;
			}}
	}
		
	
	if((i==0)&(inputstring[i+21]==0)){
			for(i=0;i<10;i++){lat[i]=inputstring[i+22];	
				if(inputstring[i+23]==','){i=10;}
						}
	}
	
	
	else{
		for(i=0;i<10;i++){lat[i]=inputstring[i+21];	
				if(inputstring[i+22]==','){i=10;}
							}
	}
	
	
	if((i==0)&(inputstring[i+35]==0)){
				for(i=0;i<10;i++){lon[i]=inputstring[i+36];	
	if(inputstring[i+37]==','){i=10;}
					}
	}
	
	
	else{
	for(i=0;i<10;i++){lon[i]=inputstring[i+35];	
				if(inputstring[i+36]==','){i=10;}
							}
	}
	lng1 = atof (lon);
	latt1 = atof(lat);
while (1){
    	delay(800);

	
		i=0;
		
		
  
	// read from gps and put data in 2 arrays 
	while(inputstring[i]!='\n')
		{ 
			inputstring[i]=UART1_read();	
			i++;
			if(i==6){
			if(inputstring[3]!='R'){
			for(i=0;i<7;i++){
			inputstring[i]=' ';
			}
			i=0;
			}}
	}
		
	
	if((i==0)&(inputstring[i+21]==0)){
			for(i=0;i<10;i++){lat[i]=inputstring[i+22];	
				if(inputstring[i+23]==','){i=10;}
						}
	}
	
	
	else{
		for(i=0;i<10;i++){lat[i]=inputstring[i+21];	
				if(inputstring[i+22]==','){i=10;}
							}
	}
	
	
	if((i==0)&(inputstring[i+35]==0)){
				for(i=0;i<10;i++){lon[i]=inputstring[i+36];	
	if(inputstring[i+37]==','){i=10;}
					}
	}
	
	
	else{
	for(i=0;i<10;i++){lon[i]=inputstring[i+35];	
				if(inputstring[i+36]==','){i=10;}
							}
	}
	lng2 = atof (lon);
	latt2 = atof(lat);
	distance += distanceCalc(lng1,latt1,lng2,latt2);
	lng1=lng2;
	latt1=latt2;
	/*
  // displaying the long and lat
		
		for(i=0;i<=sizeof(latw);i++){ 
		UART0_write(latw[i]); 
	}
	for(i=0;i<=sizeof(lat);i++){ 
		UART0_write(lat[i]); 
	} 
	for(i=0;i<=4;i++){ 
		UART0_write(space); 
	}
  for(i=0;i<=sizeof(lonw);i++){ 
		UART0_write(lonw[i]); 
	}
  for(i=0;i<=sizeof(lon);i++){ 
		UART0_write(lon[i]); 
	}	
	*/
	
	// getting distance devided into 3 characters and display them on the screen
	display_distance((int) distance);
	UART0_write('\n');
	
	
	}


}
