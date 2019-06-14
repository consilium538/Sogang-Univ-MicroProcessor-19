#include "system.h"

void delay_ms(unsigned short ms)
{
			u8 i;
			while(ms!=0)
			{
				for(i=0;i<250;i++)
				{}
				for(i=0;i<75;i++)
				{}
				ms--;
			}
	
//	TIM2->PSC = 12; // one count for 1us
//	TIM2->ARR = ms;
//	TIM2->CR1 |= 1;
//	while((TIM2->CR1 & 0x01)) {  }
//	return;
			
}

void delay_us(unsigned short us)
{
			u8 i;
			while(us!=0)
			{
				for(i=0;i<25;i++)
				{}
				for(i=0;i<7;i++)
				{}
				us--;
			}
	
//	TIM2->PSC = 12000; // one count for 1ms
//	TIM2->ARR = us;
//	TIM2->CR1 |= 1;
//	while((TIM2->CR1 & 0x01)) {  }
//	return;
			
}


void timer_setup(void)
{
	RCC->APB1ENR |= 1 << 0; // TIM2 enable
	TIM2->CR1 = 0x08;
	TIM2->CR2 = 0x00;
}
