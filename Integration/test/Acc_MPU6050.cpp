#include "Acc_MPU6050.h"

int Acc_MPU6050::iniciar() {
	Serial.println("Buscando aceler�metro MPU6050...");
	if (!this->mpu.begin()) {
		Serial.println(F("Failed to find MPU6050 chip"));
		while (1) {
			delay(10);
		}
	}
	Serial.println("MPU6050 encontrado");

	//Establecer el rango del aceler�metro y del girosc�pio
	this->mpu.setAccelerometerRange(MPU6050_RANGE_4_G); //Rango del aceler�metro
	this->mpu.setGyroRange(MPU6050_RANGE_250_DEG);       //Rango del girosc�pio

	//Establecer filtros del aceler�metro
	//mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
	//mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);

	//Establecer configuraci�n del motion detection
	this->mpu.setMotionDetectionThreshold(2);   //Threshold para la detecci�n de movimiento (G)
	this->mpu.setMotionDetectionDuration(2); //Duraci�n m�nima del evento para la detecci�n (ms)

	//Aun no s� para qu� se utiliza en.
	this->mpu.setInterruptPinLatch(true);  // Keep it latched.  Will turn off when reinitialized.
	this->mpu.setInterruptPinPolarity(true);
	this->mpu.setMotionInterrupt(true);
	
	return 1;
}

void Acc_MPU6050::leerDatos(Lectura* lectura) {
	sensors_event_t a, g, temp;
	mpu.getEvent(&a, &g, &temp);
	lectura->setValues(a.acceleration.v, g.gyro.v, temp.temperature);
}

bool Acc_MPU6050::detectarMovimiento() {
	return this->mpu.getMotionInterruptStatus();

}



