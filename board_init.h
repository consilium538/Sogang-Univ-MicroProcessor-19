#include <stm32f10x.h>

void board_init()
{
    // RCC->APB2ENR = 0x1D;
    RCC->APB2ENR = RCC_APB2RSTR_AFIORST
        | RCC_APB2RSTR_IOPARST
        | RCC_APB2RSTR_IOPBRST
        | RCC_APB2RSTR_IOPCRST;
    // 0001 1101 : GPIOA EN ,GPIOB EN, GPIOC EN, AFIO EN
    
    // AFIO->EXTICR[2] = 0x0010;
    AFIO->EXTICR[2] = AFIO_EXTICR2_EXTI5_PB;
    //PB9
    
    GPIOA->CRL=0xB4B44444;  //PA 5~7
    GPIOC->CRH = 0x00000033; //PC8,9
    GPIOC->CRL = 0x33338888; //PC0-3 key input, PC4-7 key output
    GPIOC->ODR &= ~(0xF);
    
    SysTick->LOAD = 300000; // for 100ms;
    SysTick->CTRL = SysTick_CTRL_TICKINT;
//    | SysTick_CTRL_CLKSOURCE;
	
//    timer_setup(); //TIM2 for delay configure
	
	EXTI->IMR |= EXTI_IMR_MR9;
    EXTI->FTSR |= EXTI_FTSR_TR9;
	NVIC->ISER[0] |= NVIC_ISER_SETENA_23;
}
