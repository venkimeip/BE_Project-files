//header files

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include<compat/deprecated.h>
#include<avr/eeprom.h>


//subroutines used
char change(unsigned char i);
void RTC_init(void);
void update_date_max();
void disp(unsigned char d,unsigned char e);
void lcd_init();
void lcd_busy();
void lcd_command(unsigned char a);
void lcd_senddata(unsigned char z);
void getascii(unsigned char k);
void disp_time();

void USARTInit(uint16_t ubrr_value);
void USARTWriteChar(char data);
char USARTReadChar(void);
void sound(unsigned char x);


//uint8_t buff[100]PROGMEM =
volatile uint8_t buff[400];//PROGMEM ="XXMondayWCOMRASMEMBDANILNSRTSHNAGSM MMTATECHSEMIPROJVDYA";
//volatile uint8_t mon[20] ="Monday";
volatile uint8_t day1[80];
//volatile uint8_t msg1[]="PC Commn:";
//volatile uint8_t msg2[]="Prog mode Active";
//volatile uint8_t mon[6][2][4];
//volatile uint8_t tue[6][2][4];
//volatile uint8_t wed[6][2][4];
//volatile uint8_t thu[6][2][4];
//volatile uint8_t fri[6][2][4];
//volatile uint8_t sat[6][2][4];
volatile uint8_t today[6][2][4];
void load_day(uint8_t a);
volatile uint8_t daylist[7][3]={"Mon","Tue","Wed","Thr","Fri","Sat","Sun"};



//global variables
//-------------------------------------------------------------------------
volatile uint8_t RTC_hour, RTC_minute, RTC_second, RTC_date, RTC_month ,
	RTC_date_max,ascii_msb,ascii_lsb,RTC_fs,RTC_hourchanged,disp_flag,
	day_pointer=0;
unsigned int  RTC_year,eprm=0,eprm_timeout;


//-------------------------------------------------------------------------

//uint8_t 
//---main program begins-------------------------------------------------------------

int main(void)
{


DDRA=0xFF;//lcd data
DDRC=0xFF; //lcd rs and en signals ,rtc xtal
DDRB=0X00;
DDRD=0x80;//uart rx,tx and time set signals int0,up,down, function select,pd7 buzzer
//DDRD=0x80;
PORTD=0;

lcd_init();
RTC_init(); /* initialize async RTC with 32kHz-Crystal */
USARTInit(575);//initialize uart for =>
/*  values for selecting baudrate
71=>9600,
143=>4800,
287=>2400,
575=>1200,
2303=>300

*/


for(unsigned int e=0;e<400;e++)
{
buff[e]=eeprom_read_byte(e);//load to buffer
}

//load_mon();
//int0(time set) and int 1(vb)configure begin
//DDRD=0x00;//int0 port defined as input to accept interrupt
MCUCR=0X00;//select low to high level trigger for int0
GICR=0X40;//enable int0,int1




//load();
disp_time();

sei();//enable global int


unsigned char i,j,m,c_count=0,t;
//for(m=0;m<3;m++)
//disp(0xc0+m,'1');//daylist[0][m]);
//_delay_ms(500);

//t=0;

//for(m=0xc0;m<0xcf;m++)
//disp(m,day1[t++]);
//disp(m,buff[t++]);
/*
USARTWriteChar('A');
USARTWriteChar('A');
USARTWriteChar('A');
USARTWriteChar('A');
USARTWriteChar('A');
USARTWriteChar('A');
USARTWriteChar('A');

*/
//loop:
while(1) 
{
update_disp();//refresh lcd
//check for slave mode 
i=PIND;
i=i & 0x08;
if(i==!0x08)
{

load_day(t);
t++;
if(t>6)
T=0;
//mon_disp();
}


}}


//main ends

/*
void mon_disp()
{
disp(0xc3,'>');
uint8_t a,b,c,d;
//d=0xc0;
if(RTC_second>=0 && RTC_second<10)
a=0;
else 
if(RTC_second>=10 && RTC_second<20)
a=1;
else 
if(RTC_second>=20 && RTC_second<30)
a=2;
else 
if(RTC_second>=30 && RTC_second<40)
a=3;
else 
if(RTC_second>=40 && RTC_second<50)
a=4;
else 
if(RTC_second>=50 && RTC_second<60)
a=5;



//a=RTC_second;

switch(a)
{
case 0:d=0xc4;
	for(b=0;b<2;b++)
	{
	for(c=0;c<4;c++)
	disp(d++,today[0][b][c]);
	if(b==0){
	disp(d++,' ');
	disp(d++,'B');
	disp(d++,'Y');
	disp(d++,' ');
	}}
//sound(0);
	break;

case 1:d=0xc4;
	for(b=0;b<2;b++)
	{
	for(c=0;c<4;c++)
	disp(d++,mon[1][b][c]);
	if(b==0){
	disp(d++,' ');
	disp(d++,'B');
	disp(d++,'Y');
	disp(d++,' ');
	}}
break;
case 2:
d=0xc4;
	for(b=0;b<2;b++)
	{
	for(c=0;c<4;c++)
	disp(d++,mon[2][b][c]);
	if(b==0){
	disp(d++,' ');
	disp(d++,'B');
	disp(d++,'Y');
	disp(d++,' ');
	}}
break;
case 3:d=0xc4;
	for(b=0;b<2;b++)
	{
	for(c=0;c<4;c++)
	disp(d++,mon[3][b][c]);
	if(b==0){
	disp(d++,' ');
	disp(d++,'B');
	disp(d++,'Y');
	disp(d++,' ');
	}}
break;
case 4:d=0xc4;
	for(b=0;b<2;b++)
	{
	for(c=0;c<4;c++)
	disp(d++,mon[4][b][c]);
	if(b==0){
	disp(d++,' ');
	disp(d++,'B');
	disp(d++,'Y');
	disp(d++,' ');
	}}
break;
case 5:d=0xc4;
	for(b=0;b<2;b++)
	{
	for(c=0;c<4;c++)
	disp(d++,mon[5][b][c]);
	if(b==0){
	disp(d++,' ');
	disp(d++,'B');
	disp(d++,'Y');
	disp(d++,' ');
	}}
break;

}





}

*/
void update_disp()
{
if(RTC_second%2){
disp(0x82,':');
lcd_command(0x0c);
}
else
{
disp(0x82,' ');
lcd_command(0x0c);
}
if(RTC_hourchanged)
{
sound(150);
RTC_hourchanged=0;
}

if(disp_flag)
{
disp_time();
disp_flag=0;
//disp(0xc5,UDR);

}

}
/*
if(RTC_second % 2)
disp(0x82,':');
else
{
disp(0x82,' ');
lcd_command(0x0c);
//sound(100);
}

*/



//disp_time();
////m=PIND;
//m=m & 0x04;

//if(m==0x04){
//_delay_ms(500);
//m=PIND;
//m=m & 0x04;

//if(m==0x04){
//cli();

//int1();
//sei();
//}
//_delay_ms(1000);
//}

//for(t=0;t<16;t++)
//disp(t," ");
///_delay_ms(1000);


//j= USARTReadChar();

//USARTWriteChar('[');

//USARTWriteChar(j);
//USARTWriteChar('E');
/*

j= USARTReadChar();
if(j==0x31){

m=0xc0;
disp(m++,'R');
disp(m++,'x');



for(i=0;i<101;i++)
{


//USARTWriteChar(']');
  j= USARTReadChar();
  eeprom_write_byte(i,j);
  // disp(i,j);
c_count++;
}

//USARTWriteChar('f');
goto loop;
}


//}
else if(j==0x32){

m=0xc0;
disp(m++,'T');
disp(m++,'x');


//USARTWriteChar('R');

for(i=0;i<c_count;i++)
{


//USARTWriteChar(']');
   
  j= eeprom_read_byte(i);
USARTWriteChar(j);

  // disp(i,j);
}
goto loop;

}
*/

/*

for(i=0xc0;i<=0xcf;i++)
{
USARTWriteChar(']');
   j= USARTReadChar();
   disp(i,j);


}*/
  // USARTWriteChar('A');




//main ends

/* 
Start Timer/Counter2 in asynchronous operation using a 32.768kHz crystal.
*/


void RTC_init(void)
{
	uint8_t sreg;/* Save status register and disable global interrupts */
	sreg=SREG;
	cli();	/* 0. Oscillator might take as long as one second to stabilize. */
	_delay_ms(1000);	/* 1. Disable the Timer/Counter2 interrupts by clearing OCIE2 and TOIE2. */
	TIMSK &= ~((1<<OCIE2)|(1<<TOIE2));	/* 2. Select clock source by setting AS2 as appropriate. */
	ASSR = (1<<AS2);	/* 3. Write new values to TCNT2, OCR2, and TCCR2. */
	TCNT2 = 0;	// clear TCNT2A	// select precaler: 32.768 kHz / 128 = 1 sec between each overflow:
	TCCR2 |= (1<<CS22) | (1<<CS20);	/* 4. To switch to asynchronous operation: Wait for TCN2UB, OCR2UB, and
	TCR2UB. */
	while( (ASSR & (1<<TCN2UB)) | (ASSR & (1<<OCR2UB)) |
		(ASSR & (1<<TCR2UB)) ); 
	
	/* 5. Clear the Timer/Counter2 Interrupt Flags. */
	TIFR |= ((1<<OCF2)|(1<<TOV2));
	
	/* 6. enable Timer2 Overflow interrupt */
	TIMSK |= (1<<TOIE2);
    
	/* initial values */
	RTC_hour=23;
	RTC_minute=59;
	RTC_second=55;
    RTC_date_max=31;
	RTC_date=31;
	RTC_month=3;
	RTC_year=10;
	RTC_hourchanged=0;
	ascii_msb=0;
	ascii_lsb=0;

	/* restore status-register */
	SREG=sreg;
}

//--------------------------------------------------------------------------------------





void update_date_max()
{

  		if((RTC_month==1) || (RTC_month==3) || ( RTC_month==5) || (RTC_month==7) || ( RTC_month==8 ) || ( RTC_month==10) || (RTC_month==12))

   		{

   			RTC_date_max=31;

   		};   

   		 if(  RTC_month==4 || RTC_month==6 || RTC_month==9 || RTC_month==11)

   		{ RTC_date_max=30; };

    

  		 if(RTC_month==2)

   		 {   RTC_date_max=28;

  		  if(((RTC_year%4)==0) ||	 ((RTC_year%400)==0	) )

			RTC_date_max++;
			};
	//--------------------date max ends

}

 //Timer2-Overflow ISR called every second
//SIGNAL(SIG_OVERFLOW2)
ISR(TIMER2_OVF_vect)
{ 
//clear 2 nd row of lcd
//for(unsigned char t=0;t<16;t++)

//disp(t+0xc0,' ');
//eprm=0;

//lcd_command(0x0c);//cursor off
//disp end
//if(eprm_timeout+5>RTC_second)
//{
//for(unsigned char t=0;t<16;t++)

//disp(t+0xc0,' ');

//eprm=0;
//}
RTC_second++;
//sound(1);
//disp_time();
 if (RTC_second>=60)        //keep track of time, date, month, and year
    {      
 	       RTC_second=0;
		   	RTC_minute++;
			

	}
 if (RTC_minute>=60) 
    {
	//if(eprm_timeout+5>RTC_second)
{
for(unsigned char t=0;t<16;t++)

disp(t+0xc0,' ');

//eprm=0;
}

            RTC_minute=0;
			RTC_hour++;
	}		
	
 if (RTC_hour>=24)
    {			day_pointer++;
                RTC_hour=0;
				RTC_date++; 
	            RTC_hourchanged=1;
	}
	

if(RTC_date > RTC_date_max)
   {
   RTC_date=1;
   RTC_month++;
   update_date_max();	 
}


if (RTC_month > 12)

   {
   RTC_month=1;
   RTC_year++;
   
}
disp_flag=1;
//disp_time();


}

//------------------date max decide-routine-----------------------------------------------------------------------------------


//LCD Begin
#define lcd_en PC1
#define lcd_rs PC0
void disp(unsigned char d,unsigned char e)
{
lcd_command (d);
lcd_senddata(e);
}


void lcd_init()
{
lcd_command(0x38);
lcd_command(0x0f);
lcd_command(0x06);
lcd_command(0x01);
lcd_command(0x11);

}


void lcd_busy()
{
unsigned char i,j;
for(i=0;i<60;i++) //A simple for loop for delay
for(j=0;j<255;j++);
}


void lcd_command(unsigned char a)
{
cbi(PORTC,PC0);
PORTA=a;
sbi(PORTC,PC1);
_delay_ms(1);
cbi(PORTC,PC1);
lcd_busy();
}


void lcd_senddata(unsigned char z)
{
sbi(PORTC,PC0);
PORTA=z;
sbi(PORTC,PC1);
_delay_ms(1);
cbi(PORTC,PC1);
lcd_busy();
}

//LCD ends

//int0 ends







void getascii(unsigned char k)
{
unsigned char j,l;//lsb,msb
j=0;
l=0;
j=k%10;
l=k/10;
if(j>9)
{
l++;
j-=10;
}

l=l|0x30;
ascii_msb=l;
j=j | 0x30;
ascii_lsb=j;



}



void disp_time()
{
unsigned char i,j;

getascii(RTC_hour);
disp(0x80,ascii_msb);
disp(0x81,ascii_lsb);

disp(0x82,':'); //seconda blinker


getascii(RTC_minute);
disp(0x83,ascii_msb);
disp(0x84,ascii_lsb);
disp(0x85,' ');
//disp(0x86,' ');

getascii(RTC_date);
disp(0x86,ascii_msb);
disp(0x87,ascii_lsb);

disp(0x88,'/');

getascii(RTC_month);
disp(0x89,ascii_msb);
disp(0x8a,ascii_lsb);

disp(0x8b,'/');

getascii(RTC_year);
disp(0x8c,ascii_msb);
disp(0x8d,ascii_lsb);

//disp(0x8f,' ');
for(i=0;i<3;i++)
disp(i+0xc0,daylist[day_pointer][i]);
lcd_command(0x0e);

}





//-----------------int0 isr begin for time setting----------------------------



//--------------------uarrt begin
void USARTInit(uint16_t ubrr_value)
{
  		 //Set Baud rate
  		 UBRRL = ubrr_value;
  		 UBRRH = (ubrr_value>>8);
  		 /*Set Frame Format
  		 >> Asynchronous mode
  		 >> No Parity
  		 >> 1 StopBit
  		 >> char size 8
  		 */
  		 UCSRC=(1<<URSEL)|(3<<UCSZ0);
  		 //Enable The receiver and transmitter
  		 UCSRB=(1<<RXEN)|(1<<TXEN)|(1 << RXCIE);
		// 	UCSR1B |= ; // Enable the USART Recieve Complete interrupt 
while(!(UCSRA & (1 << UDRE)));
	UDR = 0x43; // "C"
	while(!(UCSRA & (1 << UDRE)));
	UDR = 0x0d;

}

//This function is used to read the available data
//from USART. This function will wait untill data is
//available.


char USARTReadChar()
{
   		//Wait untill a data is available

   		while(!(UCSRA & (1<<RXC)));
   

   		//Now USART has got data from host
   		//and is available is buffer

  		 return UDR;
}


//This fuction writes the given "data" to
//the USART which then transmit it via TX line
void USARTWriteChar(char data)
{
   		//Wait untill the transmitter is ready

   		while(!(UCSRA & (1<<UDRE)));
   		//Now write the data to USART buffer
   		UDR=data;
}


//change routine for time set function 
char change(unsigned char i)
{
		unsigned char j,up,dn;
		j=i;
		up=0;
		dn=0;
//-----------increment key function
		up=PIND;
		up &= 0x20;
		if((up) && (!dn))
		j++;
		_delay_ms(600);


//--------------decrement key function
		dn=PIND;
		dn &= 0x10;
		if((!up) && (dn))
		j--;
		_delay_ms(600);

//return variable
		return(j);
}


//buzzer begin connected to pd7
void sound(unsigned char x)
{
DDRD=0x80;
unsigned char i,j;
while(x--){
for(i=0;i<25;i++)
{
cbi(PORTD,PD7);
_delay_us(500);
sbi(PORTD,PD7);
_delay_us(1000);
}}
DDRD=0x00;
return 0;
}

SIGNAL (SIG_UART_RECV) { // USART RX interrupt
	unsigned char c;
	static unsigned char a,b;
	c = UDR;
if(eprm==0)
{
//	eprm_timeout=RTC_second;

a=c;
		//disp(0xC7,'D');
		//disp(0xC8,'A');
		//disp(0xC9,'T');
	//	disp(0xCA,'A');

}		

//if(eprm=1)
//b=UDR;
//disp(0xc1,(0x30|(eprm & 0x0f)));


if((a=='$'))// &&(b=='X'))
{


		disp(0xCE,'R');
		disp(0xCF,'X');

//	USARTWriteChar('1');
	eeprom_write_byte(eprm++,c);
//	buff[eprm]=c;
	//disp(0xc0,c);
	if(eprm==0x1ff)
	eprm=0;

}

else if((a=='!'))// &&(b=='X'))
{


		disp(0xce,'T');
		disp(0xcf,'X');
unsigned int k;
for(k=0;k<512;k++){
USARTWriteChar(eeprom_read_byte(k));
	//	buff[eprm]=c;
	//disp(0xc0,c);
	//if(eprm==0x1ff)
	//eprm=0;

}
}
}





SIGNAL(SIG_INTERRUPT0)     
//signal handler for external interrupt int0 
{
unsigned char t,temp2=0;
RTC_second=0;

uint8_t msg[]="TimeSetMode ";
disp(0xc3,'>');

for(t=0;t<13;t++)
disp(t+0xc4,msg[t]);

lcd_command(0x0c);

//cursor function select check

temp2=PIND;
temp2 &= 0x40;


if(temp2)
RTC_fs++;


if(RTC_fs >5)
RTC_fs=0;

_delay_ms(500);
lcd_command(0x0e);
       

switch(RTC_fs)
{
case 0:lcd_command(0x80);
       RTC_hour=change(RTC_hour);
	   if(RTC_hour>23)
	   RTC_hour=0;
	   //else if(RTC_hour>23)
	   //RTC_hour=0;
	   disp_time();
       break;


case 1:
       	lcd_command(0x83);
		RTC_minute=change(RTC_minute);
 		if(RTC_minute>59)
	  	 RTC_minute=0;
 		disp_time();
		break;

case 2:

		lcd_command(0x86);
		RTC_date=change(RTC_date);
         update_date_max();
 		if(RTC_date > RTC_date_max)
	  	 RTC_date=1;
   		 disp_time();
		break;

case 3:
		lcd_command(0x89);
		RTC_month=change(RTC_month);
 		if(RTC_month>12)
	  	 RTC_month=1;
    	disp_time();
		break;

case 4:
		lcd_command(0x8c);
		RTC_year=change(RTC_year);
 		if(RTC_year>99)
	   	RTC_year=0;
    	disp_time();
		break;
case 5:
		lcd_command(0xc0);
		day_pointer=change(day_pointer);
 		if(day_pointer>6)
	   	day_pointer=0;
    	disp_time();
		break;

default:RTC_fs=0;

}
//clear msg display
for(unsigned char t=0;t<12;t++)
disp(t+0xc3,' ');


}

//---------------------------int0 isr ends----------------
//------------------------------------------------------------------------------------------------
/*
void load_mon
        (){
uint8_t i,j,c,e=0,k;
j=0;
i=0;
k=0;



USARTWriteChar('E');


for(i=0;i<7;)
{


//k=eeprom_read_byte(j);


//if(daylist[0][i]==buff[j]) //rx only 3 letters for each day from vb

if(daylist[0][i]==buff[j])

{
i++;
j++;

USARTWriteChar('1');

if(i==3)
{
j+=1;//remove this line if no synch characters :)
USARTWriteChar('y');
USARTWriteChar(0x0d);
USARTWriteChar(0x0a);

goto exit;
}


}
//USARTWriteChar('K');
else
{
//USARTWriteChar(buff[j]);

//USARTWriteChar(mon[i]);

USARTWriteChar('0');

j=j-i+1;
i=0;
if(j==150)
{
USARTWriteChar('n');
goto exit;
}
}
//USARTWriteChar('O');

}
//if(i==3)


//disp(0x8f,'y');
//PORTA=~PORTA;
//goto cont;


exit:

e=0;

//cont://PORTA=0xff;


for(c=j;c<j+70;c++)
{
day1[e++]=buff[c];
}
//uint8_t  i,j,c are fre

USARTWriteChar('S');
USARTWriteChar('U');
USARTWriteChar('B');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar('S');
USARTWriteChar('t');
USARTWriteChar('a');
USARTWriteChar('f');
USARTWriteChar('f');

USARTWriteChar(0x0d);
USARTWriteChar(0x0a);


e=0;
for(i=0;i<6;i++){
for(j=0;j<2;j++){
for(k=0;k<4;k++){

if(day1[e]=='#')
e++;

mon[i][j][k]=day1[e++];
USARTWriteChar(mon[i][j][k]);
//if(j==3)
}
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');


//}

}
USARTWriteChar(0x0d);
USARTWriteChar(0x0a);
//USARTWriteChar(' ');
//USARTWriteChar(' ');
//USARTWriteChar(' ');

}


}



//tue begin
void load_tue()

{
uint8_t i,j,c,e=0,k;
j=0;
i=0;
k=0;


USARTWriteChar('E');


for(i=0;i<7;)
{


//k=eeprom_read_byte(j);


//if(daylist[0][i]==buff[j]) //rx only 3 letters for each day from vb
if(daylist[1][i]==buff[j])

{
i++;
j++;

USARTWriteChar('1');

if(i==3)
{
j+=1;//remove this line if no synch characters :)
USARTWriteChar('y');
USARTWriteChar(0x0d);
USARTWriteChar(0x0a);

goto exit;
}


}
//USARTWriteChar('K');
else
{
//USARTWriteChar(buff[j]);

//USARTWriteChar(mon[i]);

USARTWriteChar('0');

j=j-i+1;
i=0;
if(j==200)
{
USARTWriteChar('n');
goto exit;
}
}
//USARTWriteChar('O');

}
//if(i==3)


//disp(0x8f,'y');
//PORTA=~PORTA;
//goto cont;


exit:

e=0;

//cont://PORTA=0xff;


for(c=j;c<j+70;c++)
{
day1[e++]=buff[c];
}
//uint8_t  i,j,c are fre

USARTWriteChar('S');
USARTWriteChar('U');
USARTWriteChar('B');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar('S');
USARTWriteChar('t');
USARTWriteChar('a');
USARTWriteChar('f');
USARTWriteChar('f');

USARTWriteChar(0x0d);
USARTWriteChar(0x0a);


e=0;
for(i=0;i<6;i++){
for(j=0;j<2;j++){
for(k=0;k<4;k++){

if(day1[e]=='#')
e++;

tue[i][j][k]=day1[e++];
USARTWriteChar(tue[i][j][k]);
//if(j==3)
}
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');


//}

}
USARTWriteChar(0x0d);
USARTWriteChar(0x0a);
//USARTWriteChar(' ');
//USARTWriteChar(' ');
//USARTWriteChar(' ');

}


}

//tue ends and wed begin
void load_wed()

{
uint8_t i,j,c,e=0,k;
j=0;
i=0;
k=0;



USARTWriteChar('E');


for(i=0;i<7;)
{


//k=eeprom_read_byte(j);


//if(daylist[0][i]==buff[j]) //rx only 3 letters for each day from vb
if(daylist[2][i]==buff[j])

{
i++;
j++;

USARTWriteChar('1');

if(i==3)
{
j+=1;//remove this line if no synch characters :)
USARTWriteChar('y');
USARTWriteChar(0x0d);
USARTWriteChar(0x0a);

goto exit;
}


}
//USARTWriteChar('K');
else
{
//USARTWriteChar(buff[j]);

//USARTWriteChar(mon[i]);

USARTWriteChar('0');

j=j-i+1;
i=0;
if(j==200)
{
USARTWriteChar('n');
goto exit;
}
}
//USARTWriteChar('O');

}
//if(i==3)


//disp(0x8f,'y');
//PORTA=~PORTA;
//goto cont;


exit:

e=0;

//cont://PORTA=0xff;


for(c=j;c<j+70;c++)
{
day1[e++]=buff[c];
}
//uint8_t  i,j,c are fre

USARTWriteChar('S');
USARTWriteChar('U');
USARTWriteChar('B');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar('S');
USARTWriteChar('t');
USARTWriteChar('a');
USARTWriteChar('f');
USARTWriteChar('f');

USARTWriteChar(0x0d);
USARTWriteChar(0x0a);


e=0;
for(i=0;i<6;i++){
for(j=0;j<2;j++){
for(k=0;k<4;k++){

if(day1[e]=='#')
e++;

wed[i][j][k]=day1[e++];
USARTWriteChar(wed[i][j][k]);
//if(j==3)
}
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');


//}

}
USARTWriteChar(0x0d);
USARTWriteChar(0x0a);
//USARTWriteChar(' ');
//USARTWriteChar(' ');
//USARTWriteChar(' ');

}


}
//thr begin
void load_thr()

{
uint8_t i,j,c,e=0,k;
j=0;
i=0;
k=0;



USARTWriteChar('E');


for(i=0;i<7;)
{


//k=eeprom_read_byte(j);


//if(daylist[0][i]==buff[j]) //rx only 3 letters for each day from vb
if(daylist[3][i]==buff[j])

{
i++;
j++;

USARTWriteChar('1');

if(i==3)
{
j+=1;//remove this line if no synch characters :)
USARTWriteChar('y');
USARTWriteChar(0x0d);
USARTWriteChar(0x0a);

goto exit;
}


}
//USARTWriteChar('K');
else
{
//USARTWriteChar(buff[j]);

//USARTWriteChar(mon[i]);

USARTWriteChar('0');

j=j-i+1;
i=0;
if(j==200)
{
USARTWriteChar('n');
goto exit;
}
}
//USARTWriteChar('O');

}
//if(i==3)


//disp(0x8f,'y');
//PORTA=~PORTA;
//goto cont;


exit:

e=0;

//cont://PORTA=0xff;


for(c=j;c<j+70;c++)
{
day1[e++]=buff[c];
}
//uint8_t  i,j,c are fre

USARTWriteChar('S');
USARTWriteChar('U');
USARTWriteChar('B');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar('S');
USARTWriteChar('t');
USARTWriteChar('a');
USARTWriteChar('f');
USARTWriteChar('f');

USARTWriteChar(0x0d);
USARTWriteChar(0x0a);


e=0;
for(i=0;i<6;i++){
for(j=0;j<2;j++){
for(k=0;k<4;k++){

if(day1[e]=='#')
e++;

thu[i][j][k]=day1[e++];
USARTWriteChar(thu[i][j][k]);
//if(j==3)
}
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');


//}

}
USARTWriteChar(0x0d);
USARTWriteChar(0x0a);
//USARTWriteChar(' ');
//USARTWriteChar(' ');
//USARTWriteChar(' ');

}


}

//fri begin
void load_fri()

{
uint8_t i,j,c,e=0,k;
j=0;
i=0;
k=0;



USARTWriteChar('E');


for(i=0;i<7;)
{


//k=eeprom_read_byte(j);


//if(daylist[0][i]==buff[j]) //rx only 3 letters for each day from vb
if(daylist[4][i]==buff[j])

{
i++;
j++;

USARTWriteChar('1');

if(i==3)
{
j+=1;//remove this line if no synch characters :)
USARTWriteChar('y');
USARTWriteChar(0x0d);
USARTWriteChar(0x0a);

goto exit;
}


}
//USARTWriteChar('K');
else
{
//USARTWriteChar(buff[j]);

//USARTWriteChar(mon[i]);

USARTWriteChar('0');

j=j-i+1;
i=0;
if(j==300)
{
USARTWriteChar('n');
goto exit;
}
}
//USARTWriteChar('O');

}
//if(i==3)


//disp(0x8f,'y');
//PORTA=~PORTA;
//goto cont;


exit:

e=0;

//cont://PORTA=0xff;


for(c=j;c<j+70;c++)
{
day1[e++]=buff[c];
}
//uint8_t  i,j,c are fre

USARTWriteChar('S');
USARTWriteChar('U');
USARTWriteChar('B');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar('S');
USARTWriteChar('t');
USARTWriteChar('a');
USARTWriteChar('f');
USARTWriteChar('f');

USARTWriteChar(0x0d);
USARTWriteChar(0x0a);


e=0;
for(i=0;i<6;i++){
for(j=0;j<2;j++){
for(k=0;k<4;k++){

if(day1[e]=='#')
e++;

fri[i][j][k]=day1[e++];
USARTWriteChar(fri[i][j][k]);
//if(j==3)
}
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');


//}

}
USARTWriteChar(0x0d);
USARTWriteChar(0x0a);
//USARTWriteChar(' ');
//USARTWriteChar(' ');
//USARTWriteChar(' ');

}


}

//sat begin
void load_sat()

{
uint8_t i,j,c,e=0,k;
j=0;
i=0;
k=0;



USARTWriteChar('E');


for(i=0;i<7;)
{


//k=eeprom_read_byte(j);


//if(daylist[0][i]==buff[j]) //rx only 3 letters for each day from vb
if(daylist[5][i]==buff[j])

{
i++;
j++;

USARTWriteChar('1');

if(i==3)
{
j+=1;//remove this line if no synch characters :)
USARTWriteChar('y');
USARTWriteChar(0x0d);
USARTWriteChar(0x0a);

goto exit;
}


}
//USARTWriteChar('K');
else
{
//USARTWriteChar(buff[j]);

//USARTWriteChar(mon[i]);

USARTWriteChar('0');

j=j-i+1;
i=0;
if(j==200)
{
USARTWriteChar('n');
goto exit;
}
}
//USARTWriteChar('O');

}
//if(i==3)


//disp(0x8f,'y');
//PORTA=~PORTA;
//goto cont;


exit:

e=0;

//cont://PORTA=0xff;


for(c=j;c<j+70;c++)
{
day1[e++]=buff[c];
}
//uint8_t  i,j,c are fre

USARTWriteChar('S');
USARTWriteChar('U');
USARTWriteChar('B');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar('S');
USARTWriteChar('t');
USARTWriteChar('a');
USARTWriteChar('f');
USARTWriteChar('f');

USARTWriteChar(0x0d);
USARTWriteChar(0x0a);


e=0;
for(i=0;i<6;i++){
for(j=0;j<2;j++){
for(k=0;k<4;k++){

if(day1[e]=='#')
e++;

sat[i][j][k]=day1[e++];
USARTWriteChar(sat[i][j][k]);
//if(j==3)
}
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');


//}

}
USARTWriteChar(0x0d);
USARTWriteChar(0x0a);
//USARTWriteChar(' ');
//USARTWriteChar(' ');
//USARTWriteChar(' ');

}


}

//sun begin
load_sun()

{
uint8_t a,b;

uint8_t msg[]="SUNDAY IS HOLIDAY";
for(a=0;a<18;a++)
USARTWriteChar(msg[a]);

}


load_days(){

switch(day_pointer)
{
case 0:load_mon();
break;
case 1:load_tue();
break;
case 2:load_wed();
break;
case 3:load_thr();
break;
case 4:load_fri();
break;
case 5:load_sat();
break;

default:load_sun();



}





}

*/


void load_day(uint8_t a)

{
uint8_t i,j,c,e=0,k;
j=0;
i=0;
k=0;



USARTWriteChar('E');


for(i=0;i<7;)
{


//k=eeprom_read_byte(j);


//if(daylist[0][i]==buff[j]) //rx only 3 letters for each day from vb
if(daylist[a][i]==buff[j])

{
i++;
j++;

USARTWriteChar('1');

if(i==3)
{
j+=1;//remove this line if no synch characters :)
USARTWriteChar('y');
USARTWriteChar(0x0d);
USARTWriteChar(0x0a);

goto exit;
}


}
//USARTWriteChar('K');
else
{
//USARTWriteChar(buff[j]);

//USARTWriteChar(mon[i]);

USARTWriteChar('0');

j=j-i+1;
i=0;
if(j==399)
{
USARTWriteChar('n');
goto exit;
}
}
//USARTWriteChar('O');

}
//if(i==3)


//disp(0x8f,'y');
//PORTA=~PORTA;
//goto cont;


exit:

e=0;

//cont://PORTA=0xff;


for(c=j;c<j+70;c++)
{
day1[e++]=buff[c];
}
//uint8_t  i,j,c are fre

USARTWriteChar('S');
USARTWriteChar('U');
USARTWriteChar('B');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar('S');
USARTWriteChar('t');
USARTWriteChar('a');
USARTWriteChar('f');
USARTWriteChar('f');

USARTWriteChar(0x0d);
USARTWriteChar(0x0a);


e=0;
for(i=0;i<6;i++){
for(j=0;j<2;j++){
for(k=0;k<4;k++){

if(day1[e]=='#')
e++;

today[i][j][k]=day1[e++];
USARTWriteChar(today[i][j][k]);
//if(j==3)
}
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');
USARTWriteChar(' ');


//}

}
USARTWriteChar(0x0d);
USARTWriteChar(0x0a);
//USARTWriteChar(' ');
//USARTWriteChar(' ');
//USARTWriteChar(' ');

}


}





