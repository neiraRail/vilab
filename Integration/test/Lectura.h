//Lectura.h
#ifndef LECTURA_H
#define LECTURA_H

class Lectura
    {
        public:
            Lectura();
            Lectura(float acc[3], float gyro[3], float temp);
            float* getAcc();
            float* getGyro();
            float getTemp();
            void setValues(float* acc, float* gyro, float temp);
        private:
            float* acc;
            float* gyro;
            float temp;
    };

#endif // !LECTURA_H
