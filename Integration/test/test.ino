#include "VilabUnit.h"
#include "Acc_MPU6050.h"

void setup(){
  Serial.begin(9600);
  Acc_MPU6050 imu;
  VilabUnit unit{imu};
  unit.initAll();

  unit.hola();
}
void loop(){

}
