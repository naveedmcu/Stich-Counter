void write_internal_eeprom ( unsigned char address, unsigned char data )
{
   EEADR  = address;
   EEDATA = data;
   
   EEPGD = 0;          // 0 = Access data EEPROM memory
   CFGS  = 0;         // 0 = Access Flash program or DATA EEPROM memory
   WREN  = 1;        // enable writes to internal EEPROM
   
   disable_interrupts(GLOBAL);
   
   EECON2=0x55;        // Required sequence for write to internal EEPROM
   EECON2=0xaa;        // Required sequence for write to internal EEPROM
   
   WR = 1;            // begin write to internal EEPROM
   enable_interrupts(GLOBAL);
   delay_us (1);
   while (EEIF == 0); //Wait till write operation complete
   {
      delay_us (1);
   }
   WREN = 0;      // Disable writes to EEPROM on write complete (EEIF flag on set PIR2 )
   EEIF = 0;      //Clear EEPROM write complete flag. (must be cleared in software. So we do it here)
}
////////////////////////////////////////////////////////////////////////////////
unsigned char read_internal_eeprom ( unsigned char address )
{
   EEADR=address;
   
   EEPGD = 0;          // 0 = Access data EEPROM memory
   CFGS  = 0;         // 0 = Access Flash program or DATA EEPROM memory
   RD = 1 ;          //enable the read bit for Read EEPROM operation
   return EEDATA;   //Return the Data stored in Internal EEPROM
}
////////////////////////////////////////////////////////////////////////////////
