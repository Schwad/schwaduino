#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h> 

void setup()
{
  Serial.begin(9600);
  
  Mirf.cePin = 9; //Set the CE pin to D9 
  Mirf.csnPin = 10; //Set the CE pin to D10 
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init(); //initialization nRF24L01
  
  //Set the receiving identifier "Sen01"
  Mirf.setRADDR((byte *)"Sen01");
  
  //Set the number of bytes sent and received at a time, here send an integer, write sizeof (unsigned int), actually equal to 2 bytes
  Mirf.payload = sizeof(unsigned int);
  
  //Sending channel, can fill 0~128, send and receive must be consistent.
  Mirf.channel = 3; Mirf.config();
  
  //Note that one Arduino writes Sender.ino and the other writes Receiver.ino.
  //The identifier here is written to Sender.ino 
  Serial.println("I'm Sender...");
}

unsigned int adata = 0; 

void loop()
{
  adata=123;
  //Since nRF24L01 can only send Mirf.payload data in a byte single byte array, //So all the data that needs to be transferred must be split into bytes.
  //Define a byte array to store pending data, because Mirf.payload = sizeof (unsigned int);
  //Actually the following is equal to byte data[2]; 
  byte data[Mirf.payload];

  // additional comments in pdf about why we do this
  data[0] = adata & 0xFF; //low eight bits to data[0]，
  data[1] = adata >> 8;  //high eight bits to data[1]

  Serial.println("made it 1");
  
  //Settings send data to "serv1"
  Mirf.setTADDR((byte *)"Rec01");
  Mirf.send(data);
  //The next step can only be performed after the while loop function transmission is completed. 
  while(Mirf.isSending()) {}
  Serial.println("made it 2");
  delay(20);
  Serial.println("made it 3");
}
