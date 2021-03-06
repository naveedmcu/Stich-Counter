#ifndef RTC_SDA
#define RTC_SDA  PIN_E1
#define RTC_SCL  PIN_E0
#endif

#use i2c(master, sda=RTC_SDA, scl=RTC_SCL)

#define DS1307_ALL_DISABLED         0b00000000 // All disabled
#define DS1307_OUT_ON_DISABLED_HIHG 0b10000000 // Out to Hight on Disable Out
#define DS1307_OUT_ENABLED          0b00010000 // Out Enabled
#define DS1307_OUT_1_HZ             0b00000000 // Freq. Out to 1 Hz
#define DS1307_OUT_4_KHZ            0b00000001 // Freq. Out to 4.096 Khz
#define DS1307_OUT_8_KHZ            0b00000010 // Freq. Out to 8.192 Khz
#define DS1307_OUT_32_KHZ           0b00000011 // Freq. Out to 32.768 Khz

#define Start_user_address_nvram    0x08
#define End_user_address_nvram      0x3f

byte BCD_to_decimal(byte bcd_value);
byte decimal_to_BCD(byte decimal_value);
/////////////////////////////////////////////////////////////////////////
void init_ds1307(void) {
    i2c_start(); // Start I2C communication
    i2c_write(0xD0); // Connect to DS1307 by sending its ID on I2c Bus
    i2c_write(0X07); // Select the Ds1307 ControlRegister to configure Ds1307 address
    i2c_write(0X10); // Write 0x10 to Control register to enable SQW-Out value
    
    //i2c_write(0X02); // Write 0x02 to hour register to disable 12 Hour address
    //i2c_write(0X6); // Write 0x00 to hour register to disable 12 Hour value 0110 0000
    i2c_stop(); // Stop I2C communication after initilizing DS1307
}
/////////////////////////////////////////////////////////////////////////////////
void ds1307_write(unsigned int8 address, data_){
  i2c_start();                                   // Start I2C protocol
  i2c_write(0xD0);                               // DS1307 address
  i2c_write(address);                            // Send register address
  i2c_write(decimal_to_BCD(data_));              // Write data to the selected register
  i2c_stop();                                    // Stop I2C protocol
}
///////////////////////////////////////////////////////////////////
void ds1307_get_time(byte &year, byte &month, byte &date, byte &day, byte &hour, byte &minute, byte &second){
i2c_start();                                  // Start I2C protocol
   i2c_write(0xD0);                              // DS1307 address
   i2c_write(0);                                 // Send register address
   i2c_start();                                  // Restart I2C
   i2c_write(0xD1);                              // Initialize data read
   second = BCD_to_decimal(i2c_read(1));                         // Read seconds from register 0
   minute = BCD_to_decimal(i2c_read(1));                         // Read minuts from register 1
   hour   = BCD_to_decimal(i2c_read(1));                         // Read hour from register 2
   day    = BCD_to_decimal(i2c_read(1));                         // Read day from register 3
   date   = BCD_to_decimal(i2c_read(1));                         // Read date from register 4
   month  = BCD_to_decimal(i2c_read(1));                         // Read month from register 5
   year   = BCD_to_decimal(i2c_read(0));                         // Read year from register 6
   i2c_stop();                                   // Stop I2C protocol
}
/////////////////////////////////////////////////////////////////////////////
byte BCD_to_decimal(byte bcd_value){
return((bcd_value >> 4) * 10 + (bcd_value & 0x0F));
}
/////////////////////////////////////////////////////////////////////////////
byte decimal_to_BCD(byte decimal_value){
return(((decimal_value / 10) << 4) + (decimal_value % 10));
}
////////////////////////////////////////////////////////////////////////////
void ds1307_write_nvram_byte(char addr, char value){

//#ifndef USE_INTERRUPTS
  // disable_interrupts(global);
//#endif

   i2c_start();
   i2c_write(0xD0);
   i2c_write(addr);
   i2c_write(value);
   i2c_stop();

//#ifndef USE_INTERRUPTS
  // enable_interrupts(global);
//#endif
}
////////////////////////////////////////////////////////////////////////////////
char ds1307_read_nvram_byte(char addr){

   char retval;

//#ifndef USE_INTERRUPTS
  // disable_interrupts(global);
//#endif

   i2c_start();
   i2c_write(0xD0);
   i2c_write(addr);

   i2c_start();
   i2c_write(0xD1);
   retval = i2c_read(0);
   i2c_stop();

   return(retval);

//#ifndef USE_INTERRUPTS
  // enable_interrupts(global);
//#endif
}
/**/
void ds1307_set_date_time(BYTE day, BYTE mth, BYTE year, BYTE dow, BYTE hr, BYTE min, BYTE sec)
{
  sec &= 0x7F;
  hr &= 0x3F;

  i2c_start();
  i2c_write(0xD0);            // I2C write address
  i2c_write(0x00);            // Start at REG 0 - Seconds
  i2c_write(decimal_to_BCD(sec));      // REG 0
  i2c_write(decimal_to_BCD(min));      // REG 1
  i2c_write(decimal_to_BCD(hr));      // REG 2
  i2c_write(decimal_to_BCD(dow));      // REG 3
  i2c_write(decimal_to_BCD(day));      // REG 4
  i2c_write(decimal_to_BCD(mth));      // REG 5
  i2c_write(decimal_to_BCD(year));      // REG 6
  i2c_write(0x10);            // REG 7 - 0x80 Disable squarewave output pin
  i2c_stop();
}