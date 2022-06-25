/*
  Skecth : pm_bh1750_esp8266.ino
  Description : capteurs BH1750 loumiere
  Création : PHMARDUINO le 25 juin 2022
  Date de derniere mise a jour : .........
  Bib utilisees :
   BH1750 de claws 
*/

//Include des Library
#include <stdint.h>
#include <Wire.h>
#include <BH1750.h>

//Creation d un objet lightMeter de type BH1750
BH1750 lightMeter;

void setup(){
  // Initialisation liaison serie
  Serial.begin(9600);
  Serial.println("Mesure lumiere avec bh1750");
  // Init bus I2C 
  Wire.begin();
  
  // Demarrage BH1750
  lightMeter.begin();
}

void loop() {
  // Mesure luminosite    
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Luminosite : ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(3000); // delai de 3 sec
}
