#include <SPI.h>
#include <mcp2515.h>
#define ID1 0x001 // Change name
#define ID2 0x010 // Change name
#define ID3 0x100 // Change name

struct can_frame canMsg;
MCP2515 mcp2515(10);

void setup() {
  Serial.begin(115200); // Find optimum value
  double chan1, chan2, chan3, chan4, chan5;
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS); // Find optimum value
  mcp2515.setNormalMode();  
}

int id;
uint16_t readings[4] = {0}; // stores 4 readings from 1 payload

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) { // ERROR_OK == 0 as defined in the MCP2515 class
    id = canMsg.can_id;

    // Each piece of data will be 2 bytes long, and each payload is 8 bytes long.
    // If (x>>4) > 0, x has 2 hex digits. 
    // We use the number of digits in the second byte to determine how many bit shifts to do.  
    // "digit" = number of meaningful digits in a hex number. Ex: 0x00000001 has 1 digit, and 0x10000000 has 8 digits.
    // "digitshift" = 4 bitshifts. Shifts a hex number by 1 digit.
    
    for (int i = 0; i<canMsg.can_dlc; i+=2)  { // loop through bytes 0,2,4,6 and get data from elements (0,1),(2,3),(4,5),(6,7)
      if((canMsg.data[i+1] >> 4) > 0) // if the second byte has 2 digits, we need 2 digitshifts
          readings[i/2] = canMsg.data[i] >> 8 | canMsg.data[i+1];
      else  // otherwise, the second byte has only 1 digit and we only need 1 digitshift
          readings[i/2] = canMsg.data[i] >> 4 | canMsg.data[i+1];
    }

    switch(id){
      case(ID1):
        // set relevant values from readings array
      case(ID2):
        // set relevant values from readings array
      case(ID3):
        // set relevant values from readings array
      ;
    }
         
  }
}
