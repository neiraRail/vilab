// VilabUnit.h

#ifndef ESP32VILABUNIT_h
#define ESP32VILABUNIT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "IAcelerometro.h"

class ESP32VilabUnit {
public:
	ESP32VilabUnit(IAcelerometro& imu);
	int initAll(); //WiFi, Acc, FS y 
  void hola();

private:
	IAcelerometro& imu;
  int iniciarWiFi();
  int iniciarFS();
  char* ssid;
  char* password;

};

#endif