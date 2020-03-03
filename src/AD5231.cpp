/**************************************************************************/
/*
    @file     AD5231.cpp
    @author   Will White
**	@license  MIT (see license.txt)
	
	SPI Driver for Analog Device's AD5231 Digital Potentiometer

    Version 0.1 - First test release

    https://github.com/WD24/AD5231-Arduino-Library

    Data sheet can be found at: https://www.analog.com/en/products/ad5231.html#

*/
/**************************************************************************/



#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "SPI.h"

#include "AD5231.h"

/**************************************************************************/
/*! 
    @brief  Instantiates a new AD5231 class
*/
/**************************************************************************/
AD5231::AD5231(uint8_t SS) 
{
  slaveSelect = SS;
}

/**************************************************************************/
/* 
    @brief  Setups the DigiPot
*/
/**************************************************************************/
void AD5231::begin() 
{
  
  SPI.begin ( ) ;
  pinMode (slaveSelect, OUTPUT ) ;
  digitalWrite (slaveSelect, HIGH);

}
 
/**************************************************************************/
/* 
    @brief  Increases / decreases the Wiper's position by one place
    
    IncreaseWiper will increase the reaistance between the wiper and terminal B
    on the resistor Whilst also reducing the resistance between the wiper 
    and terminal A. DecreaseWiper will do the opposite.

    ShiftUp / ShiftDown will increase / decrease the resistance between Terminal B
    and the Wiper in blocks by shifting the bit value onplace left or right.
    It will have the inverse effect on the resistance between terminal A 
    and the wiper.
*/
/**************************************************************************/

void AD5231::IncreaseWiper() // Increment +1
{
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  //SPI.beginTransaction(SPISettings(AD5231Settings));
  digitalWrite ( slaveSelect , LOW ) ;

  SPI.transfer(0xE0);
  SPI.transfer(0x00);
  SPI.transfer(0x00);

  digitalWrite ( slaveSelect , HIGH ) ;
  SPI.endTransaction();
}
void AD5231::DecreaseWiper() // Decrement -1
{
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  //SPI.beginTransaction(SPISettings(AD5231Settings));
  digitalWrite ( slaveSelect , LOW ) ;

  SPI.transfer(0x60);
  SPI.transfer(0x00);
  SPI.transfer(0x00);

  digitalWrite ( slaveSelect , HIGH );
  SPI.endTransaction();
}
void AD5231::ShiftUp() // Increment +6db
{
  // Increases the Wipersetting by shifting all the bits one place left
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  //SPI.beginTransaction(SPISettings(AD5231Settings));
  digitalWrite ( slaveSelect , LOW ) ;

  SPI.transfer(0xC0);
  SPI.transfer(0x00);
  SPI.transfer(0x00);

  digitalWrite ( slaveSelect , HIGH ) ;
  SPI.endTransaction();
}
void AD5231::ShiftDown() // Decrement -6db
{
  // Decreases the Wipersetting by shifting all the bits one place right
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  //SPI.beginTransaction(SPISettings(AD5231Settings));
  digitalWrite ( slaveSelect , LOW ) ;

  SPI.transfer(0x40);
  SPI.transfer(0x00);
  SPI.transfer(0x00);

  digitalWrite ( slaveSelect , HIGH ) ;
  SPI.endTransaction();
}
/**************************************************************************/
/* 
    @brief  Reads the data one of the 16 memory locations (0-15)

    @param[in]  input
                Address for one of the 16 two bit memory locations.
    @param[in]  output
                Unsigned int representing the two bytes of data held in the 
                memory location.
    
*/
/**************************************************************************/
int16_t AD5231::ReadMemory(int Address)
{
  //Read EEMEM at the address specified
  if (Address < 0 || Address > 15 )
  {

  }
  else
  {    
    int addr = 144  + Address;
    
    // Send the Read RDAC register command
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));    
    //SPI.beginTransaction(SPISettings(AD5231Settings));
    digitalWrite ( slaveSelect , LOW ) ;  
    SPI.transfer(addr);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    digitalWrite ( slaveSelect , HIGH ) ;
    //SPI.endTransaction();

    delay(50);

    // Send the do nothing command and read the incomming data
    //SPI.beginTransaction(SPISettings(AD5231Settings));
    digitalWrite ( slaveSelect , LOW ) ;
    byte BYTE_A = SPI.transfer(0x00);
    byte BYTE_B = SPI.transfer(0x00);
    byte BYTE_C = SPI.transfer(0x00);
    digitalWrite ( slaveSelect , HIGH ) ;
    SPI.endTransaction();

    // convert the two data bytes in to a single 16 bit integer
    unsigned int D = (BYTE_B << 8) + BYTE_C;
    // Return data back as an integer
    return D;
  }
}
/**************************************************************************/
/* 
    @brief  Reads the current wiper position

    @param[in]  output
                Unsigned int 10bit wiper position
    
*/
/**************************************************************************/
int16_t  AD5231::ReadWiper()
{
  // Send the Read RDAC register command
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  //SPI.beginTransaction(SPISettings(AD5231Settings));
  digitalWrite ( slaveSelect , LOW ) ;

  SPI.transfer(0xA0);
  SPI.transfer(0x00);
  SPI.transfer(0x00);

  digitalWrite ( slaveSelect , HIGH ) ;
  //SPI.endTransaction();

  delay(50);

  // Send the do nothing command and read the incomming data

  //SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  //SPI.beginTransaction(SPISettings(AD5231Settings));
  digitalWrite ( slaveSelect , LOW ) ;

  byte BYTE_A = SPI.transfer(0x00);
  byte BYTE_B = SPI.transfer(0x00);
  byte BYTE_C = SPI.transfer(0x00);

  digitalWrite ( slaveSelect , HIGH ) ;
  SPI.endTransaction();

  // convert the two data bytes in to a single 16 bit integer
  unsigned int D = (BYTE_B << 8) + BYTE_C;

  // Return data back as an integer
  return D;
}
/**************************************************************************/
/* 
    @brief  Writes a 10 bit value to set the wiper position
    
*/
/**************************************************************************/
void  AD5231::WriteWiper (int Val)
{
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  //SPI.beginTransaction(SPISettings(AD5231Settings));
  digitalWrite ( slaveSelect , LOW ) ;
  //SPI.transfer(0b10110000);
  SPI.transfer(0xB0);
  SPI.transfer16(Val);
  digitalWrite ( slaveSelect , HIGH ) ;
  SPI.endTransaction();
}
/**************************************************************************/
/* 
    @brief  Writes to both logic outputs (O1 & O2) on pins 1 & 16.
            The chip needs either to be power cycled or a RestoreWiper command
            issued for changes to take place.
    
*/
/**************************************************************************/
void  AD5231::SetLogicOutput(bool O1, bool O2)
{
  // Set the Digital Output for O1 & O2 pins 1 & 16
  //the AD5231 needs to be power cycled for effect to take place

  // O1 Low O2 Low
  if (O1 == false && O2 == false)
  {
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    //SPI.beginTransaction(SPISettings(AD5231Settings));
    digitalWrite ( slaveSelect , LOW ) ;
    SPI.transfer(0x31);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    digitalWrite ( slaveSelect , HIGH ) ;
    SPI.endTransaction();
  }

  // O1 High O2 Low
  else if (O1 == true && O2 == false)
  {
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    //SPI.beginTransaction(SPISettings(AD5231Settings));
    digitalWrite ( slaveSelect , LOW ) ;
    SPI.transfer(0x31);
    SPI.transfer(0x00);
    SPI.transfer(0x01);
    digitalWrite ( slaveSelect , HIGH ) ;
    SPI.endTransaction();
  }

  // O1 Low O2 High
  else if (O1 == false && O2 == true)
  {
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    //SPI.beginTransaction(SPISettings(AD5231Settings));
    digitalWrite ( slaveSelect , LOW ) ;
    SPI.transfer(0x31);
    SPI.transfer(0x00);
    SPI.transfer(0x02);
    digitalWrite ( slaveSelect , HIGH ) ;
    SPI.endTransaction();
  }

  // O1 High O2 High
  else if (O1 == true && O2 == true)
  {
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    //SPI.beginTransaction(SPISettings(AD5231Settings));
    digitalWrite ( slaveSelect , LOW ) ;
    SPI.transfer(0x31);
    SPI.transfer(0x00);
    SPI.transfer(0x03);
    digitalWrite ( slaveSelect , HIGH ) ;
    SPI.endTransaction();
  }
}
/**************************************************************************/
/* 
    @brief  Resets the DigiPot to power on conditions
    
*/
/**************************************************************************/
void  AD5231::ResetDigiPot()
{
  // restore wiper from memory
  // Reset EEMEM content to RDAC.
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  //SPI.beginTransaction(SPISettings(AD5231Settings));
  digitalWrite ( slaveSelect , LOW ) ;
  SPI.transfer(0x80);// restores the RDAC from EEMEM(0)
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  digitalWrite ( slaveSelect , HIGH );
  SPI.endTransaction();

}
/**************************************************************************/
/* 
    @brief  Saves the current wiper position to memory for the next power on.
    
*/
/**************************************************************************/
void  AD5231::SaveWiperValue ()
{
  //Store RDAC setting to EEMEM.
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  //SPI.beginTransaction(SPISettings(AD5231Settings));
  digitalWrite ( slaveSelect , LOW ) ;
  SPI.transfer(0x20);// Stores the curent RDAC to EEMEM(0)
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  digitalWrite ( slaveSelect , HIGH );
  SPI.endTransaction();
}
/**************************************************************************/
/* 
    @brief  Saves data to one of the 16 (2 byte) memory locations (0-15)
            
            NB. Location 0 is the memory for the wiper position and 
            Location 1 for the logic outputs.
    
*/
/**************************************************************************/
void  AD5231::SaveData(int Address, unsigned int Data)
{
  //Store Data to EEMEM
  int addr = 48 + Address;

  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  //SPI.beginTransaction(SPISettings(AD5231Settings));
  digitalWrite ( slaveSelect , LOW ) ;
  SPI.transfer(addr);
  SPI.transfer16(Data);
  digitalWrite ( slaveSelect , HIGH ) ;
  //SPI.endTransaction();

  delay(50);

  //SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  //SPI.beginTransaction(SPISettings(AD5231Settings));
  digitalWrite ( slaveSelect , LOW ) ;
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  digitalWrite ( slaveSelect , HIGH ) ;
  SPI.endTransaction();
}
