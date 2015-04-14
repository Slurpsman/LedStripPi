/*$off*/
/*$4 *******************************************************************************************************************
 *
 * DESCRIPTION:  getopt example 
 *
 * AUTHOR:       andreas.b@posteo.de
 *
 * FILENAME:     getopt.c
 *
 * REMARKS:      
 *
 * HISTORY:      001a, 13 04 2015, Andreas slightly modified 
 *
 * COPYRIGHT:    (C) 2015 
 *
 *********************************************************************************************************************** */
/*$on*/

/*$3 ===================================================================================================================
  $C                                             Included headers
  ======================================================================================================================= */
#define _POSIX_C_SOURCE 200112L

#include "rgb_controll.h"
/*$3 ===================================================================================================================
  $C                                                 Constants
  ======================================================================================================================= */
#define PACKAGE "LED_Strip_Controll"
#define VERSION "0.0.1"
/*$3 ===================================================================================================================
 *     $C                                                  Macros
 *      ======================================================================================================================= */

/*$3 ===================================================================================================================
 *     $C                                                   Types
 *      ======================================================================================================================= */

/*$3 ===================================================================================================================
 *     $C                                               Imported data
 *      ======================================================================================================================= */

/*$3 ===================================================================================================================
 *     $C                                               Private data
 *      ======================================================================================================================= */

/*$3 ===================================================================================================================
 *     $C                                                Public data
 *      ======================================================================================================================= */

/*$3 ===================================================================================================================
 *     $C                                      Imported function declarations
 *      ======================================================================================================================= */

/*$3 ===================================================================================================================
 *     $C                                       Public function declarations
 *      ======================================================================================================================= */

/*$3 ===================================================================================================================
 *     $C                                       Private function declarations
 *      ======================================================================================================================= */

/*$3 ===================================================================================================================
 *     $C                                       Private function definitions
 *      ======================================================================================================================= */

/*$3 ===================================================================================================================
 *     $C                                        Public function definitions
 *      ======================================================================================================================= */
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
/*
 *
 */
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
/*
 *
 */
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
/*
 *
 */
void controll_led_strip(void)
{
    led_strip_begin();
    led_strip_set_colour(pRed,pGreen,pBlue);
    led_strip_end();
}
/*
 *
 */
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
    return 0;
}
/* main starts hear */

int main (int argc, char *argv [])
{
    //uint8_t ClkPin, DataPin;
    // Funktionen noch etwas besser kapseln
    //	uint8_t m_red, m_green=0, m_blue =0;
    //	m_red =255;
    /*~~~~ */
    int opt;
    /*~~~~ */

    led_strip_init(5,4);
    /* no arguments given */
    if (argc == 1) {
        fprintf(stderr, "This program needs arguments....\n\n");
        print_help(1);
    }
    while ((opt = getopt(argc, argv, "c:hv")) != -1) 
    {
        switch (opt) 
        {
            case 'h':
                print_help(0);
                break;
            case 'v':
                printf("%s %s\n\n", PACKAGE, VERSION);
                exit(0);
                break;
            case 'c':
                if (strcmp(optarg,"demo")==0)
                    colour_generator(1);
                else{
                }
            case ':':
                fprintf(stderr,
                        "%s: Error - Option `%c' needs a value\n\n",
                        PACKAGE, optopt);
                print_help(1);
                break;
            case '?':
                if (optopt =='c')
                {
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                }
                else if (isprint(optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
               // return 1;
            default:
                abort ();
                fprintf(stderr,
                        "%s: Error - No such option: `%c'\n\n",
                        PACKAGE, optopt);
                print_help(1);
        }
    }

    /* print all remaining options */
    for (; optind < argc; optind++) {
        printf("argument: %s\n", argv[optind]);
        printf("Don't know what to do with this\n");
        printf("USAGE:\n");
        print_help(1);
    }

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

}
/*
 *
 */
void print_help(int exval)
{               /* < @@@ TODO: add comment */
    printf("%s,%s send SIGINT to <pid>\n", PACKAGE, VERSION);
    printf("%s [-h] [-p <pid>]\n\n", PACKAGE);
    printf("  -p              process id to send Signal to\n");
    exit(exval);
}
