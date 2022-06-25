/*
 * Scanner I2C pour board à base de ESP8266
 * Creation phmarduino le 25 06 2022
 * Les pins I2C :
 * D1 = SCL
 * D2 = SDA
 */

#include <Wire.h>
const int sclPin = D1;
const int sdaPin = D2;

void setup()
{
  Wire.begin(sdaPin, sclPin);
  Serial.begin(9600);
  Serial.println("Scanner I2C  : ");
}

void loop()
{
  byte error, address;
  int nDevices;

  Serial.println("Lancement du scan I2C ESP8266 ...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("Materiel I2C trouvé à l adresse : ");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println(" !");

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Erreur à l adresse : ");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("Pa de mateirle I2C trouvé\n");
  }
  else {
    Serial.println("Fin du scan.\n");
  }

  delay(2000);
}
