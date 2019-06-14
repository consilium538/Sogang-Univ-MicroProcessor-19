/********************************************************************************************************
*
* File                : system.h
* Hardware Environment: 
* Build Environment   : ST Visual Develop 4.1.6
* Version             : V1.0
* By                  : Xiao xian hui
*
*                                  (c) Copyright 2005-2010, WaveShare
*                                       http://www.waveShare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <stm32f10x.h>

void delay_ms(unsigned short ms);
void delay_us(unsigned short us);

void timer_setup(void);

#endif /*_SYSTEM_H*/
