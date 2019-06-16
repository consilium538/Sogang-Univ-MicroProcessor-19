#include <stm32f10x.h>
#include "LCD_lib.h"
#include "LTM022A69B.h"
#include "system.h"
#include "Touch.h"
#include "board_init.h"

// #define STM32F10X_MD_VL // for stm32f100rb, add to project option

#define PLAYERMOVE 6

#define LED_BLUE_H() GPIOC->BSRR = 0x1 << 8
#define LED_BLUE_L() GPIOC->BSRR = 0x1 << 24

#define LED_GREEN_H() GPIOC->BSRR = 0x1 << 9
#define LED_GREEN_L() GPIOC->BSRR = 0x1 << 25

const unsigned short IMG_circular2[] = {
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xd6ba,0xa514,0x9cd3,0xce79,0xffdf,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffdf,0x7bef,0x0841,0x0861,0x1082,0x1082,0x1082,0x0020,0x738e,0xffdf,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xdefb,0x2124,0x0841,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0020,0x18e3,0xd6ba,0xffff,0xffff,
    0xffff,0xffdf,0x2124,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x18c3,0xffdf,0xffff,
    0xffff,0x7bcf,0x0841,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0020,0x6b6d,0xffff,
    0xffff,0x0020,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0020,0xffff,
    0xd69a,0x0861,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0020,0xce79,
    0x9cd3,0x1082,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x94b2,
    0xa514,0x0020,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0020,0x8430,
    0xdedb,0x0841,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0861,0xbdf7,
    0xffff,0x0020,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xf79e,
    0xffff,0x73ae,0x0020,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0020,0x5acb,0xffff,
    0xffff,0xffdf,0x18e3,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x1082,0xf79e,0xffff,
    0xffff,0xffff,0xd6ba,0x18c3,0x0020,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0020,0x1082,0xce59,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffdf,0x6b6d,0x0020,0x0861,0x0020,0x0000,0x0861,0x0020,0x5acb,0xf79e,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xf7be,0xc618,0x8c71,0x8c71,0xc618,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff 
}; 

const unsigned short IMG_white_circular2[] = {
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x2945,0x5aeb,0x632c,0x3186,0x0020,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0020,0x8410,0xf7be,0xf79e,0xef7d,0xef7d,0xef7d,0xffdf,0x8c71,0x0020,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x2104,0xdedb,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffdf,0xe71c,0x2945,0x0000,0x0000,
    0x0000,0x0020,0xdedb,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xe73c,0x0020,0x0000,
    0x0000,0x8430,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffdf,0x9492,0x0000,
    0x0000,0xffdf,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffdf,0x0000,
    0x2965,0xf79e,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffdf,0x3186,
    0x632c,0xef7d,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x6b4d,
    0x5aeb,0xffdf,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffdf,0x7bcf,
    0x2124,0xf7be,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xf79e,0x4208,
    0x0000,0xffdf,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0861,
    0x0000,0x8c51,0xffdf,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffdf,0xa534,0x0000,
    0x0000,0x0020,0xe71c,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xef7d,0x0861,0x0000,
    0x0000,0x0000,0x2945,0xe73c,0xffdf,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffdf,0xef7d,0x31a6,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0020,0x9492,0xffdf,0xf79e,0xffdf,0xffff,0xf79e,0xffdf,0xa534,0x0861,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0841,0x39e7,0x738e,0x738e,0x39e7,0x0841,0x0000,0x0000,0x0000,0x0000,0x0000 
};

int sdata;
int i;
int j;
int x;

unsigned char read_res ;
unsigned char led_on;
unsigned short keyboard_data;
unsigned short old_keyboard_data;
unsigned long long int system_tick;
unsigned char tick_flag;
unsigned char touch_flag;

typedef struct {
    short x;
    short y;
} playerState;
playerState player, prevplayer;

typedef struct {
    short x;
    short y;
    short dx;
    short dy;
    short alive;
} bulletState;
bulletState bullet, prevbullet;

typedef struct {
    short x;
    short y;
    char alive;
} enumyState;
enumyState enumy[4];

unsigned short orangebox[16*16];
unsigned short blackbox[16*16];
unsigned short whitebox[16*16];

typedef struct {
	unsigned int  x;
	unsigned int  y;
	unsigned long x_ad_val; //ADC value
	unsigned long y_ad_val;						   	    
	unsigned char  pen_status;//The pen of the state
} _touch_dot;
extern _touch_dot touch_dot;

void display_touch_debug()
{
    //ADC results show
    lcd_display_string((const unsigned char*)"READ SUCCESS:",BLACK,GREEN,1,9);
    lcd_display_number(14,9,read_res,6);
    lcd_display_string((const unsigned char*)"X AD Val:",BLACK,GREEN,1,10);
    lcd_display_number(10,10,touch_dot.x_ad_val,6);
    lcd_display_string((const unsigned char*)"Y AD Val:",BLACK,GREEN,1,11);
    lcd_display_number(10,11,touch_dot.y_ad_val,6);
    
    //Display coordinates
    lcd_display_string((const unsigned char*)"X:",BLACK,GREEN,1,12);
    lcd_display_number(10,12,touch_dot.x,5);
    lcd_display_string((const unsigned char*)"Y:",BLACK,GREEN,1,13);
    lcd_display_number(10,13,touch_dot.y,5);
}

void keyUpdate()
{
    old_keyboard_data = keyboard_data;
    keyboard_data = 0;
    GPIOC->BSRR = 1<<4 | 1 << (7+16);
    keyboard_data |= (GPIOC->IDR & 0xF) << 0;
    GPIOC->BSRR = 1<<5 | 1 << (4+16);
    keyboard_data |= (GPIOC->IDR & 0xF) << 4;
    GPIOC->BSRR = 1<<6 | 1 << (5+16);
    keyboard_data |= (GPIOC->IDR & 0xF) << 8;
    GPIOC->BSRR = 1<<7 | 1 << (6+16);
    keyboard_data |= (GPIOC->IDR & 0xF) << 12;
    
//    if( ( keyboard_data & 1<<7 ) && !( old_keyboard_data & 1<<7 ) )
//        onKeyPressed(7);
//    else if( ( keyboard_data & 1<<6 ) && !( old_keyboard_data & 1<<6 ) )
//        onKeyPressed(6); // up
//    else if( ( keyboard_data & 1<<3 ) && !( old_keyboard_data & 1<<3 ) )
//        onKeyPressed(3); // left
//    else if( ( keyboard_data & 1<<2 ) && !( old_keyboard_data & 1<<2 ) )
//        onKeyPressed(2);
//    else if( ( keyboard_data & 1<<1 ) && !( old_keyboard_data & 1<<1 ) )
//        onKeyPressed(1);
}

void gameInit()
{
    player.x = 120;
    player.y = 90;
    
    for(i = 0; i < 4; i++)
    {
        enumy[i].x = 48 + 48*i;
        enumy[i].y = 204;
        enumy[i].alive = 1;
    }
//    enumy[0].x = 60;
//    enumy[0].y = 90;
//    enumy[0].alive = 1;
//    
//    enumy[1].x = 60;
//    enumy[1].y = 270;
//    enumy[1].alive = 1;
//    
//    enumy[2].x = 180;
//    enumy[2].y = 90;
//    enumy[2].alive = 1;
//    
//    enumy[3].x = 180;
//    enumy[3].y = 270;
//    enumy[3].alive = 1;
    
    bullet.alive = 0;
}

void gameUpdate()
{
    prevplayer.x = player.x;
    prevplayer.y = player.y;
    
    if( keyboard_data & 1<<6 ) // up
        player.x += PLAYERMOVE;
    if( keyboard_data & 1<<3 ) // left
        player.y -= PLAYERMOVE;
    if( keyboard_data & 1<<2 ) // down
        player.x -= PLAYERMOVE;
    if( keyboard_data & 1<<1 ) // right
        player.y += PLAYERMOVE;
    
    if(player.x > 232) player.x = 232;
    if(player.x < 8) player.x = 8;
    if(player.y > 312) player.y = 312;
    if(player.y < 8) player.y = 8;
    
    for( i =  0; i < 4; i++ )
    {
        if( bullet.alive && enumy[i].alive &&
            ( ( ( bullet.x - enumy[i].x ) * ( bullet.x - enumy[i].x )
            + ( bullet.y - enumy[i].y ) * ( bullet.y - enumy[i].y )) < 256 ) ) // hit
        {
            bullet.alive = 0;
            enumy[i].alive = 0;
        }
    }
    
    if( bullet.alive )
    {
        bullet.x += bullet.dx;
        bullet.y += bullet.dy;
        if( bullet.x > 224 | bullet.x < 0 ) bullet.alive = 0;
        if( bullet.y > 304 | bullet.y < 0) bullet.alive = 0;
    }
}

void onKeyPressed(const unsigned int key)
{
    // if( key == 7 ) // lcd_clear_screen(BLACK);
    if( key == 6 ) // up
        player.x += 16;
    if( key == 3 ) // left
        player.y -= 16;
    if( key == 2 ) // down
        player.x -= 16;
    if( key == 1 ) // right
        player.y += 16;
    
    if(player.x > 224) player.x = 224;
    if(player.x < 0) player.x = 0;
    if(player.y > 304) player.y = 304;
    if(player.y < 0) player.y = 0;
}

void screenRefresh()
{
    for(i = 0; i< 4; i++)
    {
        if(enumy[i].alive) lcd_display_image((const unsigned char*)whitebox, enumy[i].x-8, enumy[i].y-8, 16, 16);
        else lcd_display_image((const unsigned char*)blackbox, enumy[i].x-8, enumy[i].y-8, 16, 16);
    }
    
    lcd_display_image((const unsigned char*)blackbox, bullet.x-bullet.dx-8, bullet.y-bullet.dy-8, 16, 16);
    if( bullet.alive )
    {
        lcd_display_image((const unsigned char*)orangebox, bullet.x-8, bullet.y-8, 16, 16);
    }
    else lcd_display_image((const unsigned char*)blackbox, bullet.x-8, bullet.y-8, 16, 16);
    
     lcd_display_image((const unsigned char*)blackbox, prevplayer.x-8, prevplayer.y-8, 16, 16);
     lcd_display_image((const unsigned char*)IMG_white_circular2, player.x-8, player.y-8, 16, 16);
}

void bulletShot()
{
    lcd_display_image((const unsigned char*)blackbox, bullet.x, bullet.y, 16, 16);
    bullet.alive = 1;
    bullet.x = player.x;
    bullet.y = player.y;
    bullet.dx = ( touch_dot.x - player.x ) / 16;
    bullet.dy = ( touch_dot.y - player.y ) / 16;
}

//////////////////////// Start main() ///////////////////////

int main()
{
    board_init();
	
	LED_BLUE_H();
	lcd_init();
	LED_BLUE_L();
    
	system_tick = 0;
    tick_flag = 0;
    touch_flag = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE;
    
    for(i = 0; i < 256; i++) orangebox[i] = ORANGE;
    for(i = 0; i < 256; i++) blackbox[i] = BLACK;
    for(i = 0; i < 256; i++) whitebox[i] = WHITE;
    gameInit();
	
	while(1)
	{
        if(tick_flag)
        {
            if( !( system_tick % 10 ) )
            {
                if(led_on) LED_GREEN_H();
                else LED_GREEN_L();
                
                led_on = !led_on;
            }
            if(touch_flag)
            {
                read_res=Read_Continue();
                TOUCH_nCS_H();
                LCD_RS_L();
                LCD_CS_L();
//                display_touch_debug();
//                lcd_display_image((const unsigned char*)orangebox, touch_dot.x-8 , touch_dot.y-8, 16, 16);
                bulletShot();
                touch_flag = 0;
            }
            if( !( system_tick % 2 ) )
            {
                keyUpdate();
                
                gameUpdate();
                
                screenRefresh();
            }
//            lcd_display_number(2,2,(keyboard_data >> 0) & 0xF,2);
//            lcd_display_number(6,2,(keyboard_data >> 4) & 0xF,2);
//            lcd_display_number(10,2,(keyboard_data >> 8) & 0xF,2);
//            lcd_display_number(14,2,(keyboard_data >> 12) & 0xF,2);
            tick_flag = 0;
        }
	}
}

void EXTI9_5_IRQHandler(void) //PB9 Touch interrupt
{
    touch_flag = 1;
    EXTI->PR |= 1<<9;
}

void SysTick_Handler()
{
	system_tick++;
    tick_flag = 1;
}
