/* ------------------------------------------------------------------------------------------- */
// GetTempC - This example gets the DS1722 temperature in Celsius
// Author:  Renan R. Duarte
// E-mail:  duarte.renan@hotmail.com
// Date:    October 14, 2019
//
// Released into the public domain
/* ------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------- */
// Libraries
/* ------------------------------------------------------------------------------------------- */

#include <DS1722_SPI.h> // https://github.com/duarterr/Arduino-DS1722-SPI

/* ------------------------------------------------------------------------------------------- */
// Hardware defines
/* ------------------------------------------------------------------------------------------- */

// Peripheral pins
#define PIN_TS_CS               5

/* ------------------------------------------------------------------------------------------- */
// Constructor
/* ------------------------------------------------------------------------------------------- */

// Temperature sensor constructor
DS1722 Temp (PIN_TS_CS);

/* ------------------------------------------------------------------------------------------- */
// Initialization function
/* ------------------------------------------------------------------------------------------- */

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println();
  Serial.print("Code date: ");
  Serial.print(__DATE__);
  Serial.print(" - ");
  Serial.println(__TIME__);

  /* ----------------------------------------------------------------------------------------- */

  // The device is initialized when the constructor is defined. 
  // At this point, device is configured in oneshot mode @ 9 bits

  // Print DS1722 mode
  Serial.print ("Mode = ");
  if (Temp.getMode () == DS1722_MODE_CONTINUOUS)
    Serial.println ("Continuous");
  else if (Temp.getMode () == DS1722_MODE_ONESHOT)
    Serial.println ("Oneshot");
  else
    Serial.println ("Error - Should never enter here!");

  // Print DS1722 resolution
  Serial.print ("Res = ");
  Serial.println (Temp.getResolution());
  
  // Set DS1722 resolution
  Temp.setResolution (8);

  // Print DS1722 resolution
  Serial.print ("Res = ");
  Serial.println (Temp.getResolution());
  
  // Require a temperature conversion - Will be ready in 0.075s @ 8 bits 
  Temp.requestConversion ();

  // Wait for conversion to end
  delay (100);

  // Print result
  Serial.print ("Oneshot temp = ");
  Serial.println (Temp.getTemperature());

  /* ----------------------------------------------------------------------------------------- */  

  // Set DS1722 mode
  Temp.setMode (DS1722_MODE_CONTINUOUS);

  // Print DS1722 mode
  Serial.print ("Mode = ");
  if (Temp.getMode () == DS1722_MODE_CONTINUOUS)
    Serial.println ("Continuous");
  else if (Temp.getMode () == DS1722_MODE_ONESHOT)
    Serial.println ("Oneshot");  

  // Set DS1722 resolution
  Temp.setResolution (12);

  // Print DS1722 resolution
  Serial.print ("Res = ");
  Serial.println (Temp.getResolution());  
  
  // At this point the device will continuously sample the temperature
  // Each sample takes 1.2s @ 12 bits   
}

/* ------------------------------------------------------------------------------------------- */
// Loop function
/* ------------------------------------------------------------------------------------------- */

void loop()
{ 
  // Print result
  Serial.print ("Continuous temp = ");
  Serial.println (Temp.getTemperature()); 

  // Wait for a bit
  delay (1500);    
}

/* ------------------------------------------------------------------------------------------- */
// End of code
/* ------------------------------------------------------------------------------------------- */