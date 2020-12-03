/*
  Skecth : pm_station_v1_live.ino
  Description : Station de mesure qualite environnement V1
                Mesure pression atmospherique, temperature, hygrometrie et luminosite
                Avec des capteurs BME280 et BH1750
  Auteur : PHMARDUINO
  Date création : 21 mai 2018
  Date de derniere mise a jour : 18 juillet 2018
  Bib utilisees :
   BME280 de Sparkfun
   BH1750 de claws 
*/

//Include des Library
#include <stdint.h>
#include "SparkFunBME280.h"
#include <Wire.h>
#include <BH1750.h>
#include <ESP8266WiFi.h>

// A remplacer par l'API Key de votre "channel" thingspeak
String apiKey = "votre api key";

// A remplacer par le nom et le mot de passe de votre wifi
const char* ssid = "votre ssid de votre wifi";
const char* password = "votre mot de passe de votre wifi";

// URL serveur thingspeak pour la remontee des donnees
const char* server = "api.thingspeak.com";

// Duree de la mise en veille en secondes, ici 2 mn
const int tempsveille = 120;

// Pour pouvoir mesurer getVcc avec le CAN (ADC) interne
ADC_MODE(ADC_VCC);

// Creation d un objet client WIFI
WiFiClient client;
//Creation d un objet lightMeter de type BH1750
BH1750 lightMeter;
//Creation d un objet monCapteur de type BME280
BME280 monCapteur;

void setup(){
  // Initialisation liaison serie
  Serial.begin(9600);
  Serial.println("pm_hass_v1_free_lolin-11042020.ino sur TP-P50");
  
  //Initialisation donnees du capteur BME280
  monCapteur.settings.commInterface = I2C_MODE;
  monCapteur.settings.I2CAddress = 0x76;

  //Configuration du capteur BME280
  monCapteur.settings.runMode = 3;         // Mode normal, le capteur passe en mode sleep entre deux mesures
  monCapteur.settings.tStandby = 0;        // 0.5 ms entre deux mesures
  monCapteur.settings.filter = 0;          // Filter off, pas de filtrage en cas de variation brusque
  monCapteur.settings.tempOverSample = 1;  // Suréchantillonnage x 1 cad resolution de 0.0050°C (16 bits)
  monCapteur.settings.pressOverSample = 1; // Suréchantillonnage x 1 cad Résolution de 2.62 Pa (16 bits)
  monCapteur.settings.humidOverSample = 1; // Suréchantillonnage x 1
  
  delay(10);  //Delai attente demarrage capteur, le BME280 demande 2ms mini pour démarrer
              //Ici delai de 10 ms
  
  Serial.print("ID du capteur BME 280 : ");
  Serial.println(monCapteur.begin(), HEX); // Chargement configuration BME280

  // Demarrage BH1750
  lightMeter.begin();

  // Connexion WIFI
  Serial.print("Connexion en cours a :  ");
  Serial.println(ssid);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connexion au wifi ok ...");
}

void loop() {
  // Mesure temperature, pression et hygrometrie
  float h = monCapteur.readFloatHumidity();        // Recup hygrometrie
  float t = monCapteur.readTempC();                // Recup temperature
  float p = monCapteur.readFloatPressure()/100;    // Recup pression atmo

  // Mesure luminosite    
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Luminosite : ");
  Serial.print(lux);
  Serial.println(" lx");

  // Mesure tension alimentation ESP
  float mesureADC = ESP.getVcc();
  float tension=mesureADC/1023.00; // cf ADC sur 10 bits
  tension=tension*3.27/2.89; // cf calibrage de mon NodeMCU
  Serial.print("Tension alimentation : ");
  Serial.print(tension);
  Serial.println(" V");

  // Remontee des donnees sur le serveur thingspeak
  if (client.connect(server,80)) {  // Si connexion au serveur thingspeak OK
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(t);
           postStr +="&field2=";
           postStr += String(h);
           postStr +="&field3=";
           postStr += String(p);
           postStr +="&field4=";
           postStr += String(lux);
           postStr +="&field5=";
           postStr += String(tension);
           postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr);
  }
  client.stop();
  
  // Thingspeak demande un minimum de 15 secondes entre deux remontees de donnees, 
  delay(600000); // delai de 10 mn

  //Mise en veille cf mode deep sleep, tempsveille est en secondes
  ESP.deepSleep(tempsveille * 1000000);

  Serial.println("En attente avant la prochaine lecture...");
}
