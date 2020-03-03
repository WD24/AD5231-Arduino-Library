/**************************************************************************/
/*
    @file     AD5231.h
    @author   Will White
**	@license  MIT (see license.txt)
	
	SPI Driver for Analog Device's AD5231 Digital Potentiometer

    Version 0.1 - First test release

    https://github.com/WD24/AD5231-Arduino-Library

    Data sheet can be found at: https://www.analog.com/en/products/ad5231.html#

*/
/**************************************************************************/

#ifndef AD5231_h
#define AD5231_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class AD5231
{
 public:
  AD5231(uint8_t SS);
  void begin(); 
  void IncreaseWiper(); // Increment +1
  void ShiftUp(); // Increment +6db
  void DecreaseWiper(); // Decrement -1
  void ShiftDown(); // Decrement -6db
  int16_t  ReadMemory(int Address); //Read the data in the momory address 0 - 15
  int16_t  ReadWiper(); //Reads the current wiper position
  void WriteWiper (int Val); //Writes a 10 bit value to set the wiper position
  void SetLogicOutput(bool O1, bool O2); //Sets the outputs on the logic pins
  void ResetDigiPot(); // Reset Pot
  void SaveWiperValue (); // Saves the current wiper position to memory
  void SaveData(int Address, unsigned int Data); // Saves data to one of the 16 memory locations

 private:
  uint8_t slaveSelect;
  //SPISettings AD5231Settings(int16_t 1000000, MSBFIRST, SPI_MODE0);
//SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0))
};

#endif
