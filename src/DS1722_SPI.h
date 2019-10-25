/* ------------------------------------------------------------------------------------------- */
// DS1722 - Library for interfacing the DS1722 Digital Thermometer using SPI
// Version: 1.0
// Author: Renan R. Duarte
// E-mail: duarte.renan@hotmail.com
// Date:   October 8, 2019
// Released into the public domain
/* ------------------------------------------------------------------------------------------- */

#ifndef DS1722_SPI_h
#define DS1722_SPI_h

/* ------------------------------------------------------------------------------------------- */
// Includes
/* ------------------------------------------------------------------------------------------- */

#include "Arduino.h"
#include "SPI.h"

/* ------------------------------------------------------------------------------------------- */
// Defines
/* ------------------------------------------------------------------------------------------- */

/// String with library name
#define DS1722_CODE_NAME		"DS1722_SPI"

/// String with library version
#define DS1722_CODE_VERSION		"1.0"			

/// Device SPI clock speed
#define DS1722_SPI_CLOCK        4000000

/// One shot conversion mode flag - Use as argumets to the setMode function
#define DS1722_MODE_ONESHOT     1

/// Continuous conversion mode flag - Use as argumets to the setMode function
#define DS1722_MODE_CONTINUOUS  0

/// Configuration register read address
#define DS1722_ADDR_READ_CFG		0x00

/// Configuration register write address
#define DS1722_ADDR_WRITE_CFG		0x80

/// Temperature LSB register read address
#define DS1722_ADDR_READ_LSB		0x01

/// Temperature MSB register read address
#define DS1722_ADDR_READ_MSB		0x02

/// One shot mode bit mask
#define DS1722_MASK_ONESHOT     0x10

/// Shutdown bit mask
#define DS1722_MASK_SHUTDOWN    0x01

/// Resolution bit mask
#define DS1722_MASK_RESOLUTION  0x0E

/// Device 8 bit resolution define - Do not pass as arguments to the setResolution function
#define DS1722_RES_8            0x0

/// Device 9 bit resolution define - Do not pass as arguments to the setResolution function
#define DS1722_RES_9            0x2

/// Device 10 bit resolution define - Do not pass as arguments to the setResolution function
#define DS1722_RES_10           0x4

/// Device 11 bit resolution define - Do not pass as arguments to the setResolution function
#define DS1722_RES_11           0x6

/// Device 12 bit resolution define - Do not pass as arguments to the setResolution function
#define DS1722_RES_12           0x8

/* ------------------------------------------------------------------------------------------- */
// DS1722 class
/* ------------------------------------------------------------------------------------------- */

class DS1722
{
  public:
    // Constructor
    DS1722 (unsigned int PinCs);

    // Resolution related functions
    unsigned char getResolution ();
    bool setResolution (unsigned char Resolution);

    // Mode related functions
    bool getMode ();
    bool setMode (bool Mode);

    // One shot related functions
    bool requestConversion ();

    // Temperature related functions
	  float getTemperature ();
    
  private:  
    // CS pin mask
    unsigned int _PinCs;

    // Device memory related functions
    void writeByte (unsigned char Address, unsigned char Data);
    unsigned char readByte (unsigned char Address); 
};

#endif

/* ------------------------------------------------------------------------------------------- */
// End of code
/* ------------------------------------------------------------------------------------------- */
