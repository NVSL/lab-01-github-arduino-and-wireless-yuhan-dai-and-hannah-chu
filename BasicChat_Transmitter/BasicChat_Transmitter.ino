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
int data_size = 100;
int packet = 0;
long time;
long current;

void setup()
{
  Serial.begin(9600);  // Start up serial
  Serial1.begin(115200);
  rfBegin(15);  // Initialize ATmega128RFA1 radio on channel 11 (can be 11-26)
  
  // Send a message to other RF boards on this channel
  rfPrint("ATmega128RFA1 Dev Board Online!\r\n");
  time = micros();
}

void loop()
{  
  if(Serial.available()){
    // send out data_size characters through RFradio
    if(packet < data_size) {
      rfWrite('p');
      packet++;   
    } 
    // calculate the time it takes to send all the data
    else if(packet == data_size){
      current = micros() - time;
      Serial.print("Transfer rate for ");
      Serial.print(data_size);
      Serial.print(" packets:  ");
      // calculate kbps
      Serial.print(packet*8.0*1000.0/current);
      Serial.println(" Kbps"); 
      packet++;
    }
  }
}
