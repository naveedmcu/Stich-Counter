 #byte PIR2   = 0xFA1 
   
 #byte EECON1 = 0xFA6 
 #byte EECON2 = 0xFA7 
 #byte EEDATA = 0xFA8 
 #byte EEADR  = 0xFA9 
  
 #bit EEPGD = EECON1.7
 #bit CFGS  = EECON1.6
  
 #bit RD    = EECON1.0
 #bit WR    = EECON1.1
 #bit WREN  = EECON1.2
 
 #bit EEIF    = PIR2.4
 
 void write_internal_eeprom ( unsigned char address, unsigned char data );
 unsigned char read_internal_eeprom ( unsigned char address );
