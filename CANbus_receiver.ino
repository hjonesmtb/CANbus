#include <SPI.h>
#include <mcp2515.h>

#define ID1 0x001 // Change name
#define ID2 0x010 // Change name
#define ID3 0x100 // Change name

struct can_frame msg1; //msg with id1

MCP2515 mcp2515(10);

void setup() {
  Serial.begin(115200); // Find optimum value
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS); // Find optimum value
  mcp2515.setNormalMode();  
}

int id;
uint16_t readings[4] = {0}; // stores 4 readings from 1 payload

void loop() {
  
  if (mcp2515.readMessage(&msg1) == MCP2515::ERROR_OK) { // ERROR_OK == 0 as defined in the MCP2515 class
    id = msg1.can_id;

    // Each piece of data will be 2 bytes long, and each payload is 8 bytes long.
    // If (x>>4) > 0, x has 2 hex digits. 
    // We use the number of digits in the second byte to determine how many bit shifts to do.  
    // "digit" = number of meaningful digits in a hex number. Ex: 0x00000001 has 1 digit, and 0x10000000 has 8 digits.
    // "digitshift" = 4 bitshifts. Shifts a hex number by 1 digit.
    
    for (int i = 0; i<msg1.can_dlc; i+=2)  { // loop through bytes 0,2,4,6 and get data from elements (0,1),(2,3),(4,5),(6,7)
      if((msg1.data[i+1] >> 4) > 0) // if the second byte has 2 digits, we need 2 digitshifts
          readings[i/2] = (msg1.data[i] << 8) | msg1.data[i+1];
      else  // otherwise, the second byte has only 1 digit and we only need 1 digitshift
          readings[i/2] = (msg1.data[i] << 4) | msg1.data[i+1];
    }

    switch(id){
      case(ID1):
        Serial.println("ID 1: ");
        for(int i = 0; i < 4; i++){
            Serial.println(readings[i],HEX);
        }
        Serial.println();
        break;
      case(ID2):
          Serial.println("ID 2: ");
          for(int i = 0; i < 4; i++){
              Serial.println(readings[i],HEX);
          }
          Serial.println();
          break;
      case(ID3):
          Serial.println("ID 3: ");
          for(int i = 0; i < 4; i++){
              Serial.println(readings[i],HEX);
          }
          Serial.println();
          break;
    }
         
  }
}
