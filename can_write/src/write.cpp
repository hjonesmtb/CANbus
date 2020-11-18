#if !DEVICE_CAN
#error [NOT_SUPPORTED] CAN not supported for this target
#endif

// DO NOT MAKE AN ID  = 0
#define ID1 0xF0

#define SENSOR PA_7

// Top bluepill

#include "mbed.h"

DigitalOut led(LED1);
AnalogIn sensor(SENSOR);

Serial pc(PA_9,PA_10);
CAN sender(PB_8, PB_9);

int main()
{
    CANMessage msg(ID1);
    msg.len = 8;
    int counter = 0; // to count number of readings
    int reading = 0;
    unsigned char data[8] = {0};

    while(1){
        reading = 3300*sensor.read(); // Sensor data; 2 byte number [0 5000]

        data[2*counter] = reading>>8 & 0xFF; //0x1234 >> 8 = 0x12; 0x12 & 0xFF = 0x12 
        data[2*counter+1] = reading & 0xFF;    // 0x1234 & 0xFF = 0x34

        counter += 1;
        if(counter == 4){ // once we have 4 x 2byte readings, send the message
            if (sender.write(CANMessage(ID1,data,8))) {
                led = !led;
                pc.printf("Sent msg1. ID: %x\n",ID1);
            }else {
                sender.reset();
                pc.printf("Failed send\n");
                }
            counter = 0; // reset
        }
        wait_ms(250);
    }
}
