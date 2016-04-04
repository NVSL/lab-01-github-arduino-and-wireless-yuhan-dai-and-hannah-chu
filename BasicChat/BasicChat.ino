/*
  ATmega128RFA1 Dev Board Basic Chat
  by: Jim Lindblom
      SparkFun Electronics
  date: July 3, 2012
  License: Beerware. Feel free to use, reuse, and modify this code
  as you please. If you find it useful, you can buy me a beer.

  This code sets up the ATmega128RFA1's wireless transciever in
  the most basic way possible to serve as a serial gateway.
  Serial into the ATmega128RFA1's UART0 will go out the RF radio.
  Data into the RF radio will go out the MCU's UART0.
*/

#include "RadioFunctions.h"

int alreadyRan = 0;
long time = 0;
long received = 0;

void setup()
{
  Serial.begin(9600);  // Start up serial
  Serial1.begin(115200);
  rfBegin(15);  // Initialize ATmega128RFA1 radio on channel 11 (can be 11-26)
  
  // Send a message to other RF boards on this channel
  rfPrint("ATmega128RFA1 Dev Board Online!\r\n");
  Serial.print("I'm online!\r\n");
}

void loop()
{
  if (micros() > 10000000 && !alreadyRan)
  {
    Serial.print("Sending bunches of a's...\r\n");
    alreadyRan = 1;
    // Do once
    rfWrite('B');
    for (int i = 0; i < 100; i++)
    {
      rfWrite('a');
    }
    rfWrite('E');
  }
  if (Serial.available())  // If serial comes in...
  {
    rfWrite(Serial.read()); // ...send it out the radio.
  }
  
  if (rfAvailable()> 0)  // If data receievd on radio...
  {
    if(received == 0){
      time = micros();
      //received = 1;
      Serial.print("We're starting to receive data at time = ");
      Serial.print(time);
      Serial.print("\r\n");
    }
    received++;
    Serial.print(rfRead());  // ... send it out serial.
  }
  else // No data, is this the end of a stream?
  {
    if (received)
    {
      long currTime = micros();
      Serial.print("\r\nStopped receiving data: ");
      Serial.print(currTime);
      Serial.print("\r\nTotal time taken: ");
      Serial.print(currTime - time);
      Serial.print("\r\nReceived: ");
      Serial.print(received);
      Serial.print("\r\n");
      received = 0;
    }
  }
}
