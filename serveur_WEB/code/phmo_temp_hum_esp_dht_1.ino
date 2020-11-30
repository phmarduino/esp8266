/*
  Objet : Serveur web temperature et humidité avec un esp8266-01
  Nom : phmo_temp_hum_esp_dht_1.ino
  Infos sketch :
  - Créé le 12 jan 2016 sur la base des exemples des bibliotheques Arduino par PHMO
*/

// ajout des bibs ESP8266 WiFi et sonde dht
#include <ESP8266WiFi.h>
#include "DHT.h"

// la sonde est de type dht22
#define DHTTYPE DHT22     

// la sonde dht est connectee sur le port GPIO2 de l esp8266-01
const int DHTPIN=2;

// remplacer par le nom et mot de passe de votre reseau wifi
const char* ssid = "votre nom wifi";
const char* password = "votre mot de passe wifi";

// definition d'un serveur web ecoutant sur le port 80
WiFiServer server(80);

// instanciation objet dht
DHT dht(DHTPIN, DHTTYPE); // Instanciation objet dht

// variables de travail
  static char celsiusTemp[7];
  static char humidityTemp[7];

// traitements init
void setup() {
  // init liaison serie
  Serial.begin(115200);
  delay(10);
  // init catpeur dht
  dht.begin();

  // connexion au wifi
  Serial.println();
  Serial.print("lancement connexion au reseau wifi :  ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("connexion au WiFi OK");

  // demarrage serveur web
  server.begin();
  Serial.println("le serveur web est lancé, attente fourniture ip locale de l esp ...");
  delay(10000);

  // affichage adresse ip locale
  Serial.println(WiFi.localIP());
}

// boucle de traitement
void loop() {
    // attente client web
    WiFiClient client = server.available();
    if (client) {
        Serial.println("detection dune nouvelle demande client web .....");
      // bolean to locate when the http request ends
      boolean blank_line = true;
      while (client.connected()) {
          if (client.available()) {
             char c = client.read();
             if (c == '\n' && blank_line) {
                    float h = dht.readHumidity();         // lecture humidite
                    float t = dht.readTemperature();      // lecture temperature
                    if (isnan(h) || isnan(t)) {           // verif si lecture dht22 ok
                      Serial.println("impossible de lire les donnees du dht22 !!!!!!!");
                      strcpy(celsiusTemp, "probleme");
                      strcpy(humidityTemp, "probleme");
                    }
                    else {
                      // lecture de la sonde ok, envoi des elements sur le port serie
                      float hic = dht.computeHeatIndex(t, h, false);
                      dtostrf(hic, 6, 2, celsiusTemp);
                      dtostrf(h, 6, 2, humidityTemp);
                      // envoi des message sur la console serie
                      Serial.print("Humidite : ");
                      Serial.print(humidityTemp);
                      Serial.print(" %\t Temperature : ");
                      Serial.print(celsiusTemp);
                      Serial.print(" *C ");
                    }
                       // envoi des données au client web
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    // your actual web page that displays temperature and humidity
                    client.println("<!DOCTYPE HTML>");
                    client.println("<html>");
                    client.println("<head></head><body><h1>ESP8266 - Temperature and Humidite</h1><h3>Temperature en Celsius : ");
                    client.println(celsiusTemp);
                    client.println("*C</h3><h3>Humidite : ");
                    client.println(humidityTemp);
                    client.println("%</h3><h3>");
                    client.println("</body></html>");
                    break;
              }
              if (c == '\n') {
                // premiere ligne a blanc
                blank_line = true;
              }
              else if (c != '\r') {
                // lignes suivantes avec du contenu
                blank_line = false;
             }
         }
   }
   // fermeture connexion client
   delay(1);
   client.stop();
   Serial.println("client web deconnecte ..........");
 }
}


 
