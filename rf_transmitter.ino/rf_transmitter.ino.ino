// RF Transmission libraries
// Mirf docs http://archive.communica.co.za/Content/Catalog/Documents/D2018958833.pdf
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h> 

//  Library to make keypresses more elegant
#include <Bounce2.h>
#define BUTTON_PIN_ONE 8 // D8 nano pin
#define BUTTON_PIN_TWO 7 // D7 nano pin
#define BUTTON_PIN_THREE 6 // D6 nano pin
#define TRANSMISSION_BUTTON_PIN 5 // D5 nano pin
#define CANCEL_BUTTON_PIN 4 // D4 nano pin
// Docs https://github.com/thomasfredericks/Bounce2
Bounce2::Button button_one = Bounce2::Button();
Bounce2::Button button_two = Bounce2::Button();
Bounce2::Button button_three = Bounce2::Button();
Bounce2::Button transmission_button = Bounce2::Button();
Bounce2::Button cancel_button = Bounce2::Button();

void setup()
{
  Serial.begin(9600);

  // All button setup
  // Attach the debouncer to a pin with INPUT_PULLUP mode, if this doesn't work just do pullup
  button_one.attach(BUTTON_PIN_ONE,INPUT_PULLUP); 
  button_two.attach(BUTTON_PIN_TWO,INPUT_PULLUP); 
  button_three.attach(BUTTON_PIN_THREE,INPUT_PULLUP); 
  transmission_button.attach(TRANSMISSION_BUTTON_PIN,INPUT_PULLUP); 
  cancel_button.attach(CANCEL_BUTTON_PIN,INPUT_PULLUP); 

  // DEBOUNCE INTERVAL IN MILLISECONDS
  button_one.interval(5); 
  button_two.interval(5); 
  button_three.interval(5); 
  transmission_button.interval(5); 
  cancel_button.interval(5); 
  // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  button_one.setPressedState(LOW); 
  button_two.setPressedState(LOW); 
  button_three.setPressedState(LOW); 
  transmission_button.setPressedState(LOW); 
  cancel_button.setPressedState(LOW); 
  
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
unsigned int one_number_of_presses = 0;
unsigned int two_number_of_presses = 0;
unsigned int three_number_of_presses = 0;

void loop()
{
  // button handler
  button_one.update();
  button_two.update();
  button_three.update();
  transmission_button.update();
  cancel_button.update();
  

  if ( button_one.pressed() ) {
    Serial.println("One pressed");
    one_number_of_presses++;
    two_number_of_presses = 0;
    three_number_of_presses = 0;
    Serial.println(one_number_of_presses);
    
  }

  if ( button_two.pressed() ) {
    Serial.println("Two pressed");
    one_number_of_presses = 0;
    two_number_of_presses++;
    three_number_of_presses = 0;
    Serial.println(two_number_of_presses);
    
  }

  if ( button_three.pressed() ) {
    Serial.println("Three pressed");
    one_number_of_presses = 0;
    two_number_of_presses = 0;
    three_number_of_presses++;
    Serial.println(three_number_of_presses);
    
  }

  if ( cancel_button.pressed() ) {
    one_number_of_presses = 0;
    two_number_of_presses = 0;
    three_number_of_presses = 0;
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
      one_number_of_presses = 0;
    }

    if(two_number_of_presses > 0) {
      switch(two_number_of_presses % 3) {
        case 0:
          Serial.println("I love you");
          break;
        case 1:
          Serial.println("You're the best");
          break;
        case 2:
          Serial.println("Bed for manatee?");
          break;
      }
      two_number_of_presses = 0;
    }

    if(three_number_of_presses > 0) {
      switch(three_number_of_presses % 3) {
        case 0:
          Serial.println("Yes");
          break;
        case 1:
          Serial.println("No");
          break;
        case 2:
          Serial.println("Maybe");
          break;
      }
      three_number_of_presses = 0;
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
