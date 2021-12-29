#include <at89x51.h>

sbit speaker  = P3^7;	
unsigned int chord_idx;


code unsigned int chords[]={
    0xf9, 0x1f,		// C4
		0xf9, 0xdf,		// D4
		0xfa, 0x8b,		// E4
		0xfa, 0xd9,		// F4
		0xfb, 0x69,		// G4
		0xfb, 0xe9,		// A4
		0xfc, 0x5b,		// B4
		0xfc, 0x90,		// C5
		0xfc, 0xf0,		// D5
		0xfd, 0x46,		// E5
		0xfd, 0x6d		// F5
};	

void init(){
	TMOD=0x01;	
	EA=1; // global interrupt enable
	ET0=1; // cho phep ngat timer 0
	TR0=0; // stop timer 0
}

void TMR0_Handler() interrupt  TF0_VECTOR{
	TH0=chords[chord_idx];
	TL0=chords[chord_idx+1];
	speaker=~speaker;
}		
void delay(unsigned int time){
	while(time--);
}

void play_sound(){
	TR0=1; // start timer 0
	delay(50000);
	TR0=0; // stop timer 
	speaker=1 ;
	delay(30000);
}

void main(void) 
{
	unsigned char i;
	init();
	while(1){
		chord_idx=0;
		for(i=0;i<11;i++){				
			play_sound();
			chord_idx+=2;			
		}
		delay(100000);
	}			
}




