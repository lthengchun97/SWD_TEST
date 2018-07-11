#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdint.h>
#include "struct.h"

uint32_t SW_CLOCK,SW_IO;

#define TRUE 	  1
#define FALSE 	0

#define SW_EQ_IDCODE			0x3BA00477
#define	SW_EQ_CODE				(0xe79e)
#define SW_IDCODE_RD      0xA5
#define SW_STATUS_FLG			0xAD

#define START_EN				(1)
#define AP_EN					(1<<1)
#define	DP_EN					(0<<1)
#define WR_EN					(0<<2)
#define	RD_EN					(1<<2)
#define AddrField(x)			(x<<3)
#define	Odd_Parity				(0<<5)
#define	Even_Parity				(1<<5)
#define	STOP_BIT				(0<<6)
#define	PARK_BIT				(1<<7)

#define SW_ACK_OK               0x1
#define SW_ACK_WAIT             0x2
#define SW_ACK_FAULT            0x4
#define SW_ACK_PARITY_ERR       0x8


#define SWDCLK_Pin			1
#define	SWDIO_Pin				0

#define INPUT_MODE			0
#define	OUTPUT_MODE			1
#define OPENDRAIN_MODE	2

#define CLOCK_SPD				100

//Configure pin as output/input/open drain





int SW_CLK_L();
int SW_CLK_H();
uint32_t SW_IO_L();
uint32_t SW_IO_H();
void swdLineReset();
void idleCycles(int time);
uint32_t swdWriteBits(SWDIO_PIN_INIT *pinIO,uint32_t data, int bitsize);
uint32_t swdReadBits(int bitsize);
void writeTurnAround(SWDIO_PIN_INIT *pinIO);
void readTurnAround(SWDIO_PIN_INIT *pinIO);
void resetDebugPin();
uint32_t SW_ShiftPacket(SWDIO_PIN_INIT *pinIO,uint8_t request, uint8_t retry,uint32_t writeDat);
#endif // _DEBUG_H
