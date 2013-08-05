/****************************
 * GetRelativeHumidity
 *  An example sketch that reads the sensor and prints the
 *  relative humidity to the PC's serial port
 *
 *  Tested with the SHT21-Breakout
 *  Humidity sensor from Modern Device.
 *****************************/
#include <Wire.h>
#include <LibHumidity.h>

LibHumidity humidity = LibHumidity(0);

void setup() {
  Serial.begin(9600);
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);  //GND pin
  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH); //VCC pin
}


void loop() {
  Serial.print("RHumidity: ");
  Serial.print(humidity.GetHumidity());
  Serial.print(" Temp in C: ");
  Serial.print(humidity.GetTemperatureC());
  Serial.print(" Temp in F: ");
  Serial.println(humidity.GetTemperatureF());
}
