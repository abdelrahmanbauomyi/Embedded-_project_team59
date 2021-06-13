#include "tm4c123gh6pm.h"
#include "stdint.h"

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
double getLongitude(char* str) {
    int i;
    double deg = 0, degm = 10, min = 0, minm = 10, sec = 0, secm = 10000, seconds;
    for (i = 0; i < 10;i++) {
        if (i < 2) {
            deg += (str[i] - '0') * degm;
            degm = degm / 10;
        }
        else if (i >= 2 && i < 4) {
            min += (str[i] - '0') * minm;
            minm = minm / 10;
        }
        else if (i >= 5 && i < 10) {
            sec += (str[i] - '0') * secm;
            secm = secm / 10;
        }
    }
    seconds = (double)sec / 1000;
    return  deg + (min / 60) + (seconds / 3600);

}

double getlatitude(char* str) {
    int i;
    double deg2 = 0, degm2 = 10, min2 = 0, minm2 = 10, sec2 = 0, secm2 = 10000, seconds2;
    for (i = 0; i < 10;i++) {
        if (i < 2)  {
            deg2 += (str[i] - '0') * degm2;
            degm2 = degm2 / 10;
        }
        else if (i >= 2 && i < 4) {
            min2 += (str[i] - '0') * minm2;
            minm2 = minm2 / 10;
        }
        else if (i >= 5 && i < 10) {
            sec2 += (str[i] - '0') * secm2;
            secm2 = secm2 / 10;
        }

    }
    seconds2 = (double)sec2 / 1000;
    return deg2 + (min2 / 60) + (seconds2 / 3600);

}


int main(){
		char lat[10]="";
		char lon[10]="";
		char inputstring[] = " " ;
		int i=0;
		char latw[]={'l','a','t','=',' '};
		char space=' ';
		char lonw[]={'l','o','n','=',' '};
		double lng,latt;
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
	}}
	else{
	for(i=0;i<10;i++){lat[i]=inputstring[i+21];	
	if(inputstring[i+22]==','){i=10;}
	}}
	
	if((i==0)&(inputstring[i+35]==0)){
	for(i=0;i<10;i++){lon[i]=inputstring[i+36];	
	if(inputstring[i+37]==','){i=10;}
	}}
	else{
	for(i=0;i<10;i++){lon[i]=inputstring[i+35];	
	if(inputstring[i+36]==','){i=10;}
	}}
	
  while (1){
		
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
	UART0_write('\n');
	
	delay(5000);
	
	}


}