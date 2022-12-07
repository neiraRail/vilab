// VilabUnit.h

#ifndef _VILABUNIT_h
#define _VILABUNIT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "IAcelerometro.h"

class VilabUnit {
public:
	VilabUnit(IAcelerometro& imu);
	int initAll();
  void hola();

private:
	IAcelerometro& imu;

};

#endif