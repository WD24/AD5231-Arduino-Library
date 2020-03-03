/* AD5231_Calculate_Resistance.ino
  A library to control the AD5231 Digital Potentiometer by Analog Devices.
  https://github.com/WD24/AD5231-Arduino-Library
  This project is licensed under the MIT License - see the LICENSE.md file for details

  Data sheet for the AD5231 can be found at https://www.analog.com/en/products/ad5231.html#product-documentation

  Calculating the resistance according to the wiper value. The output is within +/- one bit value

  first you'll need to run some measurments.
*/
#include <AD5231.h> // SPI library is already included in this library so no need to add it a second time

AD5231 MyDigiPot(10); // salve select on pin 10

float Rmax = 73870L; //73.8k ohms
float Rmin = 72.6; //76 ohms


float R_Interval = (Rmax - Rmin) / 1024 ;

unsigned long R_M = Rmax; // Used by Serial.print so that it reads better
unsigned long Desired_Resistance; // input for your desired resistance
unsigned long Old_Resistance = 0; // used as a comparitor

void setup()
{
  MyDigiPot.begin(); // SPI.begin is taken care of in this initilisation
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect
  }
  delay(10);

  SketchSetup();

  Serial.println('\n');
  Serial.print("Type in your desired resistance in ohms; between ");
  Serial.print(round(Rmin) + 1);
  Serial.print(" and ");
  Serial.print(R_M);
  Serial.println(" ohms");
}

void loop()
{
  if (Serial.available() > 0)
  {
    unsigned long IncommingResistance = 0 ;
    //filter Serial data in to a unsigned long
    while (1)
    {
      char incomingByte = Serial.read();
      if (incomingByte == '\n' || incomingByte == '\r') break;   // exit the while(1), we're done receiving

      if (incomingByte == -1) continue;  // if no characters are in the buffer read() returns -1

      // convert to integer, add, and shift left 1 decimal place
      IncommingResistance *= 10;  // shift left 1 decimal place
      IncommingResistance = ((incomingByte - 48) + IncommingResistance);
    }
    if (IncommingResistance > 0)
    {
      Desired_Resistance = IncommingResistance;

      if ( Desired_Resistance != Old_Resistance)
      {
        if (Desired_Resistance > Rmax)
        {
          Serial.print("Your selected resistance is too high! please type in your desired resistance in ohms; between ");
          Serial.print(round(Rmin) + 1);
          Serial.print(" and ");
          Serial.print(R_M);
          Serial.println(" ohms");
        }
        else if (Desired_Resistance < Rmin)
        {
          Serial.print("Your selected resistance is too low! please type in your desired resistance in ohms; between ");
          Serial.print(round(Rmin) + 1);
          Serial.print(" and ");
          Serial.print(R_M);
          Serial.println(" ohms");
        }
        else
        {
          SetResistance(Desired_Resistance);
          Old_Resistance = Desired_Resistance;
          delay(1000);
        }
      }
    }
  }
}

void SetResistance(unsigned long R)
{
  // R = desired resistance
  // Rout = ((Rmax - Rmin)/1024) X Wiper Position + Rmin
  //Wiper Position = (Rout - Rmin) / R_Interval;
  
  float A = (R - Rmin) / R_Interval;

  Serial.print("Desired resistance of ");
  Serial.print(R);
  Serial.print(" ohms; Nerest resistor setting is ");

  int y = round(A);

  Serial.print((y * R_Interval) + Rmin);

  Serial.print("ohms; Wiper setting ");
  Serial.println(y);

  MyDigiPot.WriteWiper (y);
  delay(100);
  MyDigiPot.WriteWiper (y);
}

void SketchSetup()
{
  Serial.println("AD5231_Calculate_Resistance.ino");
  Serial.println("Calculating the resistance according to the wiper value. The output is within +/- one bit value");
  Serial.println("Lets start by taking some measurments.");
  Serial.println('\n');
  Serial.println ("Connect a multi-meter between Terminal B (pin 8)");
  Serial.println ("and Terminal W (pin 9) on your AD5231");
  Serial.println();

  Serial.println("Setting the AD5231 to maximum");

  MyDigiPot.WriteWiper (1023);
  delay(50);
  Serial.println("what is the resistance?");
  while (!Serial.available())
  {

  }
  if (Serial.available() > 0)
  {
    unsigned long IncommingResistance = 0 ;
    //filter Serial input in to a unsigned long
    while (1)
    {
      char incomingByte = Serial.read();
      if (incomingByte == '\n' || incomingByte == '\r') break;   // exit the while(1), we're done receiving

      if (incomingByte == -1) continue;  // if no characters are in the buffer read() returns -1

      // convert ASCII to integer, add, and shift left 1 decimal place
      IncommingResistance *= 10;  // shift left 1 decimal place
      IncommingResistance = ((incomingByte - 48) + IncommingResistance);
    }
    if (IncommingResistance > 0)
    {
      R_M = IncommingResistance;
    }
  }

  delay(500);
  Serial.print(R_M);
  Serial.println(" ohms");
  Rmax = R_M;

  Serial.println("Setting the AD5231 to minimum");
  while (Serial.read() > -1) // clear the serial incomming buffer
  {
  }
  MyDigiPot.WriteWiper (0);
  delay(50);
  Serial.println("what is the resistance?");
  while (!Serial.available())
  {

  }
  String inString = "";
  while (Serial.available() > 0)
  {
    while (1)
    {
      char incomingByte = Serial.read();
      if (incomingByte == '\n' || incomingByte == '\r') break;   // exit the while(1), we're done receiving

      if (incomingByte == -1) continue;  // if no characters are in the buffer read() returns -1
      inString += (char)incomingByte;
    }

    Rmin = inString.toFloat();

  }
  while (Serial.read() > -1) // clear the serial incomming buffer
  {
  }

  delay(500);
  Serial.print(Rmin);
  Serial.println(" ohms");


}
