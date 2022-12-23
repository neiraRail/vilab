#include "Lectura.h"

Lectura::Lectura(){
    this->acc = 0;
    this->gyro = 0;
    this->temp = 0;
}

Lectura::Lectura(float* acc, float* gyro, float temp) {
    this->acc = acc;
    this->gyro = gyro;
    this->temp = temp;
}

float* Lectura::getAcc(){
    return this->acc;
}

float* Lectura::getGyro(){
    return this->gyro;
}

float Lectura::getTemp(){
    return this->temp;
}

void Lectura::setValues(float* acc, float* gyro, float temp){
    this->acc = acc;
    this->gyro = gyro;
    this->temp = temp;
}
