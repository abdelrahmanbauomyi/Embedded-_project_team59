#include "stdint.h"
#include "C:\Keil_v5\Labware\inc\tm4c123gh6pm.h"
#define Red 0x02
#define blue 0x04
#define green 0x08
void init(void)
{
	SYSCTL_RCGCGPIO_R |=0x20;
	while(!(SYSCTL_PRGPIO_R&0x20));
	GPIO_PORTF_LOCK_R=0x4C4F434B;
	GPIO_PORTF_CR_R =0x1F;
	GPIO_PORTF_AFSEL_R=0;
	GPIO_PORTF_PCTL_R=0;
	GPIO_PORTF_AMSEL_R=0;
	GPIO_PORTF_DIR_R=0x0E;
	GPIO_PORTF_DEN_R=0x1F;
	GPIO_PORTF_PUR_R=0x11;

}
int main()
{
	init();
	while(1)

	{
		if( (GPIO_PORTF_DATA_R&0x11) == 0x10)
		{
			GPIO_PORTF_DATA_R=Red;
		}
		else if((GPIO_PORTF_DATA_R&0x11)==0x01){
			GPIO_PORTF_DATA_R=blue;
		}
			
		else if((GPIO_PORTF_DATA_R&0x11)==0x00)
		{
			GPIO_PORTF_DATA_R=green;
		}
		else
			GPIO_PORTF_DATA_R=0;
		
	}
}