Project is based on Atmega16 8 bit mcu;

Masterdevice:Connects to PC/XBEE-2 Zigbee module;
LCD 8 bit interface is present
Internal RTC configured for time calculation
Uart configured for PC inteface


Slave device:

8 Dotmatrix LED Displays are interfaced through SIPO Shift register 74HC164(as i remember);
Receives RTC Time data and Time table data from the master; extracts as 4 characters groups (Staff: Subject) format;

