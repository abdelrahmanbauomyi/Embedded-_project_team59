#include "stdint.h"
#include "math.h"
#include "tm4c123gh6pm.h"
#define wake_up 0x30
#define eight_bits_data 0x38
#define clear 0x01
#define display_on 0x0F
#define increment_cursor 0x06


void init(void){
SYSCTL_RCGCUART_R |= 0x00000002;	//Activate UART1
SYSCTL_RCGCGPIO_R |= 0x27;	 	//Activate port A,B,C,F
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
//init port c
GPIO_PORTC_DEN_R |= 0x30;
GPIO_PORTC_AFSEL_R |= 0x30;
GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000;
GPIO_PORTC_AMSEL_R &= ~0x30;
//init port F
GPIO_PORTF_LOCK_R=0x4C4F434B;
GPIO_PORTF_CR_R =0x1F;
GPIO_PORTF_AFSEL_R=0;
GPIO_PORTF_PCTL_R=0;
GPIO_PORTF_AMSEL_R=0;
GPIO_PORTF_DIR_R=0x0E;
GPIO_PORTF_DEN_R=0x1F;
GPIO_PORTF_PUR_R=0x11;
//init uart
UART1_CTL_R &= ~ 0x00000001;
UART1_IBRD_R = 104;       //INT(16,000,000/(169600))
UART1_FBRD_R = 11;        //ROUND (0.166666764)
UART1_LCRH_R &=  0x00000070;
UART1_CTL_R |=  0x00000001;

}


// check if data available to be recieved 
uint8_t available(){
    return ((UART0_FR_R & UART_FR_RXFE)== UART_FR_RXFE)?0:1;
}



char UART_read(void){
	while(!available()){}
	 return (UART1_DR_R &0xFF);
 }
 
 void UART_write(uint8_t data){
 while ((UART1_FR_R & UART_FR_TXFF) !=0);
	 UART1_DR_R = data;
 }

void delay(int x) //delay for x milliseconds.
{
int i,j;
for( i =0 ; i<x ; i++){for( j=0 ; j<3180 ; j++){}}
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


//       putting the wanted data in an array 
//       $GPGLL,lat,dir,long,dir,unwanted data
void gps(char *str){
char c;
int i=0;
    c = UART_read();
    if(c == '$'){
        c = UART_read();
        if (c == 'G'){
            c = UART_read();
            if (c == 'P'){
                c = UART_read();
                if (c == 'G'){
                    c = UART_read();
                    if (c == 'L'){
                        c = UART_read();
                        if (c == 'L'){
                            c = UART_read();
                            if (c == ','){
                                while (1){
                                c = UART_read();
                                if (c == 'E' || c == 'W'){break;}
                                str[i]=c;
                                i++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


double getLongitude(char *str){
    int i;
    double deg=0, degm=10,min=0 ,minm=10,sec=0,secm=10000,seconds;
    for ( i=0; i<10;i++){
     if (i < 2){
      deg += (str[i] - '0') * degm;
       degm=degm/10;
     }
     else if(i>=2 && i<4) {
         min += (str[i] - '0') * minm;
         minm = minm/10;
     }
     else if (i>=5 && i<10){
         sec += (str[i] - '0') * secm;
         secm = secm/10;
     }
    }
    seconds = (double)sec /1000;
    return  deg + (min/60) + (seconds/3600);

}

double getlatitude(char *str){
    int i
    double deg2=0,degm2=10,min2=0,minm2=10,sec2=0,secm2=10000,seconds2;
    for ( i=12; i<22;i++){
      if (i>=12 && i < 14){
      deg2 += (str[i] - '0') * degm2;
       degm2=degm2/10;
     }
     else if(i>=14 && i<16) {
         min2 += (str[i] - '0') * minm2;
         minm2 = minm2/10;
     }
     else if (i>16 && i<22){
         sec2 += (str[i] - '0') * secm2;
         secm2 = secm2/10;
     }
     
  }
    double seconds2 = (double)sec2 /1000;
    return deg2 + (min2/60) + (seconds2/3600);

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
    if (distance >= 100){
    GPIO_PORTF_DATA_R |= 0X02;
    }
		else GPIO_PORTF_DATA_R=0x00;
}
int main()
{
	int distance = 85; // for testing
	init();
	UART_init();
	while(1){
	lcd_display_distance(distance);
	checkDistance(distance);
	distance++;
	}
    /*
    code to extract longtitude and attitude from the uart array
    char str[] = {'3','0','0','6','.','3','4','2','3','2','N','0','3','1','1','8','.','5','4','2','9','6',','};
  double deg=0, degm=10,min=0 ,minm=10,sec=0,secm=10000;
  double deg2=0,degm2=10,min2=0,minm2=10,sec2=0,secm2=10000;
  for (int i=0; i<23;i++){
     if (i < 2){
      deg += (str[i] - '0') * degm;
       degm=degm/10;
     }
     else if(i>=2 && i<4) {
         min += (str[i] - '0') * minm;
         minm = minm/10;
     }
     else if (i>=5 && i<10){
         sec += (str[i] - '0') * secm;
         secm = secm/10;
     }
     else if (i>=12 && i < 14){
      deg2 += (str[i] - '0') * degm2;
       degm2=degm2/10;
     }
     else if(i>=14 && i<16) {
         min2 += (str[i] - '0') * minm2;
         minm2 = minm2/10;
     }
     else if (i>16 && i<22){
         sec2 += (str[i] - '0') * secm2;
         secm2 = secm2/10;
     }
     
  }
  double seconds = (double)sec /1000;
  double seconds2 = (double)sec2 /1000;
  
  double lon = deg + (min/60) + (seconds/3600);
  double lat = deg2 + (min2/60) + (seconds2/3600);
    */
	}

