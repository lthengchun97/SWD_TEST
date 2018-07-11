#include "unity.h"
#include "debug.h"
#include "struct.h"
#include "stdint.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_debug_initial_swd_io_pin_as_input(void)
{
    SWDIO_PIN_INIT pin_IO;
    readTurnAround(&pin_IO);
    TEST_ASSERT_EQUAL(pin_IO.pin,SWDIO_Pin);
    TEST_ASSERT_EQUAL(pin_IO.mode,INPUT_MODE);
}

void test_debug_initial_swd_io_pin_as_output(void)
{
    SWDIO_PIN_INIT pin_IO;
    writeTurnAround(&pin_IO);
    TEST_ASSERT_EQUAL(pin_IO.pin,SWDIO_Pin);
    TEST_ASSERT_EQUAL(pin_IO.mode,OUTPUT_MODE);
}

void test_io_pin_as_output_and_line_reset(void)
{
    SWDIO_PIN_INIT pin_IO;
    pin_IO.round = 0;
    writeTurnAround(&pin_IO);
    swdLineReset(&pin_IO);
    TEST_ASSERT_EQUAL(pin_IO.pin,SWDIO_Pin);
    TEST_ASSERT_EQUAL(pin_IO.mode,OUTPUT_MODE);
    TEST_ASSERT_EQUAL(pin_IO.outdata,1);
    TEST_ASSERT_EQUAL(pin_IO.round,50);
}

void test_write_a_data_in(void)
{
  SWDIO_PIN_INIT pin_IO;
  writeTurnAround(&pin_IO);
  uint32_t outdata = swdWriteBits(&pin_IO,0xe79e,16);
  TEST_ASSERT_EQUAL(pin_IO.pin,SWDIO_Pin);
  TEST_ASSERT_EQUAL(pin_IO.mode,OUTPUT_MODE);
  TEST_ASSERT_EQUAL(pin_IO.outdata,0xe79e);
}

void test_perform_simple_shift_packet(void)
{
  SWDIO_PIN_INIT pin_IO;
  writeTurnAround(&pin_IO);
  uint32_t outdata = SW_ShiftPacket(&pin_IO,0xA5,0,0);
  TEST_ASSERT_EQUAL(pin_IO.pin,SWDIO_Pin);
  TEST_ASSERT_EQUAL(pin_IO.mode,INPUT_MODE);
  TEST_ASSERT_EQUAL(pin_IO.acknowledge,1);
}

void test_perform_initialization_sequence(void)
{
  SWDIO_PIN_INIT pin_IO;
  pin_IO.round = 0;
  //Set the pin as output
  writeTurnAround(&pin_IO);

  // Perform line reset
  swdLineReset(&pin_IO);
  TEST_ASSERT_EQUAL(pin_IO.pin,SWDIO_Pin);
  TEST_ASSERT_EQUAL(pin_IO.mode,OUTPUT_MODE);
  TEST_ASSERT_EQUAL(pin_IO.outdata,1);

  // Request jtag to swd switch sequence
  swdWriteBits(&pin_IO,0xe79e,16);
  TEST_ASSERT_EQUAL(pin_IO.outdata,SW_EQ_CODE);
  TEST_ASSERT_EQUAL(pin_IO.req_IDCODE,1);

  // Perform line reset
  swdLineReset(&pin_IO);
  TEST_ASSERT_EQUAL(pin_IO.pin,SWDIO_Pin);
  TEST_ASSERT_EQUAL(pin_IO.mode,OUTPUT_MODE);
  TEST_ASSERT_EQUAL(pin_IO.outdata,1);

  // Read IDCODE
  uint32_t outdata = SW_ShiftPacket(&pin_IO,0xA5,0,0);
  TEST_ASSERT_EQUAL(pin_IO.pin,SWDIO_Pin);
  TEST_ASSERT_EQUAL(pin_IO.mode,INPUT_MODE);
  TEST_ASSERT_EQUAL(pin_IO.acknowledge,1);
  TEST_ASSERT_EQUAL(pin_IO.round,100);
  TEST_ASSERT_EQUAL(pin_IO.req_IDCODE,0);
  TEST_ASSERT_EQUAL(pin_IO.indata,SW_EQ_IDCODE);
}

void test_perform_initialization_for_access_port(void){
  SWDIO_PIN_INIT pin_IO;
  //Set the pin as output
  writeTurnAround(&pin_IO);

  // set the CDBGPWRUPREQ and CSYSPWRUPREQ bits of CTRL/STATUS
  uint32_t outdata = SW_ShiftPacket(&pin_IO,0xA3,0,0x20000000);
  TEST_ASSERT_EQUAL(pin_IO.pin,SWDIO_Pin);
  TEST_ASSERT_EQUAL(pin_IO.mode,OUTPUT_MODE);
  TEST_ASSERT_EQUAL(pin_IO.acknowledge,1);
  TEST_ASSERT_EQUAL(pin_IO.outdata,0x20000000);

  //Set the pin as output again to avoid any errors occur
  writeTurnAround(&pin_IO);

  //Write 0x000000F0 to SELECT (select AHB-AP, bank 0xF)
  uint32_t outdata_2 = SW_ShiftPacket(&pin_IO,0xA9,0,0x000000F0);
  TEST_ASSERT_EQUAL(pin_IO.pin,SWDIO_Pin);
  TEST_ASSERT_EQUAL(pin_IO.mode,OUTPUT_MODE);
  TEST_ASSERT_EQUAL(pin_IO.acknowledge,1);
  TEST_ASSERT_EQUAL(pin_IO.outdata,0x000000F0);

  //Read the IDR CODE

  //Write 0x00000000 to SELECT (select AHB-AP , bank 0x0)
  uint32_t outdata_2 = SW_ShiftPacket(&pin_IO,0xA9,0,0x00000000);
  TEST_ASSERT_EQUAL(pin_IO.pin,SWDIO_Pin);
  TEST_ASSERT_EQUAL(pin_IO.mode,OUTPUT_MODE);
  TEST_ASSERT_EQUAL(pin_IO.acknowledge,1);
  TEST_ASSERT_EQUAL(pin_IO.outdata,0x00000000);
}
