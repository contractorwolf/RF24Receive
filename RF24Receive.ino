
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup(void)
{
  Serial.begin(57600);

  Serial.println();
  Serial.println("RF24 Receive started");
  Serial.println("waiting on incoming messages...");

  // Setup and configure rf radio
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

  // optionally, reduce the payload size to improve reliability
  //radio.setPayloadSize(8);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);

  radio.startListening();

  radio.printDetails();//doesnt work without printf
}

void loop(void)
{
    // check for available data
    if ( radio.available() )
    {
      unsigned long received_message;
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &received_message, sizeof(unsigned long) );

        Serial.print("Message received... ");
        Serial.println(received_message);

	// Delay to let the other unit transition to receiver
	//delay(20);
      }

      //stop listening to transmit
      radio.stopListening();

      //relay the message back to sender to confirm it was received
      radio.write( &received_message, sizeof(unsigned long) );
      
      // resume listening
      radio.startListening();      
      
      //RESPOND TO COMMANDS HERE
      //************************
      
            
      //************************
      
      Serial.println("Response Sent, returned to listening");
    }
}

