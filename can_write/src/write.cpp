#if !DEVICE_CAN
#error [NOT_SUPPORTED] CAN not supported for this target
#endif

// DO NOT MAKE AN ID  = 0
#define ID1 0xf0
#define ID2 0xCD
#define ID3 0x539

#define SENSOR PA_7

//Bluepill on small board - 0xF0
//Bluepill on big board near power - 0xCD

#include "mbed.h"

void readMsg();
void handleMsg(int, uint16_t*);

DigitalOut led(LED1);
AnalogIn sensor(SENSOR);

Serial pc(PA_9,PA_10);
CAN sender(PB_8, PB_9);

CANMessage msg_in;
uint16_t readings[4] = {0}; // stores 4 readings from 1 payload

int main()
{
    CANMessage msg(ID1);
    int counter = 0; // to count number of readings
    int reading = 0;
    uint8_t data[8] = {0};
    sender.filter(ID2,0xff);
    
    sender.attach(readMsg);
    
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
        wait_ms(500);
    }
}

void readMsg(void)
{
    sender.read(msg_in);

    for (int i = 0; i < msg_in.len; i += 2) // loop through bytes 0,2,4,6 and get data from elements (0,1),(2,3),(4,5),(6,7)
    {                         
        if ((msg_in.data[i + 1] >> 4) > 0){ // if the second byte has 2 digits, we need 2 digitshifts
            readings[i / 2] = (msg_in.data[i] << 8) | (msg_in.data[i + 1]);
        }
        else{ // otherwise, the second byte has only 1 digit and we only need 1 digitshift
            readings[i / 2] = (msg_in.data[i] << 4) | (msg_in.data[i + 1]);
        }
    }
    
    pc.printf("Succesful read! ID: %x, Length: %d\n", msg_in.id,msg_in.len);
    handleMsg(msg_in.id, readings); //deal with formatted msg. (send to pits, print to screen, etc.)
}

void handleMsg(int id, uint16_t* readings)
{
    switch (id)
    {
    case (ID1):
        pc.printf("ID 1: \n");
        for (int i = 0; i < 4; i++)
        {
            pc.printf("%d\n", readings[i]);
        }
        break;
    case (ID2):
        pc.printf("ID 2: \n");
        for (int i = 0; i < 4; i++)
        {
            pc.printf("%d\n", readings[i]);
        }
        break;
    case (ID3):
        pc.printf("ID 3: \n");
        for (int i = 0; i < 4; i++)
        {
            //pc.printf("%d\n", readings[i]);
        }
        break;
    default:
        break;
    }
}
