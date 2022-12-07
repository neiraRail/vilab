#include "IAcelerometro.h"
#include <Adafruit_MPU6050.h>

class Acc_MPU6050 : public IAcelerometro
{
public:
	// Heredado via IAcelerometro
	void leerDatos(Lectura* lectura) override;

	int iniciar() override;

	bool detectarMovimiento() override;
private:
	Adafruit_MPU6050 mpu;

};
