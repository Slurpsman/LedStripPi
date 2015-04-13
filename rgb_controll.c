/*
  LED_Strip_Suli.cpp
  This is a Suly compatable Library
  
  2014 Copyright (c) Seeed Technology Inc.  All right reserved.
  
  Loovee
  2013-5-8
  Slurpsman
  2015-31-01

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
/*
	Change log: modified function to work with wiringpi
*/

#include "rgb_controll.h"
/*
IO_T *__pinClk;
IO_T *__pinDta;
*/
//uint8_t pinClk;
//uint8_t pinData;

void led_strip_init(uint8_t clk, uint8_t data)
{
	if(wiringPiSetup()==-1)
	{
	  printf("wiringPi couldn't load");
	}
	else
	{
	   pinMode (clk, OUTPUT);
	   pinMode (data, OUTPUT);
	   pinClk = clk;
//	   printf("Clock pin gesetzt\n");
	   pinData = data;
//	   printf("Daten pin gesetzt\n");
	pRed =0;
	pGreen = 0;
	pBlue = 0;
	delay(1);
	controll_led_strip();
	}
}

void led_strip_begin()
{
    led_strip_32_zero();
}

void led_strip_end()
{
    led_strip_32_zero();
}

void led_strip_32_zero()
{
	led_send_data(LOW);
}

uint8_t led_strip_take_anticode(uint8_t data)
{
    uint8_t tmp = 0;
    if(!(data & 0x80))
    {
        tmp |= 0x02; 
    }
    
    if(!(data & 0x40))
    {
        tmp |= 0x01;
    }
    
    return tmp;
}

void led_strip_set_colour(uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t dx = 0x00;
    dx |= (uint32_t)0x03 << 30;             // highest two bits 1, flag bits
    dx |= (uint32_t)led_strip_take_anticode(blue) << 28;
    dx |= (uint32_t)led_strip_take_anticode(green) << 26;	
    dx |= (uint32_t)led_strip_take_anticode(red) << 24;
 
    dx |= (uint32_t)blue << 16;
    dx |= (uint32_t)green << 8;
    dx |= red;
    led_send_data(dx);
}

void led_send_data(uint32_t ledStatus)
{
	// init Routine erstellen
//	uint8_t ClkPin, DataPin;
	uint8_t sendByte;
	for (uint8_t byte=0;byte< sizeof(ledStatus);byte++)
	{
		sendByte = (uint8_t)(ledStatus >> 8*(3-byte));
		shiftOut (pinData, pinClk, MSBFIRST, sendByte) ;
	}
}
void controll_led_strip(void)
{
  led_strip_begin();
  led_strip_set_colour(pRed,pGreen,pBlue);
  led_strip_end();
}
uint32_t colour_generator(uint8_t stepsize)
{
  uint32_t theColour=0;
  uint16_t tRed=0,tGreen=0, tBlue=0;
  uint8_t twait = 5;
  for(tRed=0;tRed<256;tRed+=stepsize)
  {
    pRed=tRed;
    controll_led_strip();
    delay(twait);
  }
  pRed=0;
  for(tGreen=0;tGreen<256;tGreen+=stepsize)
  {
    pGreen=tGreen;
    controll_led_strip();
    delay(twait);
  }
  pGreen=0;
  for(tBlue=0;tBlue<256;tBlue+=stepsize)
  {
    pBlue=tBlue;
    controll_led_strip();
    delay(twait);
  }
  pBlue=0;

}
/* main starts hear */

int main (int argc, char *argv [])
{
	//uint8_t ClkPin, DataPin;
	// Funktionen noch etwas besser kapseln
	
//	uint8_t m_red, m_green=0, m_blue =0;
//	m_red =255;
	led_strip_init(5,4);
//	delay(500);
	if (argc ==1)
	{
	delay(500);
	pRed=255;	
	controll_led_strip();
	delay(1000);
	pRed=0;
	pBlue=255;
	controll_led_strip();
	delay(1000);
	pBlue=0;
	pGreen=255;
	controll_led_strip();
	delay(1000);
	pGreen=0;
	controll_led_strip();
	return 0;
	}
	if(strcmp(argv[1],"demo")==0)
	{
	   colour_generator(1);
	   return 0;
	}

	if (strcmp( argv[1],"off")==0)
	{
	return 0;
	}
//	pRed= 127;
//	controll_led_strip();
//	led_strip_begin();
//	led_strip_set_color(m_red, m_green, m_blue);
//	led_strip_end();

//	printf("Programm ends\n");	
	
}
