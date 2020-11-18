#if !DEVICE_CAN
#error[NOT_SUPPORTED] CAN not supported for this target
#endif
// DO NOT MAKE AN ID  = 0
#define ID1 0xCD
#define ID2 0xF0
#define ID3 0x539

#include "mbed.h"

// bottom bluepill
DigitalOut led(LED1);
CAN reader(PB_8, PB_9);
Serial pc(PA_9, PA_10);

void readMsg(CANMessage, uint16_t*);
void handleMsg(int, uint16_t*);

int main()
{
    CANMessage msg;
    msg.len = 8;
    int id = 0;
    uint16_t readings[4] = {0}; // stores 4 readings from 1 payload

    while (true)
    {
        if (reader.read(msg))
        {
            led = !led;
            id = msg.id;
            readMsg(msg, readings); //fill readings array with msg data
            pc.printf("Succesful read! ID: %x\n", id);
            handleMsg(id, readings); //deal with formatted msg. (send to pits, print to screen, etc.)
        }
    }
}
/*
msg needs to have data with an even number of bytes. Otherwise the function tries to access data outside of the message. 
This is OK because any message can be zero padded to an even number of bytes.
*/
void readMsg(CANMessage msg, uint16_t* readings)
{
    for (int i = 0; i < msg.len; i += 2) // loop through bytes 0,2,4,6 and get data from elements (0,1),(2,3),(4,5),(6,7)
    {                         
        if ((msg.data[i + 1] >> 4) > 0){ // if the second byte has 2 digits, we need 2 digitshifts
            readings[i / 2] = (msg.data[i] << 8) | (msg.data[i + 1]);
        }
        else{ // otherwise, the second byte has only 1 digit and we only need 1 digitshift
            readings[i / 2] = (msg.data[i] << 4) | (msg.data[i + 1]);
        }

    }
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

/* 
TX line always at 3.3v
RX line always at 5v
 */