#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg1;
struct can_frame canMsg2;
struct can_frame canMsg3;

MCP2515 mcp2515(10);


void setup() {
  canMsg1.can_id  = 0x001;
  canMsg1.can_dlc = 8;
  canMsg1.data[0] = 0x1;
  canMsg1.data[1] = 0x1;
  canMsg1.data[2] = 0x0;
  canMsg1.data[3] = 0x0;
  canMsg1.data[4] = 0xAA;
  canMsg1.data[5] = 0xA;
  canMsg1.data[6] = 0x1;
  canMsg1.data[7] = 0x2;

  canMsg2.can_id  = 0x10;
  canMsg2.can_dlc = 8;
  canMsg2.data[0] = 0x10;
  canMsg2.data[1] = 0x10;
  canMsg2.data[2] = 0x10;
  canMsg2.data[3] = 0x10;
  canMsg2.data[4] = 0x10;
  canMsg2.data[5] = 0x10;
  canMsg2.data[6] = 0x10;
  canMsg2.data[7] = 0x10;

  canMsg3.can_id  = 0x100;
  canMsg3.can_dlc = 8;
  canMsg3.data[0] = 0xa;
  canMsg3.data[1] = 0xb;
  canMsg3.data[2] = 0xc;
  canMsg3.data[3] = 0xd;
  canMsg3.data[4] = 0xe;
  canMsg3.data[5] = 0xf;
  canMsg3.data[6] = 0xaa;
  canMsg3.data[7] = 0xbb;

  
  while (!Serial);
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  
  Serial.println("Example: Write to CAN");
}

void loop() {
  mcp2515.sendMessage(&canMsg1);
  delay(500);
  mcp2515.sendMessage(&canMsg2);
  delay(500);
  mcp2515.sendMessage(&canMsg3);

  Serial.println("Messages sent"); 
  delay(500);
}
