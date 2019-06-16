#include <stm32f10x.h>

#include "system.h"
#include "LTM022A69B.h"
#include "LCD_lib.h"
#include "touch.h"

//#define TOUCH_nCS_H()      sbi(PD_ODR,7);
//#define TOUCH_nCS_L()      cbi(PD_ODR,7);

#define CMD_READ_X    0xD0
#define CMD_READ_Y    0x90

 typedef struct 
{
	unsigned int  x;
	unsigned int  y;
	unsigned long x_ad_val;
	unsigned long y_ad_val;						   	    
	unsigned char  pen_status;		  
}_touch_dot;
_touch_dot touch_dot,*p_touch_dot;

void TOUCH_init(void)
{

}

void TOUCH_WRITE_REG(unsigned int index)
{
	
	LCD_RS_L();
	TOUCH_nCS_L();

    SPI0_communication((unsigned char)(index>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(index));

	TOUCH_nCS_H();
	LCD_RS_H();

}

/******************************************************************************
* Function Name  : LCD_WRITE_COMMAND
* Description    : send command to LCD
* Input          : index, data
* Output         : None
* Return         : None
******************************************************************************/
void TOUCH_WRITE_COMMAND(unsigned int index,unsigned int data)
{
	//select command register

	LCD_RS_L();
	TOUCH_nCS_L();

    SPI0_communication((unsigned char)(index>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(index));
	LCD_CS_H();
	//send data
	LCD_RS_H();

	TOUCH_nCS_L();
    SPI0_communication((unsigned char)(data>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(data));
	TOUCH_nCS_H();

}

/*******************************************************************************
* Function Name  : LCD_WRITE_DATA
* Description    : write data to LCD
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TOUCH_WRITE_DATA(unsigned int data)
{
    SPI0_communication((unsigned char)(data>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(data));
}

unsigned int GetTouchADC (unsigned char data)
{	  
	unsigned int NUMH , NUML;
	unsigned int Num;
	LCD_CS_H()
	TOUCH_nCS_L();
		
	delay_us(2);
	SPI0_communication(data);
	delay_us(2);
	NUMH=SPI0_communication(0x00);
	NUML=SPI0_communication(0x00);
	Num=((NUMH)<<8)+NUML; 	
	Num>>=4;
	TOUCH_nCS_H();

	return(Num);   
}

#define READ_TIMES 10
#define LOST_VAL 4

unsigned int GetTouchADCEx(unsigned char cmd_code)
{
	unsigned int i, j;
	unsigned int buf[READ_TIMES];
	unsigned int sum=0;
	unsigned int temp;

	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=GetTouchADC(cmd_code);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
}

unsigned char Read_ADS(unsigned int *x_ad,unsigned int *y_ad)
{
	unsigned int xtemp,ytemp;			 	 		  
	xtemp=GetTouchADCEx(CMD_READ_X);
	ytemp=GetTouchADCEx(CMD_READ_Y);								   
	if(xtemp<100||ytemp<100)
        return 1;
	*x_ad=xtemp;
	*y_ad=ytemp;        
	return 0;
}

#define ERR_RANGE 10

unsigned char Read_ADS2(unsigned long *x_ad,unsigned long *y_ad) 
{
	unsigned int x1,y1;
 	unsigned int x2,y2;
 	unsigned char res; 

    res=Read_ADS(&x1,&y1);
    if(res==1)  return(1);
    res=Read_ADS(&x2,&y2);
    if(res==1)  return(1);
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))
        &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x_ad=(x1+x2)/2;
        *y_ad=(y1+y2)/2;
        return 0;
    }
    else return 1;
} 
  
void convert_ad_to_xy(void)
{
//	touch_dot.x=(240-(touch_dot.x_ad_val-100)/7.500);
//	touch_dot.y=(320-(touch_dot.y_ad_val-135)/5.705);
//    touch_dot.x = (((touch_dot.x_ad_val * 240)>>12)-110)*2;
//    touch_dot.y = (((touch_dot.y_ad_val * 320)>>12)-150)*2;

//X=(240 * AD - 2100* 240) / 1900
//Y=(320 * AD - 2100* 320) / 1900
//    touch_dot.x=(240*touch_dot.x_ad_val -100*240)/ 1900;
    touch_dot.x = ( touch_dot.x_ad_val >> 3 ) - 6;
//    touch_dot.y=(320*touch_dot.y_ad_val -100*320)/ 1900;
    touch_dot.y = ( 3 * touch_dot.y_ad_val - 270 ) / 17;
}

unsigned char Read_Once(void)
{
	if(Read_ADS2(&touch_dot.x_ad_val,&touch_dot.y_ad_val)==0)	
	{
		convert_ad_to_xy();  
		return 0;
	}
	else return 1;
}

unsigned char Read_Continue(void)
{
	if(Read_ADS2( &touch_dot.x_ad_val,&touch_dot.y_ad_val )==0)
	{
		convert_ad_to_xy(); 
		return 0;	
	}
	else return 1;	
}










