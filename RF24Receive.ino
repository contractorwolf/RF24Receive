
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
  Serial.println("*** PRESS 'T' to begin transmitting to the other node\n\r");

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
    // if there is data ready
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      unsigned long got_message;
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &got_message, sizeof(unsigned long) );

        // Spew it
        Serial.print("Got payload... ");
        Serial.println(got_message);

	// Delay to let the other unit transition to receiver
	//delay(20);
      }

      // First, stop listening so we can talk
      radio.stopListening();

      // Send the final one back.
      radio.write( &got_message, sizeof(unsigned long) );
      
      // Now, resume listening so we catch the next packets.
      radio.startListening();      
      
      Serial.println("Response Sent, returned to listening");
    }
}

