#include "debug.h"
#include "struct.h"
#include <stdint.h>

uint8_t ack_err;
uint32_t readData;

int SW_CLK_L(){
  //SW_CLOCK = 0;
  return 0;
}

int SW_CLK_H(){
  //SW_CLOCK = 1;
  return 1;
}

uint32_t SW_IO_L(){
  //SW_IO = 0;
  return 0;
}

uint32_t SW_IO_H(){
  //SW_IO = 1;
  return 1;
}

void _swdAsOuput(SWDIO_PIN_INIT *pinIO){
  pinIO->pin = SWDIO_Pin;
  pinIO->mode = OUTPUT_MODE;
}

void _swdAsInput(SWDIO_PIN_INIT *pinIO){
  pinIO->pin = SWDIO_Pin;
  pinIO->mode = INPUT_MODE;
}

void _swdAsOuputOpenDrain(SWDIO_PIN_INIT *pinIO){
  pinIO->pin = SWDIO_Pin;
  pinIO->mode = OPENDRAIN_MODE;
}


void idleCycles(int time){
	// pin2 stands for SWCLK pin (pin 14 port B)
	int i;
	for(i=0;i<time;i++){
		SW_CLOCK = SW_CLK_L();
		SW_CLOCK = SW_CLK_H();
	}
}

void delay(int speed){
  int counter,value;
  for(counter = 0; counter<speed;counter++)
  {
    value++;
  }
}

void swdLineReset(SWDIO_PIN_INIT *pinIO){
	int i;
	pinIO->outdata = SW_IO_H();
	for(i=0;i<50;i++)
	{
    SW_CLOCK = SW_CLK_L();
		SW_CLOCK = SW_CLK_H();
    pinIO->round ++;
	}
}

uint32_t swdWriteBits(SWDIO_PIN_INIT *pinIO,uint32_t data, int bitsize){
	int i;

  if(pinIO->mode == OUTPUT_MODE)
  {
    for(i=0;i<bitsize;i++){
  		pinIO->outdata = data;
      SW_CLOCK = SW_CLK_L();
  		SW_CLOCK = SW_CLK_H();
  	}
  }
  else
  {
    return 0;
  }
  if(pinIO->outdata == 0xe79e)
  {
    pinIO->req_IDCODE = 1;
  }
  else
  {
    return pinIO->outdata;
  }
  return pinIO->outdata;
}




//uint32_t swdReadBits(int bitsize){
//	int i;
//	uint32_t swiftBytes[bitsize];
//	for(i=0;i<bitsize;i++){
//		swiftBytes[i] = HAL_GPIO_ReadPin(GPIOA,(SWDIO_Pin >> i) & 0x01);
//		HAL_GPIO_WritePin(GPIOB, SWDCLK_Pin, SW_CLK_H);
//		HAL_Delay(CLOCK_SPD);
//		HAL_GPIO_WritePin(GPIOB, SWDCLK_Pin, SW_CLK_L);
//		HAL_Delay(CLOCK_SPD);
//	}
//	return *swiftBytes;
//}


void readTurnAround(SWDIO_PIN_INIT *pinIO){
	int i;
  _swdAsInput(pinIO);
  SW_CLOCK = SW_CLK_L();
  SW_CLOCK = SW_CLK_H();
}

void writeTurnAround(SWDIO_PIN_INIT *pinIO){
	int i;
	_swdAsOuput(pinIO);
  SW_CLOCK = SW_CLK_L();
  SW_CLOCK = SW_CLK_H();
}

uint32_t SW_ShiftPacket(SWDIO_PIN_INIT *pinIO,uint8_t request, uint8_t retry,uint32_t writeDat)
{
	uint8_t ack, limit,i,data;
  uint32_t finalData;

    // If retry parameter is zero, use the default value instead
    if (retry == 0)
    {
        retry = 255;
    }
    limit = retry;

    // While waiting, do request phase (8-bit request, turnaround, 3-bit ack)
    do
    {
        // Turnaround or idle cycle, makes or keeps SWDIO an output
        pinIO->outdata = 1;
        _swdAsOuput(pinIO);
        //idleCycles(1);

        // Shift out the 8-bit packet request
        data = swdWriteBits(pinIO,request,8);

        // Turnaround cycle makes SWDIO an input
        _swdAsInput(pinIO);
        //idleCycles(1);

        // Shift in the 3-bit acknowledge response
        //swdReadBits(32);
        if(pinIO->outdata == request)
        {
          ack = SW_ACK_OK;
          pinIO->acknowledge = 0x001;
          pinIO->indata = 0x001;
        }

        // Check if we need to retry the request
        if ((ack == SW_ACK_WAIT) && --retry)
        {
            // Delay an increasing amount with each retry
            for (i=retry; i < limit; i++);
        }
        else
        {
            break;  // Request phase complete (or timeout)
        }
    }
    while (TRUE);

    // If the request was accepted, do the data transfer phase (turnaround if
    // writing, 32-bit data, and parity)
    if (ack == SW_ACK_OK)
    {
        if (request & 0x04)
        {
            // Swap endian order while shifting in 32-bits of data
        	idleCycles(1);

          if(pinIO->req_IDCODE == 1 && pinIO->round >= 50)
          {
            pinIO->indata = SW_EQ_IDCODE;
            finalData = SW_EQ_IDCODE;
            pinIO->req_IDCODE = 0;
          }
          else{
            pinIO->indata = pinIO->lastdata;
          }
        }
        else
        {
            // Turnaround cycle makes SWDIO an output
            _swdAsOuput(pinIO);
            //idleCycles(1);
            // Swap endian order while shifting out 32-bits of data
            finalData=swdWriteBits(pinIO,writeDat, 32);

            // Shift out the parity bit
            //SWDIO_Out = SW_CalcDataParity(); _StrobeSWCLK;
        }
    }
    
    // Update the global error accumulator if there was an error
    if (ack != SW_ACK_OK)
    {
    	ack_err = ack;
    }
    return finalData=0;
}
