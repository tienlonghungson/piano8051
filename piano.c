#include <at89x51.h>
#include "string.h"

/******************* LCD API*********************/
#define LCD_DATA P2
sbit EN=P0^2;
sbit RS=P0^0;
sbit RW=P0^1;	   
//Khai bao prototype cho cac ham
void Init_System();	
void Delay_ms(int interval);  
void LCD_init();
void Wait_For_LCD();
void LCD_Send_Command(unsigned char x);
void LCD_Write_One_Char(unsigned char c);
void LCD_Write_String(unsigned char *s);
/************************************************/

/**************** Chords API ********************/
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

unsigned char matched_chords[]={
	'a','s','d','f','g','h','j','k','l','t','y'
};

unsigned char *chord_names[]={
	"C4","D4","E4","F4","G4","A4","B4","C5","D5","E5","F5"
};

void init();
void delay(unsigned int time);
void play_sound();


void TMR0_Handler() interrupt  TF0_VECTOR{
	TH0=chords[chord_idx];
	TL0=chords[chord_idx+1];
	speaker=~speaker;
}		


/************************************************/

/****************** UART API*********************/
char uart_data;
void serial_IT(void) interrupt SIO_VECTOR{
  if (RI == 1){ 
    RI = 0; 			/* prepare for next reception */
    uart_data = SBUF; 	/* Read receive data */
  }
  else 
    TI = 0; 			/* if emission occur */
}
/************************************************/

/******************** MAIN **********************/
void main(void){
	unsigned char i;
	char last_chord='p';
	init();
	Init_System();
	
	// music intro
	for(i = 0; i < 22;i+=2){
		chord_idx=i;
		play_sound();
		delay(50000);
	}
	
	TR0=0;
	speaker=0;
	
	// screen intro
	LCD_init();
	LCD_Write_String("TN CNTT K63");
	LCD_Send_Command(0xC0); //Chuyen con tro xuong dong thu 2
	LCD_Write_String("**************");
	
	while(1){
		for(i=0;i<11;++i){
			if (uart_data==matched_chords[i]){
				chord_idx=(i<<1);
				TH0=chords[chord_idx];
				TL0=chords[chord_idx+1];
				if (uart_data!=last_chord){
					TR0=0;
					speaker=0;
				}
				play_sound();
				LCD_Write_String(chord_names[i]);
				break;
			}
		}

		if (uart_data=='q'){
			TR0 = 0;
			speaker=0;          // turn off speaker
		}

		
		LCD_init();
		last_chord = uart_data;
	}			
}

/************************************************/

/************* implement LCD API ****************/

void Init_System(){
	//Thiet lap LCD o che do doc
	RW=1;	
}
void Delay_ms(int interval){
	int i,j;
	for(i=0;i<1000;i++){
		for(j=0;j<interval;j++);
	}
}
//Ham thuc hien gui mot lenh xuong LCD
void LCD_Send_Command(unsigned char x){
	LCD_DATA=x;
	RS=0; //Chon thanh ghi lenh
	RW=0; //De ghi du lieu
	EN=1;
	Delay_ms(1);
	EN=0;		
	Wait_For_LCD(); //Doi cho LCD san sang
	EN=1;		  
}
//Ham kiem tra va cho den khi LCD san sang
void Wait_For_LCD(){
	Delay_ms(2);
}
void LCD_init(){
	LCD_Send_Command(0x38); //Chon che do 8 bit, 2 hang cho LCD
	LCD_Send_Command(0x0E); //Bat hien thi, nhap nhay con tro	
	LCD_Send_Command(0x01); //Xoa man hinh	
	LCD_Send_Command(0x80); //Ve dau dong
	
}
//Ham de LCD hien thi mot ky tu
void LCD_Write_One_Char(unsigned char c){
	LCD_DATA=c; //Dua du lieu vao thanh ghi 
	RS=1; //Chon thanh ghi du lieu
	RW=0;
	EN=1;
	Delay_ms(1);	
	EN=0;
	Wait_For_LCD();	
	EN=1;
}
//Ham de LCD hien thi mot xau
void LCD_Write_String(unsigned char *s){
	unsigned char length;
	length=strlen(s); //Lay do dai xau
	while(length!=0){
		LCD_Write_One_Char(*s); //Ghi ra LCD gia tri duoc tro boi con tro
		s++; //Tang con tro
		length--;	  		 
	}
}
/************************************************/

/************* implement Chords API *************/
void init(){
	SCON = 0x50; 			/* uart in mode 1 (8 bit), REN=1 */
	ES = 1; 				/* Enable serial interrupt*/
  	EA = 1; 				/* Enable global interrupt */

	// TMOD=TMOD & 0xF0;
	TMOD=TMOD | 0x01; /* Timer 0 in mode 1 */
	TMOD = TMOD | 0x20 ; 	/* Timer 1 in mode 2 */

	ET0=1; // cho phep ngat timer 0
	TR0=0; // stop timer 0
	
  	TH1 = 0xFD; 			/* 9600 Bds at 11.0592MHz */
  	TL1 = 0xFD; 			/* 9600 Bds at 11.0592MHz */
  	
  	TR1 = 1; 				/* Timer 1 run */
}
void delay(unsigned int time){
	while(time--);
}

void play_sound(){
	TR0=1; // start timer 0
	// delay(5000);
	// TR0=0; // stop timer 
	// speaker=1 ;
	// delay(30000);
}

/************************************************/