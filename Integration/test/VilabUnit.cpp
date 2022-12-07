// 
// 
// 

#include "VilabUnit.h"

VilabUnit::VilabUnit(IAcelerometro& imu): imu { imu }
{
}

int VilabUnit::initAll()
{
	if (!imu.iniciar()) {
		return 0;
	}
	return 1;
}

void VilabUnit::hola(){
  Serial.print("Hola");
}


