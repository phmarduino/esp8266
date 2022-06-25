/*
  Skecth : pm_bh1750-bme280_esp8266.ino
  Description : Station de mesure qualite environnement V1
                Mesure pression atmospherique, temperature, hygrometrie et luminosite
                Avec des capteurs BME280 et BH1750 et remontée dans thingspeak toutes les 2 mn
  Création : PHMARDUINO le 25 juin 2022
  Date de derniere mise a jour : .........
  Bib utilisees :
   BME280 de Sparkfun
   BH1750 de claws 
*/

//Include des Library
#include <stdint.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <BH1750.h>

//Creation d un objet lightMeter de type BH1750
BH1750 lightMeter;
//Creation d un objet monCapteur de type BME280
Adafruit_BME280 bme;

void setup(){
  // Initialisation liaison serie
  Serial.begin(115200);
  delay(100);
  // Demarrage BME280  
  bme.begin(0x76);  
  // Init bus I2C 
  Wire.begin();
  // Demarrage BH1750
  lightMeter.begin();
}

void loop() {
  // Mesure temperature, pression et humidite   
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.print(" °C");
  Serial.print(" - Pression = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.print(" hPa");
  Serial.print(" - Humidite = ");
  Serial.print(bme.readHumidity());
  Serial.print(" %");
  // Mesure luminosite    
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print(" - Luminosite : ");
  Serial.print(lux);
  Serial.println(" lx");
   
  delay(1000); // delai de 1 sec
}
