/*
 * File:   newmain.c
 * Author: Juan Pablo
 *
 * Created on 24 de octubre de 2020, 12:27 AM
 */

 #include <xc.h>
#include<stdio.h>
#include <stdlib.h>
#define _XTAL_FREQ 8000000
// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
//#define MAX_STRLEN 14
unsigned char entrada;
unsigned char data;
//char received_string[MAX_STRLEN+1];
int cont=0;
int estado=0;                //estado actual de las luces
void spi_init(void){
    // disable MSSP
    SSPCONbits.SSPEN = 0;    //poder configurar pines como entrada/salida
    // SDI1 set
    TRISCbits.TRISC4 = 1; // configure RC4 as input
    // SS1 set
    TRISAbits.TRISA5 = 1;
    // SDO1 cleared
    // SCK1 cleared
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC3 = 0;

    // SPI mode 0
    SSPCONbits.CKP = 0; // Idle state for clock is a low level
    SSPSTATbits.CKE = 1; // La transmisión ocurre en la transición del estado de reloj activo a inactivo
    SSPSTATbits.SMP = 1; // Datos de entrada muestreados al final del tiempo de salida de datos
    SSPCONbits.SSPM = 0; // SPI Master mode, SCK = FOSC/4 frecuencis del reloj
    SSPCONbits.SSPEN = 1; // enable MSSP1
}

void spi_fast_shift(){
    SSPBUF = data;
    while( !PIR1bits.SSPIF ); // wait for buffer full
    PIR1bits.SSPIF = 0; // clear SSP1IF

}
void main(){
    OSCCONbits.IRCF=0b111;
    OSCCONbits.SCS=0b00;
    ANSEL=0B00000000;
    ANSELH=0B00000000;
    TRISB=0b11111111;
    TRISD=0b00000000;
    TRISA=0b00000011;
    PORTB=0b00000000;
    PORTD=0b00000000;
    OPTION_REGbits.nRBPU=0;
    WPUBbits.WPUB=1;
    OPTION_REGbits.INTEDG=1;
    // set up INT0
   // INTCONbits.INTE = 1; // enable INT2 interrupt

    // Initialize spi module
    spi_init();
  
    INTCONbits.PEIE = 1; // peripheral interrupts enabled
    INTCONbits.GIE = 1;  // global interrupt enable

    while(1){
 // __delay_ms(100);
  while(cont==1){
    data='a';  
    spi_fast_shift();
  PORTDbits.RD1=1;
  estado=1;
  if(PORTBbits.RB6==0){
      cont=0;
  }
  }
    while(cont==0){
    data='b';  
    spi_fast_shift();    
    PORTDbits.RD1=0;
    estado=0;
    if(PORTBbits.RB7==0){
        cont=1;
    }
    
    }

}
}
/*void __interrupt(void) ISR(void){
    // external interrupt IRQ pin of NRF24L01
    if(INTCONbits.INTF ){
	}
        // reset INT2 flag
        INTCONbits.INTF = 0;
    }*/
