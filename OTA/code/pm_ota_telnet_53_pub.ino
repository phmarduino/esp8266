/*  Skecth : pm_ota_telnet_53
  Description : Terminal serie distant
  Auteur : PHMARDUINO
  Création : 21 juillet 2018
  Derniere mise a jour :
*/

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// A remplacer par le nom et le mot de passe de votre wifi
const char* ssid = "freeboxLM";
const char* password = "722AB93D9F";
  
WiFiServer TelnetServer(23); // Declaration Serveur Telnet
WiFiClient TelnetClient;     // Declaration Client Telnet
 
void setup() {
  // Connection au reseau WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); }

  // Demarrage serveur Telnet 
  TelnetServer.begin();
  TelnetServer.setNoDelay(true);
 
  // Demarrage OTA avec postionnement du nom
  ArduinoOTA.setHostname("mon_esp8266"); 
  ArduinoOTA.begin();
}
 
void loop() {
  ArduinoOTA.handle(); // recherche conneciton OTA
  Serial.println(millis()); // Temps ecoule depuis le demarrage de la carte em ms
  
  // Si nouvelle connection client Telnet
  if (TelnetServer.hasClient()) {
    TelnetClient = TelnetServer.available();       
    TelnetClient.flush();  // Nettoyage du buffer en entree
    TelnetClient.println("Bonjour, client Telnet detecte !!!!!");             
    TelnetClient.println("-----------------------------------");
  }

  // Si client Telnet toujours connecte
  if (TelnetClient || TelnetClient.connected()) {
    TelnetClient.print("Temps actuel : ");
    TelnetClient.println(millis());
  }
    
  delay(2000);  // Delai de deux secondes
}
 
