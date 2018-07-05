/*
  LibHumidity - A Humidity Library for Arduino.

  Supported Sensor modules:
    SHT21-Breakout Module - http://www.moderndevice.com/products/sht21-humidity-sensor

  Created by Christopher Ladden at Modern Device on December 2009.
  modified by Paul Badger March 2010

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <inttypes.h>
#include <Wire.h>
//#include <wiring.h>
#include "LibHumidity.h"
#include "Arduino.h"

/******************************************************************************
 * Constructors
 ******************************************************************************/

/**********************************************************
 * Initialize the sensor based on the specified type.
 **********************************************************/
LibHumidity::LibHumidity(uint8_t sensorType) {
    Wire.begin();
    readDelay = 100;
}

/******************************************************************************
 * Global Functions
 ******************************************************************************/

/**********************************************************
 * GetHumidity
 *  Gets the current humidity from the sensor.
 *
 * @return float - The relative humidity in %RH
 **********************************************************/
float LibHumidity::GetHumidity(void) {

    float humidity;

    humidity = calculateHumidity(readSensor(eRHumidityHoldCmd),
                                 readSensor(eTempHoldCmd));

    return humidity;
}

/**********************************************************
 * GetTemperatureC
 *  Gets the current temperature from the sensor.
 *
 * @return float - The temperature in Deg C
 **********************************************************/
float LibHumidity::GetTemperatureC(void) {

    float temperature;

    temperature = calculateTemperatureC(readSensor(eTempHoldCmd));

    return temperature;
}

/**********************************************************
 * GetTemperatureF
 *  Gets the current temperature from the sensor.
 *
 * @return float - The temperature in Deg F
 **********************************************************/
float LibHumidity::GetTemperatureF(void) {
	
    float temperature;
	
    temperature = calculateTemperatureF(readSensor(eTempHoldCmd));
	
    return temperature;
}

/**********************************************************
 * SetReadDelay
 *  Set the I2C Read delay from the sensor.
 *
 *  The SHT21 humidity sensor datasheet says:
 *  Parameter Resolution typ max Units
 *    14 bit      66        85      ms
 *    13 bit      33        43      ms
 *    12 Bit      17        22      ms
 *    11 bit       8        11      ms
 *    10 bit       4         6      ms
 *
 *      Measurement time
 *      (max values for -40°C
 *        125°C.)
 *      8 bit 1 3 ms
 *
 **********************************************************/
void LibHumidity::SetReadDelay(uint16_t delay) {
    readDelay = delay;
}

/******************************************************************************
 * Private Functions
 ******************************************************************************/

uint16_t LibHumidity::readSensor(uint8_t command) {

    uint16_t result;

    Wire.beginTransmission(eSHT21Address);   //begin
    Wire.write(command);                      //send the pointer location
    delay(readDelay);
    Wire.endTransmission();                  //end

    Wire.requestFrom(eSHT21Address, 3);
    while(Wire.available() < 3) {
      ; //wait
    }

    //Store the result
    result = ((Wire.read()) << 8);
    result += Wire.read();
	result &= ~0x0003;   // clear two low bits (status bits)
    return result;
}


float LibHumidity::calculateTemperatureC(uint16_t analogTempValue) {

  float st;
  float temperatureC;

st = analogTempValue;

  temperatureC =  (((175.72 * (float)analogTempValue) / 65536.0) - 46.85); //T= -46.85 + 175.72 * ST/2^16
  return temperatureC;
}

float LibHumidity::calculateTemperatureF(uint16_t analogTempValue) {
	
	float st;
	float temperatureF;
	
	st = analogTempValue;
	temperatureF =  ((((175.72 * (float)analogTempValue) / 65536.0) - 46.85) * 9.0 / 5.0 ) + 32.0; //T= -46.85 + 175.72 * ST/2^16
	return temperatureF;
}


float LibHumidity::calculateHumidity(uint16_t analogHumValue, uint16_t analogTempValue) {

float srh = analogHumValue;
float humidityRH;                       // variable for result

//-- calculate relative humidity [%RH] --

 humidityRH = -6.0 + (125.0/65536.0 * srh);       // RH= -6 + 125 * SRH/2^16
 return humidityRH;
}


