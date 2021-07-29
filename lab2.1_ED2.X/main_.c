/*------------------------------------------------------------------------------
Archivo: mainsproject.s
Microcontrolador: PIC16F887
Autor: Andy Bonilla
Compilador: pic-as (v2.30), MPLABX v5.45
    
Programa: laboratorio 2
Hardware: PIC16F887
    
Creado: 23 de julio de 2021    
Descripcion: un laboratoria bien fumado tbh pero chilero
------------------------------------------------------------------------------*/

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT   //configuracion de oscilador interno
#pragma config WDTE = OFF       // Watchdog Timer Enable bit
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // master clear off 
#pragma config CP = OFF         // Code Protection bit off
#pragma config CPD = OFF        // Data Code Protection bit off
#pragma config BOREN = OFF      // Brown Out Reset Selection bits off
#pragma config IESO = OFF       // Internal External Switchover bit off
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit off 
#pragma config LVP = OFF        // Low Voltage Programming Enable bit off 

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit 
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 

/*-----------------------------------------------------------------------------
 ----------------------------LIBRERIAS-----------------------------------------
 -----------------------------------------------------------------------------*/
#include <xc.h>                 //se incluye libreria del compilador
#include <stdint.h>             //se incluye libreria
#include <pic16f887.h>          //se incluye libreria del pic
#include "LCD.h"                //se incluye libreria de LCD
#include "ADC_CONFIG.h"         //se incluye libreria del adc
#include "UART_CONFIG.h"        //se incluye libreria de uart

/*-----------------------------------------------------------------------------
 ------------------------DIRECTIVAS DE COMPILADOR------------------------------
 -----------------------------------------------------------------------------*/
#define _XTAL_FREQ 4000000

/*-----------------------------------------------------------------------------
 ------------------------ PROTOTIPOS DE FUNCIONES ------------------------------
 -----------------------------------------------------------------------------*/
void setup(void);
void toggle_adc(void);                      //toggle de canales de adc
unsigned char datos_ascii(uint8_t numero);  //conversion de numeros a ascii
uint8_t lcd_ascii();                        //cadena de caracteres para lcd

/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/
unsigned char conversion1;          //variable de potenciometro1
unsigned char conversion2;          //variable de potenciometro2
unsigned char cuenta_uart;          //variable de cuenta en uart

/*-----------------------------------------------------------------------------
 ---------------------------- INTERRUPCIONES ----------------------------------
 -----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 ----------------------------- MAIN LOOP --------------------------------------
 -----------------------------------------------------------------------------*/
void main(void)
{
    setup();
    /*lcd_init();         //invoco la funcion de inicializacion de la lcd
	cmd(0x90);          //invocao la funcion de configurcion de comandos lc*/
    Lcd_Init();
    Lcd_Cmd(0x8A);
    
    while(1)
    {
        //------
        toggle_adc();
        //------
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("S1    S2    S3");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(lcd_ascii());
        /*lcd_linea(1,1);             //selecciono la linea 1 para escribir
        show(" S1   S2   S3 ");//mensaje a enviar linea 1
        lcd_linea(2,1);             //selecciono la linea 2 para escibrir
        show(lcd_ascii()); //mensaje a enviar linea 2*/
    }

}
/*-----------------------------------------------------------------------------
 ---------------------------------- SET UP -----------------------------------
 -----------------------------------------------------------------------------*/
void setup(void)
{
    //---------CONFIGURACION DE ENTRADAS ANALOGICAS
    ANSEL=0;                //solo se limpian entradas analogicas
    ANSELH=0;               //solo se limpian entradas analogicas
    ANSELbits.ANS0=1;       //entrada para el potenciometro 1
    ANSELbits.ANS1=1;       //entrada para el potenciometro 2
    
    //---------CONFIGURACION DE IN/OUT
    TRISB=0;             //todo el portB como salida
    TRISD=0;             //todo el portB como salida
    /*TRISDbits.TRISD5=0;     //salida para pines lcd
    TRISDbits.TRISD6=0;     //salida para pines lcd
    TRISDbits.TRISD7=0;     //salida para pines lcd*/
    //---------LIMPIEZA DE PUERTOS
    PORTB=0;
    PORTD=0;
    
    //---------CONFIGURACION DE OSCILADOR
    OSCCONbits.IRCF=0b110;      //Freq a 8MHz
    OSCCONbits.SCS=1;           //Oscilador interno
    
    //---------LLAMADO DE FUNCION DE CONFIGURACION DE ADC
    adc_config();
    
    //---------LLAMADO DE FUNCION DE CONFIGURACION DE UART
    
    //---------CONFIGURACIOND DE INTERRUPCIONES
    INTCONbits.GIE=1;           //se habilita interrupciones globales
    
    
    
}
/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/

//--------FUNCION PARA SWITCHEO DE CANALES **se le puede poner/quitar canales
void toggle_adc(void)
{
    if (ADCON0bits.GO==0)
    {
        switch(ADCON0bits.CHS)
        {
            /*default:
                conversion1=ADRESH;         //potenciometro 1
                __delay_us(100);            //delay para cargar capacitor          
                ADCON0bits.CHS=1;           //switch de canal
                break;*/
                
            case(0):
                conversion1=ADRESH;         //potenciometro 1
                __delay_us(100);            //delay para cargar capacitor          
                ADCON0bits.CHS=1;           //switch de canal
                ADCON0bits.GO=1;            //se inicia otra conversion ADC
                break;
                    
            case(1):
                conversion2=ADRESH;         //potenciometro 2
                __delay_us(100);            //delay para cargar capacitor
                ADCON0bits.CHS=0;           //switch de canal
                ADCON0bits.GO=1;            //se inicia otra conversion ADC
                break;
            
            __delay_us(100);                //delay para carga de capacitor
            ADCON0bits.GO=1;                //se inicia otra conversion ADC
        }
    }
}


//--------funcion para conversion de valores ascii
unsigned char datos_ascii(uint8_t numero)    //funcion para convertir a valores ascii
{
    switch(numero)
    {
        default:
            return 0x30;        //retorna 0 en ascii
            break;
        case(0):
            return 0x30;        //retorna 0 en ascii
            break;
            
        case(1):
            return 0x31;        //retorna 1 en ascii
            break;
            
        case(2):
            return 0x32;        //retorna 2 en ascii
            break;
            
        case(3):
            return 0x33;        //retorna 3 en ascii
            break;
            
        case(4):
            return 0x34;        //retorna 4 en ascii
            break;
            
        case(5):
            return 0x35;        //retorna 5 en ascii
            break;
            
        case(6):
            return 0x36;        //retorna 6 en ascii
            break;
            
        case(7):
            return 0x37;        //retorna 7 en ascii
            break;
            
        case(8):
            return 0x38;        //retorna 8 en ascii
            break;
            
        case(9):
            return 0x39;        //retorna 9 en ascii
            break;
            
    }
    
}

//--------funcion de cadena de caracteres para lcd
uint8_t lcd_ascii()
{
    uint8_t random[16];                    //se usa set de 16bits
    random[0]=datos_ascii(((2*(conversion1)/100)%10));   //centenas de potenciometro 1
    random[1]=0x2E;                     //punto decimal
    random[2]=datos_ascii(((2*(conversion1)/100)%10));    //decenas de potenciometro 1
    random[3]=datos_ascii((2*conversion1)%10);   //unidades de potenciometro 1
    random[4]=32;                       //se deja espacio 
    random[5]=datos_ascii(((2*(conversion2)/100)%10));   //centenas de potenciometro 2
    random[6]=0x2E;                     //punto decimal
    random[7]=datos_ascii(((2*(conversion2)/100)%10));    //decenas de potenciometro 2
    random[8]=datos_ascii((2*conversion2)%10);   //unidades de potenciometro 2
    random[9]=32;                       //se deja espacio
    random[10]=datos_ascii(cuenta_uart);  //centenas de cuenta uart
    random[11]=0x2E;                    //punto decimal
    random[12]=datos_ascii(cuenta_uart);  //centenas de cuenta uart
    random[13]=datos_ascii(cuenta_uart);  //centenas de cuenta uart
    random[14]=32;                      //se deja espacio
    random[15]=32;                      //se deja espacio
    return random;                      //se retorna el valor para el lcd
}
