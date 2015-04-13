#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringShift.h>

//static void led_strip_clk_rise();
static void led_strip_32_zero();
static uint8_t led_strip_take_anticode(uint8_t data);
//static void led_strip_dta_send(uint32 dta);

// user api
void led_send_data(uint32_t led_status);
void led_strip_begin();
void led_strip_end();
void led_strip_init(uint8_t clk, uint8_t data);
void led_strip_set_colour(uint8_t r, uint8_t g, uint8_t b);
void controll_led_strip(void);
uint32_t colour_generator(uint8_t stepsize);
uint8_t pinClk, pinData;
uint8_t pRed, pBlue, pGreen;
