unsigned int display(unsigned int num)
{
   unsigned int sig[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0xFF, 0x6F};

   unsigned int pattern = 0;
   pattern = sig[num];
   
   return(pattern);
}
/////////////////////////////////////////////////////////////////////////////////
void data_display( unsigned int32 i, short Counter_Enable  ) 
{
   if ( !Counter_Enable )
   {
      digit = i % 10;
      digit1 = display(digit);
      output_a(0x00);             // Turn off all displays 0000 1111
      output_c(digit1);           // Send ones digit
      output_a(0x20);             // Turn on display for ones  00 01 1111 0x07 ==1f//00 10 0000
      delay_ms(5);
      
      digit = (i / 10) % 10;
      digit10 = display(digit);
      output_a(0x00);             // Turn off all displays
      output_c(digit10);          // Send tens digit
      if ( digit == 0 && digita == 0 && digitb == 0 && digitc == 0 && digitd == 0  ){ output_a(0x00);}
      else { output_a(0x10); }    // Turn on display for tens 00 00 1011 00 101111 0x0b ==2f //00 01 0000
      delay_ms(5);
       
      digit = (i / 100) % 10;
      digita = digit;
      digit100 = display(digit);
      output_a(0x00);             // Turn off all displays
      output_c(digit100);         // Send hundreds digit
      if ( digit == 0 && digitb == 0 && digitc == 0 && digitd == 0  ){ output_a(0x00);}
      else{ output_a(0x08);  }    // Turn on display for hundreds 0000 1101 , 00 11 0111 0xd ==37 //00 00 1000
      delay_ms(5);
       
      digit = (i / 1000) % 10;
      digitb = digit;
      digit1000 = display(digit);
      output_a(0x00);             // Turn off all displays
      output_c(digit1000);        // Send thousands digit
      if ( digit == 0 && digitc == 0 && digitd == 0  ){ output_a(0x00);}
      else{ output_a(0x04);  }    // Turn on display for thousands 0000 1110 00 11 1011 0xe ==3B //00 00 0100
      delay_ms(5);
   
      digit = (i / 10000) % 10;
      digitc = digit;
      digit10000 = display(digit);
      output_a(0x00);              // Turn off all displays
      output_c(digit10000);        // Send thousands digit
      if ( digit == 0 && digitd == 0  ){ output_a(0x00);}
      else{ output_a(0x02); }      // Turn on display for thousands 00 11 1101 3D //00 00 0010
      delay_ms(5);
       
      digit = (i / 100000) % 10;
      digitd = digit;
      digit100000 = display(digit);
      output_a(0x00);               // Turn off all displays
      output_c(digit100000);        // Send thousands digit
      if ( digit == 0 ){  output_a(0x00);}
      else{ output_a(0x01); }      // Turn on display for thousands 00 11 1110 //00 00 0001
      delay_ms(5);
   }
   else { output_a(0x00); }
}
///////////////////////////////////////////////////////////////////////////
void Reset_display( unsigned int32 i, short reset_sw )
{
   if( reset_sw == 0 ){ Puls_count = 0; }
   else if(rpm_flag){rpm_display(i, 0);}
   else{ data_display(i,0); }
}
/////////////////////////////////////////////////////////////////////////////
void manu_function(void)
{

   rpm_manu ();
   
  int rpm_delay = 0;
if(manu_count == 1)
{
   while(rpm_delay < 10)
   {
     rpm_flag = 1;
     rpm_delay++;
   }
   if(system_count.sw_4 == 0){  rpm_delay = 0; }
   else if(rpm_delay == 5){manu_count = 0;}
}
else if(manu_count == 2)
{
rpm_flag = 0;
   while(rpm_delay < 10)
   {
      if(system_count.sw_2 && system_count.sw_3)
        s = 1;
        if(s == 1) {
        if(system_count.sw_2 == 0) {
          s = 0;
          minutes ++;
            if(minutes > 59){
               minutes = 0;
            }
         }
         if(system_count.sw_3 == 0) {
            s = 0;
            if(minutes < 1)
               minutes = 1;
               minutes--;
           }
       }
       rpm_delay++;
       ds1307_write(1, minutes);
   } 
   if(system_count.sw_4 == 0){  rpm_delay = 0; }
   else if(rpm_delay == 5){manu_count = 0;}
}
else if(manu_count == 3)
{
   while(rpm_delay < 10)
   {
      if(system_count.sw_2 && system_count.sw_3)
        s = 1;
        if(s == 1) {
        if(system_count.sw_2 == 0) {
          s = 0;
          hours ++;
            if(hours > 12){
               hours = 1;
            }
         }
         if(system_count.sw_3 == 0) {
            s = 0;
            if(hours < 1)
               hours = 1;
               hours--;
           }
        }
       rpm_delay++;
       ds1307_write(1, hours);
   } 
   if(system_count.sw_4 == 0){ rpm_delay = 0; }
   else if(rpm_delay == 5){manu_count = 0;}
}
else
{
  
}     
}
/////////////////////////////////////////////////////////////////////////////
void rpm_manu (void)
{

   if(system_count.sw_4)
      ss = 1;
      if(ss == 1) {
         if(system_count.sw_4 == 0) {
            ss = 0;
            manu_count ++;
            if(manu_count >= 4){
               manu_count = 0;
            }
         }
      }
 //delay_ms(10);
}
/////////////////////////////////////////////////////////////////////////////
void write_data(unsigned int number){
  for(j = 0x80; j > 0; j = j >> 1) {
     if(number & j)
       output_high(data_pin);
     else
       output_low(data_pin);
       output_high(clock_pin);
       output_low(clock_pin);
   }
       output_high(latch_pin);
       output_low(latch_pin);
}
/////////////////////////////////////////////////////////////////////////////
void led_display_number(unsigned int j, unsigned int i)
{

    int8 digits[4];
    
    digits[0] = (i % 10);                 // Prepare to display ones
    output_low(PIN_D3);output_low(PIN_D4);output_low(PIN_D5);output_low(PIN_D6); // Turn off all displays
    write_expanded_outputs(&digits[0]);
    output_low(PIN_D3);output_low(PIN_D4);output_low(PIN_D5);output_high(PIN_D6); // Turn on display for ones
    delay_ms(1);
    digits[1] = (i / 10) % 10;          // Prepare to display tens
    output_low(PIN_D3);output_low(PIN_D4);output_low(PIN_D5);output_low(PIN_D6); // Turn off all displays
    write_expanded_outputs(&digits[1]);
    output_low(PIN_D3);output_low(PIN_D4);output_high(PIN_D5);output_low(PIN_D6); // Turn on display for tens
    delay_ms(1);
    
    digits[2] = (j % 10);        // Prepare to display hundreds
    output_low(PIN_D3);output_low(PIN_D4);output_low(PIN_D5);output_low(PIN_D6);  // Turn off all displays
    write_expanded_outputs(&digits[2]);
    output_low(PIN_D3);output_high(PIN_D4);output_low(PIN_D5);output_low(PIN_D6);  // Turn on display for hundreds
    delay_ms(1);
    digits[3] = (j / 10) % 10;        // Prepare to display thousands
    output_low(PIN_D3);output_low(PIN_D4);output_low(PIN_D5);output_low(PIN_D6);   // Turn off all displays
    write_expanded_outputs(&digits[3]);
    output_high(PIN_D3);output_low(PIN_D4);output_low(PIN_D5);output_low(PIN_D6);   // Turn on display for thousands
    delay_ms(1);  
}
/////////////////////////////////////////////////////////////////////////
void rpm_display( unsigned int32 i, short Counter_Enable  ) 
{
unsigned int32 rpm;
rpm = ((i * rpm_show_in) / time_period_msec) * 1000 / pulses_get_time * revolutions_per_minute;
rpm = rpm / 1000;
   if ( !Counter_Enable )
   {
      digit = rpm % 10;
      digit1 = display(digit);
      output_a(0x00);             // Turn off all displays 0000 1111
      output_c(digit1);           // Send ones digit
      output_a(0x20);             // Turn on display for ones  00 01 1111 0x07 ==1f//00 10 0000
      delay_ms(5);
      
      digit = (rpm / 10) % 10;
      digit10 = display(digit);
      output_a(0x00);             // Turn off all displays
      output_c(digit10);          // Send tens digit
      if ( digit == 0 && digita == 0 && digitb == 0 && digitc == 0 && digitd == 0  ){ output_a(0x00);}
      else { output_a(0x10); }    // Turn on display for tens 00 00 1011 00 101111 0x0b ==2f //00 01 0000
      delay_ms(5);
       
      digit = (rpm / 100) % 10;
      digita = digit;
      digit100 = display(digit);
      output_a(0x00);             // Turn off all displays
      output_c(digit100);         // Send hundreds digit
      if ( digit == 0 && digitb == 0 && digitc == 0 && digitd == 0  ){ output_a(0x00);}
      else{ output_a(0x08);  }    // Turn on display for hundreds 0000 1101 , 00 11 0111 0xd ==37 //00 00 1000
      delay_ms(5);
       
      digit = (rpm / 1000) % 10;
      digitb = digit;
      digit1000 = display(digit);
      output_a(0x00);             // Turn off all displays
      output_c(digit1000);        // Send thousands digit
      if ( digit == 0 && digitc == 0 && digitd == 0  ){ output_a(0x00);}
      else{ output_a(0x04);  }    // Turn on display for thousands 0000 1110 00 11 1011 0xe ==3B //00 00 0100
      delay_ms(5);
   
      digit = (rpm / 10000) % 10;
      digitc = digit;
      digit10000 = display(digit);
      output_a(0x00);              // Turn off all displays
      output_c(digit10000);        // Send thousands digit
      if ( digit == 0 && digitd == 0  ){ output_a(0x00);}
      else{ output_a(0x02); }      // Turn on display for thousands 00 11 1101 3D //00 00 0010
      delay_ms(5);
       
      digit = (rpm / 100000) % 10;
      digitd = digit;
      digit100000 = display(digit);
      output_a(0x00);               // Turn off all displays
      output_c(digit100000);        // Send thousands digit
      if ( digit == 0 ){  output_a(0x00);}
      else{ output_a(0x01); }      // Turn on display for thousands 00 11 1110 //00 00 0001
      delay_ms(5);
   }
   else { output_a(0x00); }
}
