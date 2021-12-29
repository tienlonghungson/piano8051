#include<at89x51.h>
char uart_data;

void main (void)
{
  SCON = 0x50; 			/* uart in mode 1 (8 bit), REN=1 */
  TMOD = TMOD | 0x20 ; 	/* Timer 1 in mode 2 */
  TH1 = 0xFD; 			/* 9600 Bds at 11.0592MHz */
  TL1 = 0xFD; 			/* 9600 Bds at 11.0592MHz */
  ES = 1; 				/* Enable serial interrupt*/
  EA = 1; 				/* Enable global interrupt */
  TR1 = 1; 				/* Timer 1 run */
  while(1); 			/* endless */
}

void serial_IT(void) interrupt SIO_VECTOR // 4
{
  if (RI == 1)
  { 
    RI = 0; 			/* prepare for next reception */
    uart_data = SBUF; 	/* Read receive data */
    //SBUF = uart_data; 	/* Send back same data on uart*/
		SBUF = 'L';
  }
  else 
    TI = 0; 			/* if emission occur */
}
