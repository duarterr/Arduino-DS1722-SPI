/* ------------------------------------------------------------------------------------------- */
// DS1722 - Library for interfacing the DS1722 Digital Thermometer using SPI
// Version: 1.0
// Author: Renan R. Duarte
// E-mail: duarte.renan@hotmail.com
// Date:   October 8, 2019
// Released into the public domain
/* ------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------- */
// Includes
/* ------------------------------------------------------------------------------------------- */

#include "Arduino.h"
#include "SPI.h"
#include "DS1722_SPI.h"

/* ------------------------------------------------------------------------------------------- */
// Constructor
/* ------------------------------------------------------------------------------------------- */

DS1722::DS1722(unsigned int PinCs)
{
  // Set chip select pin as output
  pinMode(PinCs, OUTPUT);

  // Deselect device (active high)
  digitalWrite (PinCs, LOW);

  // Start SPI bus
  SPI.begin ();

  // Save CS pin bitmask
  _PinCs = PinCs;
}

/* ------------------------------------------------------------------------------------------- */
// Functions definitions
/* ------------------------------------------------------------------------------------------- */

// Name:        writeByte
// Description: Writes a byte to DS1722 memory
// Arguments:   Address - Register to be written
//              Data - Byte to be written
// Returns:     none

void DS1722::writeByte(unsigned char Address, unsigned char Data)
{
  // Configure SPI transaction
  SPI.beginTransaction(SPISettings(DS1722_SPI_CLOCK, MSBFIRST, SPI_MODE1));

  // Select device (active high)
  digitalWrite (_PinCs, HIGH); 

  // Send address byte
  SPI.transfer(Address);

  // Send data byte
  SPI.transfer(Data);
  
  // Deselect device (active high)
  digitalWrite (_PinCs, LOW);

  // End SPI transaction
  SPI.endTransaction();    
}

/* ------------------------------------------------------------------------------------------- */

// Name:        readByte
// Description: Reads a byte from DS1722 memory
// Arguments:   Address - Register to be read
// Returns:     Byte read

unsigned char DS1722::readByte (unsigned char Address)
{
  // Data byte
  unsigned char Data = 0;

  // Configure SPI transaction  
  SPI.beginTransaction(SPISettings(DS1722_SPI_CLOCK, MSBFIRST, SPI_MODE1));

  // Select device (active high)  
  digitalWrite (_PinCs, HIGH);

  // Send address byte
  SPI.transfer (Address); 

  // Read data byte (0xFF = dummy)
  Data = SPI.transfer (0xFF);

  // Deselect device (active high)
  digitalWrite (_PinCs, LOW);

  // End SPI transaction
  SPI.endTransaction();   

  // Return read byte
  return Data;
}

/* ------------------------------------------------------------------------------------------- */

// Name:        getResolution
// Description: Gets the current resolution of DS1722
// Arguments:   none
// Returns:     Resolution (8, 9, 10, 11 or 12)

unsigned char DS1722::getResolution ()
{
  // Get current value of resolution bits
  unsigned char RawResolution = readByte(DS1722_ADDR_READ_CFG) & DS1722_MASK_RESOLUTION;  

  // Return resolution according to bits value
  if (RawResolution == DS1722_RES_8)
    return 8;
  else if (RawResolution == DS1722_RES_9)
      return 9;
  else if (RawResolution == DS1722_RES_10)
      return 10;
  else if (RawResolution == DS1722_RES_11)
      return 11;
  else if (RawResolution == DS1722_RES_12)
      return 12;
  else
      return 0;
}

/* ------------------------------------------------------------------------------------------- */

// Name:        setResolution
// Description: Sets the resolution of DS1722
// Arguments:   Resolution (8, 9, 10, 11 or 12)
// Returns:     false if new resolution is equal to current or true on completion

bool DS1722::setResolution (unsigned char Resolution)
{
  // New configuration byte
  unsigned char NewCfg = readByte(DS1722_ADDR_READ_CFG);
  
  // Constrain desired value between 8 and 12
  unsigned char NewResolution = constrain(Resolution, 8, 12);

  // Check if new resolution is equal to current
  if (NewResolution == getResolution())
    return false;
  
  // Prepare new configuration byte
  if (NewResolution == 8)
    NewCfg = (NewCfg & ~DS1722_MASK_RESOLUTION) | (DS1722_RES_8 & DS1722_MASK_RESOLUTION);
  else if (NewResolution == 9)
    NewCfg = (NewCfg & ~DS1722_MASK_RESOLUTION) | (DS1722_RES_9 & DS1722_MASK_RESOLUTION);
  else if (NewResolution == 10)
    NewCfg = (NewCfg & ~DS1722_MASK_RESOLUTION) | (DS1722_RES_10 & DS1722_MASK_RESOLUTION);
  else if (NewResolution == 11)
    NewCfg = (NewCfg & ~DS1722_MASK_RESOLUTION) | (DS1722_RES_11 & DS1722_MASK_RESOLUTION);
  else if (NewResolution == 12)
    NewCfg = (NewCfg & ~DS1722_MASK_RESOLUTION) | (DS1722_RES_12 & DS1722_MASK_RESOLUTION);
  else
    return false;

  // Send new configuration byte
  writeByte (DS1722_ADDR_WRITE_CFG, NewCfg);

  // Success
  return true;
}

/* ------------------------------------------------------------------------------------------- */

// Name:        getMode
// Description: Gets the current conversion mode of DS1722
// Arguments:   none
// Returns:     DS1722_MODE_CONTINUOUS (logic 0) or DS1722_MODE_ONESHOT (logic 1)

bool DS1722::getMode ()
{
  // Return value of DS1722_MASK_SHUTDOWN bit
  return (readByte(DS1722_ADDR_READ_CFG) & DS1722_MASK_SHUTDOWN);  
}

/* ------------------------------------------------------------------------------------------- */

// Name:        setMode
// Description: Sets the conversion mode of DS1722
// Arguments:   DS1722_MODE_CONTINUOUS (logic 0) or DS1722_MODE_ONESHOT (logic 1)
// Returns:     false if new mode is equal to current or true on completion

bool DS1722::setMode (bool Mode)
{
  // Check if new mode is equal to current
  if (Mode == getMode())
    return false;

  // Get current configuration byte
  unsigned char NewCfg = readByte(DS1722_ADDR_READ_CFG);

  // Prepare new configuration byte
  if (Mode == DS1722_MODE_ONESHOT)
    NewCfg |= DS1722_MASK_SHUTDOWN;

  else
    NewCfg &= ~DS1722_MASK_SHUTDOWN;

  // Send new configuration byte
  writeByte (DS1722_ADDR_WRITE_CFG, NewCfg);

  // Success
  return true;
}

/* ------------------------------------------------------------------------------------------- */

// Name:        requestConversion
// Description: Requests a temperature conversion (only in one shot mode)
// Arguments:   none
// Returns:     false if mode is set to continuous or true on completion

bool DS1722::requestConversion ()
{
  // Check if current mode is set to one shot
  if (getMode() == DS1722_MODE_CONTINUOUS)
    return false;

  // Prepare new configuration byte
  unsigned char NewCfg = readByte(DS1722_ADDR_READ_CFG);
  NewCfg |= DS1722_MASK_ONESHOT;

  // Send new configuration byte
  writeByte (DS1722_ADDR_WRITE_CFG, NewCfg);

  // Success
  return true;
}

/* ------------------------------------------------------------------------------------------- */

// Name:        getTemperature
// Description: Returns the value of the temperature registers of DS1722 in Celsius
// Arguments:   none
// Returns:     Temperature in Celsius (-55C to 120C)

float DS1722::getTemperature ()
{
  // Variables to store the register values
  unsigned char TempLSB, TempMSB = 0;
  
  // Temperature
  float Temperature = 0;

  // Configure SPI transaction
  SPI.beginTransaction(SPISettings(DS1722_SPI_CLOCK, MSBFIRST, SPI_MODE1));

  // Select device (active high)
  digitalWrite (_PinCs, HIGH); 

  // Send address byte
  SPI.transfer(DS1722_ADDR_READ_LSB); 

  // Read two bytes (DS1722_ADDR_READ_LSB and DS1722_ADDR_READ_MSB)
  TempLSB = SPI.transfer(0xFF);
  TempMSB = SPI.transfer(0xFF);
  
  // Deselect device (active high)
  digitalWrite (_PinCs, LOW);

  // End SPI transaction
  SPI.endTransaction();  

  // If negative temperature, apply two's complement
  if(TempMSB & 0x80)
  {
    TempMSB = ~TempMSB + 0x01;
    TempLSB = ~TempLSB + 0x01;
	
	// Decimal part of the temperature is stored on 4 most significant bits of TempLSB
	TempLSB >>= 4;	

	// Convert to float and handle negative numbers
	Temperature = -((float)TempMSB) + (((float)TempLSB) / 16.0);  
  }
  
  // If positive
  else
  {
	// Decimal part of the temperature is stored on 4 most significant bits of TempLSB
	TempLSB >>= 4;	

	// Convert to float and handle negative numbers
	Temperature = ((float)TempMSB) + (((float)TempLSB) / 16.0);  	  
  }
 
  // Return temperature
  return (Temperature); 
}

/* ------------------------------------------------------------------------------------------- */
// End of code
/* ------------------------------------------------------------------------------------------- */
