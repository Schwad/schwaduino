#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
//Define a variable adata to store the final result. 
unsigned int adata = 0;

void setup()
{
Serial.begin(9600);
//---------Initial part, can't be modified at any time--------- 
Mirf.cePin = 9; //Set CE Pin to D9
Mirf.csnPin = 10; //Set CE Pin to D10
Mirf.spi = &MirfHardwareSpi;
Mirf.init(); //initialization nRF24L01

//---------Configuration part, can be modified it at any time---------
//Set the receiving identifier "Rev01"
Mirf.setRADDR((byte *)"Rec01");

//Set the number of bytes sent and received at a time, here sent an integer. //Write sizeof(unsigned int), which is actually equal to 2 bytes.
Mirf.payload = sizeof(unsigned int);

// Sending channel, can fill 0~128, send and receive must be consistent. 
Mirf.channel = 3;
Mirf.config();

//Note that one Arduino writes Sender.ino and the other writes Receiver.ino. 
//To identify the program written in Receiver.ino.
Serial.println("I'm Receiver...");
}

void loop()
{

//Define a scratchpad array with a size of Mirf.payload.
byte data[Mirf.payload];

if(Mirf.dataReady()) //Waiting the prepared receive data.
  {
    Mirf.getData(data); //Receive data to data array. 
    //data[1]< move left 8 bits and data[0] merge, reorganize data. 
    adata = (unsigned int)((data[1] << 8) | data[0]);
    delay(2000);
    Serial.print("pin="); 
    Serial.println(adata);
    delay(2000);
  Serial.println("made it 3");
    //Can also output double-byte data. 
    //Serial.write(data[1]); 
    //Serial.write(data[0]);
  }
}
