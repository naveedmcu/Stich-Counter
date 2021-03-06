#include <18F46K22.h>
#include "HW_IO.h"
//#device ADC=10
#fuses NOWDT, PUT 
#fuses INTRC          //Internal RC Osc
#fuses PLLEN          //HW PLL enabled
#fuses NOIESO         //Internal External Switch Over mode disabled
#fuses NOMCLR         //Master Clear pin used for I/O
#fuses NOFCMEN        //Fail-safe clock monitor disabled
#use delay( internal = 16MHZ )
//#use rs232( BAUD=4800, RCV=PIN_C7, XMIT=PIN_C6, STREAM=RS232, PARITY=N, BITS=8 )
#zero_ram 
///////////////////////////////////////////////////////////////////////////////
#use fast_io(A)
#use fast_io(B)
#use fast_io(C)
#use fast_io(D)
#use fast_io(E)
/////////////////////////////////////////////////////////////////////////////

#define StichDisplay 1
#define RPMDisplay 2

struct system_variables {
    int1 sec_inc;
    int1 min_inc;
    unsigned int16 m_second;
    unsigned int second;
    unsigned int minute;

    int1 initialized;
    unsigned int init_count;

    int1 enable_sw, sw_1, sw_2, sw_3, sw_4, sw_r; //setting switch flags
    unsigned int key_press;

} system_count;

struct date_time_variables {
    int8 times;
    unsigned int8 second, minute, hour, date, month, year, day;
} time;

unsigned int digit, digit1, digit10, digit100, digit1000, digit10000, digit100000;
unsigned int digita, digitb, digitc, digitd;
unsigned int32 Puls_count;

const byte number[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0xFF, 0x6F};

unsigned int hours, minutes;

int i;
int1 rpm_flag = 0;
int1 time_call_flag = 0;
short s, ss;
unsigned int j, digit_rtc;
//unsigned int32 rtc = 0;
int manu_count = 0;
int8 DigitNum = 0;
int8 DisplayLevel = 1;
int8 DigitNum1 = 0;
int32 DataDigit;
int32 SaveDataDigit;
int16 RealRPM, CalculatedRPM;
int8 hh;
int8 mm;
int8 digits[4];
int8 LSB0;
int8 LSB1;
int8 LSB2;
int8 LSB3;
int8 MSB0;
int8 MSB1;
int8 MSB2;
int8 MSB3;
int8 OneSecCounter = 0;
int8 TwoSecCounter = 0;

//////////////////////////////////////////////////////////////////////////////
void led_display_number(unsigned int32 data);
void rpm_display(unsigned int32 i, short Counter_Enable);
void rpm_manu(void);
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
////////////////////////////////////////////////////////////////////////////////

void main() {
    setup_oscillator(OSC_16MHZ | OSC_INTRC);
    set_tris_a(0x00);
    set_tris_b(0x3F); //0011 1111
    set_tris_c(0x00);
    set_tris_d(0x00);
    set_tris_e(0x00);
    SETUP_ADC_PORTS(NO_ANALOGS);
    init_ds1307();
    delay_ms(100);
    clear_interrupt(INT_EXT);
    ext_int_edge(L_TO_H);
    clear_interrupt(INT_EXT);
    enable_interrupts(INT_EXT);

    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_16 | RTCC_8_bit); //13.1 ms overflow
    set_timer0(6); // Timer0 preload value
    clear_interrupt(INT_TIMER0); // Clear timer0 interrupt flag bit
    enable_interrupts(INT_TIMER0); // Enable timer0 interrupt

    setup_timer_1(T1_INTERNAL);
    enable_interrupts(INT_TIMER1);

    set_timer5(15536);
    setup_timer_5(T5_INTERNAL | T5_DIV_BY_8);
    enable_interrupts(INT_TIMER5);

    setup_timer_2(T2_DIV_BY_16, 250, 10);
    enable_interrupts(INT_TIMER1);

    enable_interrupts(GLOBAL);
    //ds1307_set_date_time(7,8,18,7,12,50,0);
    time.hour = 9;
    time.minute = 56;
    //led_display_number(time.hour, time.minute);
    ////////////////////////////////////////////////////////////////////////////
    DataDigit = Puls_count;
    hh = 53;
    mm = 68;

    //    DataDigit = 12345;
    //    LSB0 = make8(DataDigit, 0);
    //    LSB1 = make8(DataDigit, 1);
    //    LSB2 = make8(DataDigit, 2);
    //    LSB3 = make8(DataDigit, 3);
    //    
    //    ds1307_write_nvram_byte(0, LSB0);
    //    ds1307_write_nvram_byte(1, LSB1);
    //    ds1307_write_nvram_byte(2, LSB2);
    //    ds1307_write_nvram_byte(3, LSB3);


    MSB0 = ds1307_read_nvram_byte(10);
    MSB1 = ds1307_read_nvram_byte(11);
    MSB2 = ds1307_read_nvram_byte(12);
    MSB3 = ds1307_read_nvram_byte(13);
    Puls_count = make32(MSB3, MSB2, MSB1, MSB0);


    while (true) {
        //DataDigit ++;
        if (system_count.sw_2 == 0) {
            system_count.sw_2 = 0;
            DisplayLevel++;
            if (DisplayLevel > 2) {
                DisplayLevel = StichDisplay;
            }
        }
        switch (DisplayLevel) {
            case StichDisplay:
            {
                DataDigit = Puls_count;
                break;
            }
            case RPMDisplay:
            {
                DataDigit = RealRPM;
                break;
            }
        }
        SaveDataDigit = Puls_count;
        LSB0 = make8(SaveDataDigit, 0);
        LSB1 = make8(SaveDataDigit, 1);
        LSB2 = make8(SaveDataDigit, 2);
        LSB3 = make8(SaveDataDigit, 3);

        delay_ms(100);
        hh = time.hour;
        mm = time.minute;
        //manu_function(); 
        //        data_display(123,1);
        //        delay_ms(1000);
        //        data_display(111,1);
        //        delay_ms(1000);
        //        digit = i % 10;
        //        digit1 = display(digit);
        //        output_a(0x00); // Turn off all displays 0000 1111
        //        output_c(digit1); // Send ones digit
        //        output_a(0x20); // Turn on display for ones  00 01 1111 0x07 ==1f//00 10 0000
        //        delay_ms(10);
        //
        //        output_a(0x00); // Turn on display for ones  00 01 1111 0x07 ==1f//00 10 0000
        //        delay_ms(10);
        if (system_count.sw_1 == 0) {
            system_count.sw_1 = 0;
            Puls_count = 0;
            ds1307_set_date_time(7, 8, 18, 7, 0, 0, 0);
            i2c_write(0xD0); // Connect to DS1307 by sending its ID on I2c Bus
            i2c_write(0X07); // Select the Ds1307 ControlRegister to configure Ds1307 address
            i2c_write(0X10); // Write 0x10 to Control register to enable SQW-Out value
        }
        ds1307_get_time(time.year, time.month, time.date, time.day, time.hour, time.minute, time.second);
        ds1307_write_nvram_byte(10, LSB0);
        ds1307_write_nvram_byte(11, LSB1);
        ds1307_write_nvram_byte(12, LSB2);
        ds1307_write_nvram_byte(13, LSB3);

        //led_display_number(time.hour, time.minute);
        //Reset_display( Puls_count, system_count.sw_1 );
    }
}
/**/
#INT_TIMER5

void timer5_isr(void) {
    if (OneSecCounter == 10) {
        RealRPM = CalculatedRPM;
        CalculatedRPM = 0;
        OneSecCounter = 0;
    }
    if (TwoSecCounter == 20) {
        CalculatedRPM = 0;
        TwoSecCounter = 0;
    }
    set_timer5(15536); // Timer0 preload value
    clear_interrupt(INT_TIMER5); // Clear timer0 interrupt flag bit
}

////////////////////////////////////////////////////////////////////////////////
//RegValue = 256-(Delay * Fosc)/(Prescalar*4)) = 256-((10ms * 20Mhz)/(256*4)) = 256-195=60
#INT_TIMER0

void timer0_isr(void) {

    data_display(DataDigit, 1);
    led_display_number(mm, hh);
    set_timer0(6); // Timer0 preload value
    clear_interrupt(INT_TIMER0); // Clear timer0 interrupt flag bit

    //set_timer0(200);
    //Reset_display(Puls_count, system_count.sw_1);
    //if(rpm_flag){rpm_display(Puls_count, system_count.sw_1);}
    //else{Reset_display( Puls_count, system_count.sw_1 );}
}
////////////////////////////////////////////////////////////////////////////////
#int_TIMER1

void TIMER1_isr(void) {
    output_toggle(PIN_E2);
    set_timer1(55295);
    system_count.m_second++;
    if (system_count.m_second == 488) {
        system_count.m_second = 0;
        system_count.second++;
        system_count.sec_inc = 1;
        if (system_count.second >= 60) {
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
    time_call_flag = 1;
    restart_wdt();
}
/**/
#int_ext

void ext_isr(void) //call switch
{
    clear_interrupt(int_ext);
    Puls_count++;
    if (Puls_count > 999999) {
        Puls_count = 0;
    }
    TwoSecCounter = 0;
    CalculatedRPM++;
}
