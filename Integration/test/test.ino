#include "ESP32VilabUnit.h"
#include "Acc_MPU6050.h"

void setup(){
  Serial.begin(9600);
  
  Acc_MPU6050 imu;

  ESP32VilabUnit unit{imu};
  
  if(!unit.initAll()){
    ESP.restart();
  }

  unit.hola();
}
void loop(){

}
