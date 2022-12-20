// VilabUnit.h

#ifndef _VILABUNIT_h
#define _VILABUNIT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "IAcelerometro.h"
#include "IConection.h"

class VilabUnit {
public:
	VilabUnit(IAcelerometro& imu, IConnection& con);
	int initAll();
  void hola();

private:
	IAcelerometro& imu;
  IConnection& con;

};

#endif