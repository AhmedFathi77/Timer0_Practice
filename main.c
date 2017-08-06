
/*
 * main.c
 *
 *  Created on: Aug 5, 2017
 *      Author: leenovoz510
 */
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define RS 5
#define RW 6
#define EN 7

void send_command(char command);
void send_data(char data);
void clear_screan();
void Print_On_Screen(char *str);
void Go_to(char x ,char y);
void LCD_init();

volatile uint8_t tot_overflow;



// TIMER0 overflow interrupt service routine
// called whenever TCNT0 overflows
ISR(TIMER0_OVF_vect)
{
    tot_overflow++;
}

void main(void){
	 DDRC = 0xff;
	 DDRD = 0xff;
	 DDRB = 0xff;

	 _delay_ms(10);
	 LCD_init();
	 _delay_ms(10);
	 clear_screan();

	 TIMER0_Init();
int cou = 0 ;
	while(1){
		 char counter[5] ;
		Go_to(1,1);
		Print_On_Screen("Timer : ");
		itoa(cou, counter, 10);
		Go_to(1,9);
	    Print_On_Screen(counter);



		 // check if no. of overflows = 12
		        if (tot_overflow >= 62)  // NOTE: '>=' is used
		        {
		            // check if the timer count reaches 53
		            if (TCNT0 >= 53)
		            {

		                PORTB ^= (1 << 0)|(1<<1);    // toggles the led
		                TCNT0 = 0;            // reset counter
		                tot_overflow = 0;     // reset overflow counter
		                cou++;
		            }
		        }
	}
}

void TIMER0_Init(){
	 // set up timer with prescaler = 256
	    TCCR0 |= (1 << CS02)|(1<<CS00);

	    // initialize counter
	    TCNT0 = 0;

	    // enable overflow interrupt
	    TIMSK |= (1 << TOIE0);

	    // enable global interrupts
	    sei();

	    // initialize overflow counter variable
	    tot_overflow = 0;

}

void send_command(char commnd){
	PORTC = commnd;
	PORTD &=~ (1<<RS);
	PORTD &=~(1<<RW);
	PORTD |= (1<<EN);
	_delay_ms(50);
	PORTD &=~(1<<EN);
}
void send_data(char data){
	PORTC = data;
	PORTD |= (1<<RS);
	PORTD &= ~(1<<RW);

	PORTD |= (1<<EN);
	_delay_ms(50);
	PORTD &=~(1<<EN);
}
void clear_screan(){
	send_command(0x01);
	_delay_ms(10);
}
void Print_On_Screen(char *str){
	int i=0;
	while(str[i] != 0){
		send_data(str[i]);
		i++;
	}
}
void Go_to(char x ,char y){
	char firstAddress[] = {0x80,0xC0,0x94,0xD4};
	send_command(firstAddress[y-1] + x-1);
		_delay_ms(10);
}
void LCD_init(){
	send_command(0x38);  // 8-bit mode
	send_command(0x0C);
	send_command(0x06);
	send_command(0x80);
	_delay_ms(10);
}


