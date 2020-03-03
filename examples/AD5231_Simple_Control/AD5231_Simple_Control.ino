/* AD5231_Simple_Control.ino
  A library to control the AD5231 Digital Potentiometer by Analog Devices.
  https://github.com/WD24/AD5231-Arduino-Library
  This project is licensed under the MIT License - see the LICENSE.md file for details

  Data sheet for the AD5231 can be found at https://www.analog.com/en/products/ad5231.html#product-documentation

  Control an AD5231 digital potentiometer with your microcontroller.
  Conect a manual potentiometer one side to 5v the other to ground.
  to analog pin 1 the wiper.

  Terminal A (pin 10) of the AD5231 to 5v and Terminal B (pin 8) to ground via a resistor equal to the digipot
  the wiper (pin 9) to the anode of an LED and a 330 ohm resistor between the cathode and ground.
*/

#include <AD5231.h> // SPI library is already included in this library so no need to add it a second time

AD5231 MyDigiPot(10); // salve select on pin 10

int OldRead = 0;
int NewRead;

void setup()
{
  MyDigiPot.begin(); // SPI.begin is taken care of in this initilisation
  
  pinMode(A1, INPUT);
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect
  }
  delay(10);

}

void loop() 
{
  NewRead = analogRead(A1);

  if(NewRead != OldRead)
  {
    MyDigiPot.WriteWiper (NewRead);

    OldRead = NewRead;
  }
}
