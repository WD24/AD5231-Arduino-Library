/* AD5231_Testing.ino
  A library to control the AD5231 Digital Potentiometer by Analog Devices.
  https://github.com/WD24/AD5231-Arduino-Library
  This project is licensed under the MIT License - see the LICENSE.md file for details

  Data sheet for the AD5231 can be found at https://www.analog.com/en/products/ad5231.html#product-documentation

  Testing the AD5231 running the digital potentiometer through a series of different tasks.

    Reading the Potentiometers Wiper position (0 - 1023) 10 bit (not its resistance)
    Read the memory
    Saving new data to memory
    Setting the Potentiometers Wiper to a given position
    Save wiper's position to memory
    Reset the digital potentiometer
    Toggle on or off the logic pins O1 & O2
    Increasing the Potentiometer's position by one
    Decreasing the Potentiometer's position by one

  Connections

   AD5231 pin | AD5231 pin name         | Arduino uno pin

    1           O1   Logic Output 1      Not Connected
    2               clk                     13
    3           SDI     MOSI                11
    4           SDO     MISO                12  (pull up with a 1K - 10K resistor)
    5           GND     Logic Ground        GND
    6           Vss     Negative supply     GND
    7               T                       GND
    8           Terminal B                  Resistor Terminal B
    9           Wiper Terminal              Resistor's Wiper Terminal
    10          Terminal A                  Resistor Terminal A
    11          Vdd positive supply         5v
    12          WP Write Protect            5v
    13          PR Hardware Override        5V
    14      CS chip sellect / slave select  10
    15          RDY Ready                 Not Connected (add pull up resistor if you wish to use this pin)
    16          O2   Logic Output 2       Not Connected

*/
#include <AD5231.h> // SPI library is already included in this library so no need to add it a second time

AD5231 MyDigiPot(10); // salve select on pin 10

void setup()
{
  MyDigiPot.begin(); // SPI.begin is taken care of in this initilisation
  Serial.begin(9600);
  while (!Serial) 
  {
    ; // wait for serial port to connect
  }
  delay(100);

  Serial.print("Wiper Position: ");
  Serial.println (MyDigiPot.ReadWiper()); // read the position of the wiper

  delay(100);
  Serial.println("Read Memories");

  Serial.println("Memory Address:  Value ");
  // Read and print out the contence for each memory position
  for (int a = 0; a < 16; a++)
  {
    Serial.print("EEMEM "); //Electronically Errasible MEMory
    Serial.print(a);
    Serial.print(":           ");
    Serial.println (MyDigiPot.ReadMemory(a)); // read the value of the memory position
  }

  Serial.println("Saving the value 600 to Memory location 6 ");
  MyDigiPot.SaveData(6, 600); // save to memory location 6 the value 600 NB. each memory location can save two bytes like an interger.

  Serial.print("Reading Memory location 6, value is: ");

  int b = MyDigiPot.ReadMemory(6); // read memory location 6 and place it in an interger b
  Serial.println(b);//print the value of b to the serial port.

  Serial.println("Setting the potentiometer to midscale");
  MyDigiPot.WriteWiper (512); // Set the resistor to midscale

  /*
     Save the curent wiper position to memory so that when the digipot next powers on
     it will return to this position.
  */
  MyDigiPot.SaveWiperValue(); // Save Wipers position to memory0
  delay(50); 
  ResetDigiPot(); // Reset the digipot for the settings to take place
  delay(50);

  // set the logic pins O1 & O2 (pins 1 & 16) to Pin 1 HIGH and pin 16 LOW
  SetLogicOutput(true, false);
  delay(50);
  ResetDigiPot(); // Reset the digipot for the settings to take place
  delay(50);
}

void loop()
{
  MyDigiPot.WriteWiper (512); // Set the resistor to midscale

  Serial.println("increasing resistance and displaying the ten bit values");

  //Count up 20 places
  for (int I = 0 ; I <= 20; I++)
  {
    MyDigiPot.IncreaseWiper(); // Increase by one position

    Serial.print (MyDigiPot.ReadWiper()); // read the position of the wiper
    Serial.print(" : ");
    delay(3000);
  }

  delay(10000); //pause for 10 seconds before counting down
  Serial.println();
  Serial.println("decreasing resistance and displaying the ten bit values");

  //Count down 20 places
  for (int I = 0 ; I <= 20; I++)
  {
    MyDigiPot.DecreaseWiper(); // Decrease by one position

    Serial.print (MyDigiPot.ReadWiper()); // read the position of the wiper
    Serial.print(" : ");
    delay(3000);
  }
  Serial.println();

}
