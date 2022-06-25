/*
  Skecth : pm_bme280_esp8266.ino
  Description : Mesure pression atmospherique, temperature, hygrometrie 
                Avec capteur BME280 
  Création : PHMARDUINO le 25 juin 2022
  Date de derniere mise a jour : .........
  Bib utilisee :
   BME280 de Adafruit
*/

//Include des Library
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//Creation d un objet monCapteur de type BME280
Adafruit_BME280 bme;

void setup(){
  Serial.begin(115200);
  delay(100);
  bme.begin(0x76);   
}

void loop() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.print(" °C");
    Serial.print(" - Pression = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.print(" hPa");
    Serial.print(" - Humidite = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
   
    delay(1000); // delai de 1 sec
}
