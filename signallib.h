/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   signallib.h
 * Author: W.TORII
 * Comments:
 * Revision history: v0.1
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

//#include <xc.h>

#define AQUOS_LEN 6
#define CEILING_LEN 5

char AQUOS[30][AQUOS_LEN] = {
                                {0xaa,0x5a,0x8f,0x12,0x16,0xd1},    //on-off    0
                                {0xaa,0x5a,0x8f,0x12,0x4e,0x32},    //1ch       1
                                {0xaa,0x5a,0x8f,0x12,0x4f,0x22},    //2ch       2
                                {0xaa,0x5a,0x8f,0x12,0x50,0xc2},    //3ch       3
                                {0xaa,0x5a,0x8f,0x12,0x51,0xd2},    //4ch       4
                                {0xaa,0x5a,0x8f,0x12,0x52,0xe2},    //5ch       5
                                {0xaa,0x5a,0x8f,0x12,0x53,0xf2},    //6ch       6
                                {0xaa,0x5a,0x8f,0x12,0x54,0x82},    //7ch       7
                                {0xaa,0x5a,0x8f,0x12,0x55,0x92},    //8ch       8
                                {0xaa,0x5a,0x8f,0x12,0x56,0xa2},    //9ch       9
                                {0xaa,0x5a,0x8f,0x12,0x57,0xb2},    //10ch      10
                                {0xaa,0x5a,0x8f,0x12,0x58,0x42},    //11ch      11
                                {0xaa,0x5a,0x8f,0x12,0x59,0x52},    //12ch      12
                                {0xaa,0x5a,0x8f,0x12,0x11,0xa1},    //ch-up     13
                                {0xaa,0x5a,0x8f,0x12,0x12,0x91},    //ch-down   14
                                {0xaa,0x5a,0x8f,0x12,0x14,0xf1},    //vol-up    15
                                {0xaa,0x5a,0x8f,0x12,0x15,0xe1},    //vol-down  16
                                {0xaa,0x5a,0x8f,0x12,0x5e,0x22},    //d-button  17
                                {0xaa,0x5a,0x8f,0x12,0x80,0x12},    //bule      18
                                {0xaa,0x5a,0x8f,0x12,0x81,0x02},    //red       19
                                {0xaa,0x5a,0x8f,0x12,0x82,0x32},    //green     20
                                {0xaa,0x5a,0x8f,0x12,0x83,0x22},    //yellow    21
};   //2ch
char CEILING[10][CEILING_LEN] = {   {0x01, 0xd0,0x0c, 0x11, 0x01},
                //1000 0000 0000 1011 0011 0000 1000 1000 1000 0000},     //onoff
                                    {0xaa,0x5a,0x8f,0x12,0x02}};

#endif	/* XC_HEADER_TEMPLATE_H */

