#include "ESPWiFi.h"
#include <WiFi.h>

ESPWiFi::ESPWiFi(char* ssid, char* password){
  this->ssid = ssid;
  this->password = password;
}

int ESPWiFi::conectar(){
  unsigned long mytime;
  
  Serial.print("Intentando conectando a la siguiente red: ");
  Serial.println(this->ssid);
  WiFi.begin(this->ssid, this->password);
  mytime = millis();
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if(millis() - mytime > 5000){
        return 0;
      }
  }
    Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  return 1;
}