#include "ESP32VilabUnit.h"
#include <WiFi.h>
#include <SPIFFS.h>

ESP32VilabUnit::ESP32VilabUnit(IAcelerometro& imu): imu { imu }
{
}

int ESP32VilabUnit::iniciarFS(){
  Serial.println("Incializando SPIFFS...");
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return 0;
  }
  Serial.println("SPIFFS montado correctamente");
  return 1;
}

int ESP32VilabUnit::iniciarWiFi(){
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

int ESP32VilabUnit::initAll()
{
	if (!imu.iniciar()) {
		return 0;
	}
  if(!this->iniciarWiFi()){
    return 0;
  }
  if(!this->iniciarFS()){
    return 0;
  }
  
	return 1;

}

void ESP32VilabUnit::hola(){
  Serial.print("Hola");
}


