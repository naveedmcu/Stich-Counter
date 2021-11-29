unsigned int key_scan( void )
{
   if( !system_count.sw_r )
   {
      delay_ms( 20 );
      if( !system_count.sw_1 ) { system_count.sw_r = SET; return 1; }
      if( !system_count.sw_2 ) { system_count.sw_r = SET; return 2; }
      if( !system_count.sw_3 ) { system_count.sw_r = SET; return 3; }
   }
   else if( system_count.sw_1 && system_count.sw_2 && system_count.sw_3 ) { system_count.sw_r = RESET; }
   return 9;
}
////////////////////////////////////////////////////////////////////////////////

