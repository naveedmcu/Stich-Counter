#include <18F46K22.h>
//#device ADC=10
#fuses HSH, NOWDT, NOPUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT 
#use delay( clock = 20000000 )
//#use rs232( BAUD=4800, RCV=PIN_C7, XMIT=PIN_C6, STREAM=RS232, PARITY=N, BITS=8 )
#zero_ram 
///////////////////////////////////////////////////////////////////////////////
#use fast_io(A)
#use fast_io(B)
#use fast_io(C)
#use fast_io(D)
#use fast_io(E)
///////////////////////////////////////////////////////////////////////////////
#byte PORTA = 0xF80
#byte PORTB = 0xF81
#byte PORTC = 0xF82
#byte PORTD = 0xF83
#byte PORTE = 0xF84
///////////////////////////////////////////////////////////////////////////////
//switch actions
#define SET   1
#define RESET  0
#define press 0
#define on  1
#define off  0
#define sw_debounce 20

#define data_pin  PIN_D2
#define clock_pin PIN_D1
#define latch_pin PIN_D0

#define rpm_show_in 1000
#define time_period_msec 100
#define revolutions_per_minute 60
#define pulses_get_time 10
////////////////////////////////////////////////////////////////////////////////
#bit Enable_SW      = PORTB.1 // Countr Enable switch
#bit SW_1           = PORTB.2 // Reset
#bit SW_2           = PORTB.3 // UP
#bit SW_3           = PORTB.4 // DOWN
#bit SW_4           = PORTB.5// RPM/Setch
/////////////////////////////////////////////////////////////////////////////
static struct system_variables
{
   int1 sec_inc;
   int1 min_inc;
   unsigned int16 m_second;
   unsigned int second;
   unsigned int minute;
   
   int1 initialized;
   unsigned int init_count;
   
   int1 enable_sw, sw_1, sw_2, sw_3, sw_4, sw_r;//setting switch flags
   unsigned int key_press;
   
}system_count;
static struct date_time_variables
{
   int8 times;
   unsigned int8  second, minute, hour, date, month, year, day;
} time;

unsigned int digit, digit1, digit10, digit100,digit1000,digit10000,digit100000;
unsigned int digita, digitb, digitc, digitd;
unsigned int32 Puls_count ;

const byte number[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0xFF, 0x6F};

unsigned int hours, minutes;

int i;
int1 rpm_flag = 0;
int1 time_call_flag = 0;
short s, ss;
unsigned int j, digit_rtc ;
//unsigned int32 rtc = 0;
int manu_count = 0;
//////////////////////////////////////////////////////////////////////////////
void led_display_number(unsigned int32 data);
void rpm_display( unsigned int32 i, short Counter_Enable  ) ;
void rpm_manu (void);
void send_byte(char data); 
///////////////////////////////////////////////////////////////////////////////
#include "stdio.h"
#include "stdlib.h"
#include "internal_eeprom.h"

#include "74595.c"
#include "DS1307.c"
#include "MUX.c"

#include "internal_eeprom.c"
#include "system_counter.c"
//////////////////////////////////////////////////////////////////////////////
#int_ext
void ext_isr(void) //call switch
{
   clear_interrupt(int_ext);
   Puls_count++;
   if(Puls_count > 999999){ Puls_count = 0; }      
}
///////////////////////////////////////////////////////////////////////////
//RegValue = 256-(Delay * Fosc)/(Prescalar*4)) = 256-((10ms * 20Mhz)/(256*4)) = 256-195=60
#INT_TIMER0
void timer0_isr(void)
{
  clear_interrupt(INT_TIMER0);    // Clear timer0 interrupt flag bit
  //set_timer0(200);
  Reset_display( Puls_count, system_count.sw_1 );
  //if(rpm_flag){rpm_display(Puls_count, system_count.sw_1);}
  //else{Reset_display( Puls_count, system_count.sw_1 );}
}
///////////////////////////////////////////////////////////////////////////////
#int_TIMER1
void TIMER1_isr( void )
{
   set_timer1( 55295 );
   system_count.m_second++;
   if( system_count.m_second == 488 )
   {
      system_count.m_second = 0;
      system_count.second++;
      system_count.sec_inc = 1;
      if( system_count.second >= 60 )
      {
         system_count.second = 0;
         system_count.minute++;
         //if(system_count.second == 5){ time_call_flag =1; }else{ delay_us(10);time_call_flag =0;}
      }
   }
   
      system_count.enable_sw = Enable_SW;
   
      system_count.sw_1 = SW_2; // Reset
      system_count.sw_2 = SW_1; //UP
      system_count.sw_3 = SW_3; //DOWN
      system_count.sw_4 = SW_4; //RPM
 time_call_flag =1;     
restart_wdt();   
}
///////////////////////////////////////////////////////////////////////////////
void main()  
{
   set_tris_a(0x00); 
   set_tris_b(0x3F); //0011 1111
   set_tris_c(0x00);
   set_tris_d(0x00);
   set_tris_e(0x00);
   init_ds1307();
   port_b_pullups(true);
   
   ext_int_edge(H_TO_L);
   clear_interrupt(INT_EXT);
   enable_interrupts(INT_EXT);
   
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_64|RTCC_8_bit);      //13.1 ms overflow
   set_timer0(0);                // Timer0 preload value
   clear_interrupt(INT_TIMER0);   // Clear timer0 interrupt flag bit
   enable_interrupts(INT_TIMER0); // Enable timer0 interrupt
   
   setup_timer_1( T1_INTERNAL );
   enable_interrupts( INT_TIMER1 );
   
   enable_interrupts(GLOBAL);
   //ds1307_get_time(time.year, time.month, time.date, time.day, time.hour, time.minute, time.second);
   time.hour=9; time.minute =56;
   //led_display_number(time.hour, time.minute);
///////////////////////////////////////////////////////////////////////////////  
   while(true)
   { 
      manu_function(); 
      //ds1307_get_time(time.year, time.month, time.date, time.day, time.hour, time.minute, time.second);
      
      led_display_number(time.hour, time.minute);
      //Reset_display( Puls_count, system_count.sw_1 );
   }    
   
}

