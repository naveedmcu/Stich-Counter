/* 
 * File:   HW_IO.h
 * Author: andoird
 *
 * Created on May 13, 2018, 12:01 PM
 */

#ifndef HW_IO_H
#define	HW_IO_H

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

#endif	/* HW_IO_H */

