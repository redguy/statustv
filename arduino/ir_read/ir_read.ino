#include <IRLib.h>
#include <IRLibMatch.h>
#include <IRLibTimer.h>

/* Receiving IR signal and dump the details */
#include <IRLib.h>
//create a receiver object
IRrecv rcv(6);
 
//create a decoder object
IRdecode decoder;

void setup()
{
  Serial.begin(115200);//We will read the output on the serial monitor
  rcv.enableIRIn(); // Start the receiver
}

void loop() {
  //Loop until we get a signal and pass it to the decoder
  if (rcv.GetResults(&decoder)) {
    decoder.decode();//decode the signal
    decoder.DumpResults();//dump the results on the serial monitor
    rcv.resume(); //restart the receiver
    

  }
}



