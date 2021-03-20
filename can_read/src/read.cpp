#if !DEVICE_CAN
#error[NOT_SUPPORTED] CAN not supported for this target
#endif
// DO NOT MAKE AN ID  = 0
#define ID_WRITER 0xA
#define ID2 0xB
#define ID3 0xC

//Bluepill on small board - 0xF0
//Bluepill on big board near power - 0xCD

#include "mbed.h"

SPI spi(PA_7,PA_6,PA_5); // for sending date to Arduino

DigitalOut cs(PA_4);

// bottom bluepill
DigitalOut led(LED1);
CAN reader(PB_8, PB_9,125000);
Serial pc(PA_9, PA_10);

void readMsg();
void handleMsg(int, uint16_t*);

CANMessage msg;
uint16_t readings[4] = {0}; // stores 4 readings from 1 payload

int main()
{
    cs = 1;

    spi.format(20,0); //16 bit messages + 4 bit ID, clock pol = clock phase = 0
    spi.frequency(250000); // 1MHz freq

    reader.attach(&readMsg);
}
/*
msg needs to have data with an even number of bytes. Otherwise the function tries to access data outside of the message. 
This is OK because any message can be zero padded to an even number of bytes.
*/
void readMsg(void)
{
    led = !led;
    reader.read(msg);

    for (int i = 0; i < msg.len; i += 2) // loop through bytes 0,2,4,6 and get data from elements (0,1),(2,3),(4,5),(6,7)
    {                         
        if ((msg.data[i + 1] >> 4) > 0){ // if the second byte has 2 digits, we need 2 digitshifts
            readings[i / 2] = (msg.data[i] << 8) | (msg.data[i + 1]);
        }
        else{ // otherwise, the second byte has only 1 digit and we only need 1 digitshift
            readings[i / 2] = (msg.data[i] << 4) | (msg.data[i + 1]);
        }
    }
    
    pc.printf("Succesful read! ID: %x, Length: %d\n", msg.id,msg.len);
    handleMsg(msg.id, readings); //deal with formatted msg. (send to pits, print to screen, etc.)
}


void handleMsg(int id, uint16_t* readings)
{
    cs = 0; //get ready to write to arduino

    uint16_t data[2] = {0}; // stores ID + 1 reading. To be sent to arduino
    data[0] = id;

    switch (id)
    {
    case (ID_WRITER):
        pc.printf("ID 1: \n");
        for (int i = 0; i < 4; i++)
        {   
            spi.write(readings[i]);
            pc.printf("%d\n", readings[i]);
        }
        break;
    case (ID2):
        pc.printf("ID 2: \n");
        for (int i = 0; i < 4; i++)
        {
            data[1] = readings[i];
            spi.write(readings[i]);
            pc.printf("%d\n", readings[i]);
        }
        break;
    case (ID3):
        pc.printf("ID 3: \n");
        for (int i = 0; i < 4; i++)
        { 
            data[1] = readings[i];
            spi.write(readings[i]);  
            pc.printf("%d\n", readings[i]);
        }
        break;
    default:
        break;
    }

    cs = 1; // done writing to arduino
}