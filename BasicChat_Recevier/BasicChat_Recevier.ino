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

int rec_data = 0;
int data_size = 100;
bool first = true;
long beginT;
long endT;
bool done = false;

void setup()
{
  Serial.begin(9600);  // Start up serial
  Serial1.begin(115200);
  rfBegin(15);  // Initialize ATmega128RFA1 radio on channel 11 (can be 11-26)
  
  // Send a message to other RF boards on this channel
  rfPrint("ATmega128RFA1 Dev Board Online!\r\n");
}

void loop()
{ 
  if (rfAvailable())  // If data receievd on radio...
  {
    if(first) {
      beginT = micros(); 
      first = false;
    }
    // calculate the data that is recevide from recevier
    if(rfRead() == 'p'){
      rec_data++;
    }
    else{
       endT = micros();
       done = true;
    }
  }
  else if(Serial.read() == 's' && done) 
  {
    Serial.print("Data recevided: ");
    Serial.println(rec_data);
    Serial.print("Recevied Rate: ");
    Serial.print(rec_data/(data_size*1.0) * 100.0);
    Serial.println("%");
    Serial.print("Receving spped: ");
    Serial.print(rec_data*1000.0/(endT-beginT));
    Serial.println(" Kbps");
  }
}
