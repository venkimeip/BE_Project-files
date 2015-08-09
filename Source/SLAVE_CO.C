/* RX CHARACTER FROM UART AND DISP ON DOTMATRIX WITH OVERLAP
*/

#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include<compat/deprecated.h>
#include<avr/eeprom.h>
volatile uint8_t RTC_hour, RTC_minute, RTC_second, RTC_date, RTC_month ,
	RTC_minutechanged,RTC_date_max,ascii_msb,ascii_lsb,RTC_year,count;
unsigned int  eprm;
//-------------------------------------------------------------------------
void USARTInit(uint16_t ubrr_value);
void USARTWriteChar(char data);
char USARTReadChar(void);
void sound(unsigned char x);
volatile uint8_t msg[40]="VENKATESHA R";
uint8_t text[200];
uint8_t time[100];

uint8_t text_ptr,time_ptr;
uint8_t array[100][5]PROGMEM ={
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},//10
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},//20
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},//30
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},//40
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0x00,0x18,0x18,0x18,0x00},// -
{0,0,0,0,0},
{0,0,0,0,0},
{0x7e,0x81,0x81,0x81,0x7e},//0
{0x01,0x41,0xff,0x01,0x01},//1 
{0x41,0x83,0x85,0x89,0x71},//2
{0x99,0x99,0x99,0x99,0x66},//3
{0xfc,0x04,0x1f,0x04,0x04},//4
{0xf1,0x91,0x91,0x91,0x8e},//5
{0x1e,0x21,0x49,0x89,0x06},//6
{0x80,0x80,0x87,0x88,0xf0},//7
{0x66,0x99,0x99,0x99,0x66},//8
{0x61,0x91,0x91,0x91,0x7e},//9
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0,0,0,0,0},
{0x7f,0x98,0x98,0x98,0x7f},//A
{0xff,0x99,0x99,0x99,0x66},//B
{0x3c,0x42,0x81,0x81,0x81},//C
{0xff,0x81,0x81,0x81,0x7e},//D
{0xff,0x99,0x99,0x99,0x99},//E
{0XFF,0x98,0x98,0x98,0x98},//F
{0X3E,0x41,0x81,0xC9,0xCF},//G
{0xff,0x18,0x18,0x18,0xff},//H
{0x81,0x81,0xff,0x81,0x81},//I
{0x83,0x81,0xfe,0x80,0x80},//J
{0xff,0x18,0x24,0x42,0x81},//K
{0xff,0x01,0x01,0x01,0x01},//L
{0xff,0x40,0x20,0x40,0xff},//M
{0xff,0x30,0x18,0x0C,0xff},//N
{0x7e,0x81,0x81,0x81,0x7e},//O
{0xff,0x90,0x90,0x90,0x60},//P
{0x7e,0x81,0x85,0x83,0x7f},//Q
{0xff,0x90,0x98,0x94,0x63},//R
{0x62,0x91,0x91,0x91,0x4e},//S
{0x80,0x80,0xff,0x80,0x80},//T
{0xfe,0x01,0x01,0x01,0xfe},//U
{0xfc,0x02,0x01,0x02,0xfc},//V
{0xff,0x02,0x04,0x02,0xff},//W
{0xc3,0x24,0x18,0x24,0xc3},//X
{0xc0,0x20,0x1f,0x20,0xc0},//Y
{0x83,0x85,0x89,0x91,0xe1},//Z
};



//-----------------------------
main()
{
DDRA=0XFF;
DDRC=0XFF;
DDRD=0xff;
count=0;
text_ptr=1;
time_ptr=1;
RTC_init();
USARTInit(575); 
unsigned char i,j,m;
for(i=0;i<20;i++)
disptext(msg[i]);

loadtime();

while(1)                                                                                                                      
{

sbi(PORTC,PC0);//set data

if(RTC_second<30)
{
//show time 
scan_time();
}

else
//show text
scan_text();

// time format : HH-MM DD/MM/YY day

if(RTC_minutechanged)
{
loadtime();
RTC_minutechanged=0;
}

}}
//main ends



/*

void disptext1(uint8_t address,uint8_t m)  //(address,data)
{
uint8_t j;//,k,k_t;

text_ptr=address * 5;

for(j=0;j<5;j++)
{
text[text_ptr++]=array[m][j];
if(text_ptr>199)
text_ptr=1;
}
}
*/

//------------under edit------------------------
//stores string on to temp which wil be displayed
void disptext(uint8_t m)
{
uint8_t j;//,k,k_t;
for(j=0;j<5;j++)
{
text[text_ptr++]=array[m][j];
if(text_ptr>199)
text_ptr=1;
}
}



void scan_text()
{
uint8_t i;
for(i=count;i<=count+150;i++)
{

PORTA=text[i];
sbi(PORTC,PC1);
_delay_ms(5);
cbi(PORTC,PC1);

cbi(PORTC,PC0);

}

}

//-------------to test-----------------------------------------------------------------

void scan_time()
{
uint8_t i;
for(i=count;i<=count+80;i++)
{
PORTA=time[i];
sbi(PORTC,PC1);
_delay_ms(5);
cbi(PORTC,PC1);

cbi(PORTC,PC0);


}
}

void disptime(uint8_t m)
{
uint8_t j;
for(j=0;j<5;j++)
{

time[time_ptr++]=array[m][j];
}
if(time_ptr>99)
time_ptr=0;

}



void loadtime()
{
// format : HH-MM DD/MM/YY day
//count=0;
time_ptr=1;
getascii(RTC_hour);
disptime(ascii_msb);
disptime(ascii_lsb);
disptime('-');
getascii(RTC_minute);
disptime(ascii_msb);
disptime(ascii_lsb);
disptime(' ');

getascii(RTC_date);
disptime(ascii_msb);
disptime(ascii_lsb);
disptime('/');
getascii(RTC_month);
disptime(ascii_msb);
disptime(ascii_lsb);
disptime('/');
getascii(RTC_year);
disptime(ascii_msb);
disptime(ascii_lsb);
//disptime('/');
}


//dont alter below---------------------------------------------------------------

void RTC_init(void)
{
	uint8_t sreg;/* Save status register and disable global interrupts */
	sreg=SREG;
	cli();	/* 0. Oscillator might take as long as one second to stabilize. */
	_delay_ms(1000);	/* 1. Disable the Timer/Counter2 interrupts by clearing OCIE2 and TOIE2. */
	TIMSK &= ~((1<<OCIE2)|(1<<TOIE2));	/* 2. Select clock source by setting AS2 as appropriate. */
	ASSR = (1<<AS2);	/* 3. Write new values to TCNT2, OCR2, and TCCR2. */
	TCNT2 = 0;	// clear TCNT2A	// select precaler: 32.768 kHz / 128 = 1 sec between each overflow:
	TCCR2 |= (1<<CS20)|(1<<CS22); /*| (1<<CS20)|(1<<CS2);	 4. To switch to asynchronous operation: Wait for TCN2UB, OCR2UB, and
	TCR2UB. */
	while( (ASSR & (1<<TCN2UB)) | (ASSR & (1<<OCR2UB)) |
		(ASSR & (1<<TCR2UB)) ); 
	
	/* 5. Clear the Timer/Counter2 Interrupt Flags. */
	TIFR |= ((1<<OCF2)|(1<<TOV2));
	
	/* 6. enable Timer2 Overflow interrupt */
	TIMSK |= (1<<TOIE2);
    
	/* initial values */
	RTC_hour=0;
	RTC_minute=15;
	RTC_second=50;
        RTC_date_max=31;
	RTC_minutechanged = 1;
	
ascii_msb=0;
ascii_lsb=0;

	/* restore status-register */
	SREG=sreg;
	sei();
}
//------------timer2 rtc isr-begin--------------------------------

//sound ends
ISR(TIMER2_OVF_vect)
{ 
PORTD^=0x40;//blink led
//disp begin
// disp_time();

count+=5;
//if(count>40)
if(count>((time_ptr/5)-8))
count=0;


//disp end
RTC_second++;

 if (RTC_second==60)        //keep track of time, date, month, and year
    {
 	       RTC_second=0;
		   	RTC_minute++;
			RTC_minutechanged=1;

	}
 if (RTC_minute==60) 
    {
            RTC_minute=0;
			RTC_hour++;
	}
 if (RTC_hour>=24)
    {
                RTC_hour=0;
				RTC_date++; 
				update_date_max();
	}
	
	
	if(RTC_date==1)



if(RTC_date > RTC_date_max)
   {
   RTC_date=1;
   RTC_month++;	 };

if (RTC_month > 12)

   {
   RTC_month=1;
   RTC_year++;
   }

}
//------------timer2 rtc isr-ends--------------------------------

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
//---------uart begin----------
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
//while(!(UCSRA & (1 << UDRE)));
//UDR = 0x43; // "C"
//	while(!(UCSRA & (1 << UDRE)));
//	UDR = 0x0d;

}
void sound(unsigned char x)
{
DDRD=0x80;
unsigned char i;
while(x--){
for(i=0;i<25;i++)
{
//scan_display();

cbi(PORTD,PD7);
_delay_us(500);
sbi(PORTD,PD7);
_delay_us(1000);
}}
DDRD=0x00;
return 0;
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

//uart ends
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


SIGNAL (SIG_UART_RECV) { // USART RX interrupt
	unsigned char c;
	static unsigned char a,b;
	c = UDR;


if(eprm==0)
{
b=text_ptr;

a=c;
}		
if((a=='$'))// &&(b=='X'))
{

//b=text_ptr;
		disptext('R');
		disptext('X');

	eeprom_write_byte(eprm++,c);
//	buff[eprm]=c;
	//disp(0xc0,c);
	if(eprm==0x1ff)
	eprm=0;

}

else if((a=='!'))// &&(b=='X'))
{
unsigned int k;
for(k=0;k<512;k++)
USARTWriteChar(eeprom_read_byte(k));
text_ptr=b;

		disptext('T');
		disptext('X');

}



//if((a=='~'))//quit

{
text_ptr=b;
		disptext(' ');
		disptext(' ');
		disptext(' ');
		disptext(' ');
		disptext(' ');
		disptext(' ');
		disptext(' ');
		disptext(' ');
		eprm=0;

}


}

