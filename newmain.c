/*
 * File:   newmain.c
 * Author: W.TORII
 * Target : PIC16F1705
 * Created on 2020/03/04, 7:30
 */

#include <xc.h>
#include <string.h>
#include "signallib.h"
#define _XTAL_FREQ 32000000    //  delay用に必要(クロック8MHzを指定)
#define HIGH 1
#define LOW 0
#define IR PORTAbits.RA2        //IR-LED (940nm))
#define RecIR PORTAbits.RA5     //photo transistor
#define SW  PORTAbits.RA4       //tact switch
#define WAKE_HW PORTCbits.RC2
#define WAKE_SW PORTCbits.RC3

#define BUF_N 64

// コンフィギュレーション１の設定
#pragma config FOSC     = INTOSC   // 内部ｸﾛｯｸ使用する(INTOSC)
#pragma config WDTE     = OFF      // ｳｵｯﾁﾄﾞｯｸﾞﾀｲﾏｰ無し(OFF)
#pragma config PWRTE    = ON       // 電源ONから64ms後にﾌﾟﾛｸﾞﾗﾑを開始する(ON)
#pragma config MCLRE    = OFF      // 外部ﾘｾｯﾄ信号は使用せずにﾃﾞｼﾞﾀﾙ入力(RA3)ﾋﾟﾝとする(OFF)
#pragma config CP       = OFF      // ﾌﾟﾛｸﾞﾗﾑﾒﾓﾘｰを保護しない(OFF)
#pragma config BOREN    = ON       // 電源電圧降下常時監視機能ON(ON)
#pragma config CLKOUTEN = OFF      // CLKOUTﾋﾟﾝをRA4ﾋﾟﾝで使用する(OFF)
#pragma config IESO     = OFF      // 外部・内部ｸﾛｯｸの切替えでの起動はなし(OFF)
#pragma config FCMEN    = OFF      // 外部ｸﾛｯｸ監視しない(OFF)
// コンフィギュレーション２の設定
#pragma config WRT      = OFF      // Flashﾒﾓﾘｰを保護しない(OFF)
#pragma config PPS1WAY  = OFF      // ロック解除シーケンスを実行すれば何度でもPPSLOCKをセット/クリアできる
#pragma config ZCDDIS   = ON       // ゼロクロス検出回路は無効とする(ON)
#pragma config PLLEN    = ON      // 動作クロックを32MHzでは動作させる(ON)
#pragma config STVREN   = ON       // スタックがオーバフローやアンダーフローしたらリセットをする(ON)
#pragma config BORV     = HI       // 電源電圧降下常時監視電圧(2.5V)設定(HI)
#pragma config LPBOR    = OFF      // 低消費電力ブラウンアウトリセット(LPBOR_OFF)
#pragma config LVP      = ON      // 低電圧プログラミング機能使用(ON)

char uartBuf[BUF_N];
char uartCharCnt = 0;
char waitingMode = 0;

void SendText_UART(char* data, char n)
{
    for(char i = 0; i < n; i++)
    {
        while(!TXIF);
        TX1REG = data[i];
    }
    while(!TXIF);
    TX1REG = '\n';
    __delay_ms(10);
}

//1T is about 0.4ms
void Output1T()
{
    //pulse width = 26.2us when 38.2kHz modulation
    const int repeatN = 400/26.2; //(400us / (2*pulse width))
    for(char i = 0; i < repeatN; i++)
    {
        IR = HIGH;
        __delay_us(20);
        IR = LOW;
    }
}

void Output1()
{
    Output1T();
    __delay_us(1200);
}

void Output0()
{
    Output1T();
    __delay_us(400);
}
void SendIRCommand(char* data, int n)
{
    //Reader
    for(int i = 0; i < 8; i++)
        Output1T();
    __delay_us(1600);
    
    //data
    for(int i = 0; i < n; i++)
    {
        char c = data[i];
        for(int j = 0; j < 8; j++)
        {
            if (0x01 & c)
                Output1();
            else
                Output0();
            c = (c >> 1);
        }
    }
    
    //Traller
    Output1();
}

void SelectCommand(char data)
{
    if(data >> 6 == 0x00)
        SendIRCommand(AQUOS[0], AQUOS_LEN);
    if(data >> 6 == 0x01)
        SendIRCommand(CEILING[data & 0x3f], CEILING_LEN);
}

void Initialize()
{
    OSCCON = 0b01110000;    //internal clock = 32MHz (4x8MHz)
    
    ANSELA = 0x00;  //none of analog input
    ANSELC = 0x00;  //none of analog input
    TRISA = 0x00;   //all pin as output 
    TRISC = 0x20;   //all pin as output except RC5
    PORTA = 0x00;   //initial out is LOW
    PORTC = 0x00;   //initial out is LOW
    
    
    //UART ::  TX:RC4  RX;RC5
    RC4PPS = 0b10100;       //RC4 as TX
    RXPPS = 0b10101;        //RC5 as RX
    TX1STA = 0b00100100;
    RC1STA = 0b10010000;
    SPBRG = 16;     //baudrate = 115200 (actual rate = 117.64k)
    
    //inturruption
    RCIF = 0;       //interrupt by serial RX
    RCIE = 1;       //interrupt by serial rx
    PEIE = 1;
    GIE = 1;
    
    __delay_ms(4000);
}

void interrupt isr()
{
    if(PIR1bits.RCIF)
    {
        if(waitingMode) //on MLDP mode
        {
            char d = RCREG;
            SelectCommand(d);
        }
        else if(RCREG != '\n')
        {
            uartBuf[uartCharCnt] = RCREG;
            uartCharCnt++;
        }
        else
            uartCharCnt = 0;
        PIR1bits.RCIF = 0;
    }
}

char IsEqual(char* a, char* b, char n)
{
    for(int i = 0; i < n; i++)
        if(a[i] != b[i])
            return 0;
    return 1;
}

void SetRN4020()
{
    WAKE_HW = HIGH;
    __delay_ms(100);
    WAKE_SW = HIGH; // RN4020 send the text, "CMD", through UTX line
    __delay_ms(10);    

    SendText_UART("PZ", 2);
    SendText_UART("SN,NightHeron", 13);     //device name
    SendText_UART("SDM,NightHeron", 14);    //Model-Characteristic name
    SendText_UART("SDN,WTORII", 10);        //manufacture name
    SendText_UART("SS,00000001", 11);       //private profile
    SendText_UART("SR,36000000", 11);
    SendText_UART("R,1", 3);
    SendText_UART("PS,cd3152d04ddb86610f4d7fd794da92f9",35);
    SendText_UART("PC,cd3162d04ddb86610f4d7fd794da92f9,42,05", 41);
    
    SendText_UART("R,1", 3);    
}

void main(void) {
    Initialize();
    SetRN4020();
    
    while(1)
    {
        __delay_ms(2000);
        if(IsEqual(uartBuf, "CMD", 3) || IsEqual(uartBuf, "ERR", 3))
        {
            ;//do nothing
        }
        else if(IsEqual(uartBuf, "Connected", 9))   //for test
        {
            SendText_UART("N,test", 6);
            SendText_UART("A", 1);
        }
        else if(IsEqual(uartBuf, "MLDP",4))
        {
            waitingMode = 1;
            //SendText_UART("ready",5);
        }
    }
    return;
}
