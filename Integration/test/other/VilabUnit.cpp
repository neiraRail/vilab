// 
// 
// 

#include "VilabUnit.h"

VilabUnit::VilabUnit(IAcelerometro& imu, IConnection& con): imu { imu }, con { con }
{
}

int VilabUnit::initAll()
{
	if (!imu.iniciar()) {
		return 0;
	}
  if(!con.conectar()){
    return 0;
  }
	return 1;



}

void VilabUnit::hola(){
  Serial.print("Hola");
}


