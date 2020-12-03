/*
  Skecth : pm_basic_ota_nodemcu_53
  Description : Initialisation OTA NodeMCU 53
  Auteur : PHMARDUINO  Création : 20 juillet 2018
  Derniere mise a jour :
  */

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// A remplacer par le nom et le mot de passe de votre wifi
const char* ssid = "votre nom de wifi";
const char* password = "votre mot de passe wifi";

void setup() {
  pinMode(2, OUTPUT);       // builtin led pour le NodeMCU cad GPIO02 ou D4
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("phm_esp8266");

  // No authentication by default
  ArduinoOTA.setPassword("toto");

  ArduinoOTA.onStart([]() {
    Serial.println("Starting ....  ");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
  digitalWrite(2, HIGH);    // turn the LED on (HIGH is the voltage level)  
  delay(1000);              // wait for a second
  digitalWrite(2, LOW);     // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
