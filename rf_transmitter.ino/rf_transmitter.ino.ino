// RF Transmission libraries
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h> 

//  Library to make keypresses more elegant
#include <Bounce2.h>
#define BUTTON_PIN 8 // D8 nano pin
Bounce2::Button button = Bounce2::Button(); // Docs https://github.com/thomasfredericks/Bounce2

void setup()
{
  Serial.begin(9600);

  // All button setup
  button.attach(BUTTON_PIN,INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode, if this doesn't work just do pullup
  // DEBOUNCE INTERVAL IN MILLISECONDS
  button.interval(5); 
  // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  button.setPressedState(LOW); 
  
  // All Transmitter Setup
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

//Global for data
unsigned int adata = 0; 

//Global for number of presses
unsigned int number_of_presses = 0;

void loop()
{
  // button handler
  button_one.update();

  if ( button.pressed() ) {
    one_number_of_presses++;
    two_number_of_presses = 0;
    three_number_of_presses = 0;
    Serial.println(one_number_of_presses);
    
  }

  if ( transmission_button.pressed() ) {
    if(one_number_of_presses > 0) {
      switch(one_number_of_presses % 3) {
        case 0:
          Serial.println("make me a sandwich");
          break;
        case 1:
          Serial.println("get me a beer");
          break;
        case 2:
          Serial.println("goin to the pub");
          break;
      }
    }
  }

  // transmission handler
  adata=123;
  //Since nRF24L01 can only send Mirf.payload data in a byte single byte array, //So all the data that needs to be transferred must be split into bytes.
  //Define a byte array to store pending data, because Mirf.payload = sizeof (unsigned int);
  //Actually the following is equal to byte data[2]; 
  byte data[Mirf.payload];

  // additional comments in pdf about why we do this
  data[0] = adata & 0xFF; //low eight bits to data[0]，
  data[1] = adata >> 8;  //high eight bits to data[1]
//
//  Serial.println("made it 1");
//  
//  //Settings send data to "serv1"
//  Mirf.setTADDR((byte *)"Rec01");
//  Mirf.send(data);
//  //The next step can only be performed after the while loop function transmission is completed. 
//  while(Mirf.isSending()) {}
//  Serial.println("made it 2");
//  delay(20);
//  Serial.println("made it 3");
}
