#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>


Adafruit_MPU6050 mpu;
void setup() {
  //Empezar y esperar el monitor serial
  Serial.begin(115200);
  while (!Serial)
    delay(100);
  delay(1000);
  Serial.println(F("Inicializando sistema"));
  //Comprobar chip MPU6050 acelerometro
  if (!mpu.begin()) {
    Serial.println(F("Failed to find MPU6050 chip"));
    while (1) {
      delay(10);
    }
  }
  //Serial.println("MPU6050 encontrado");
  
  //Establecer el rango del acelerómetro y del giroscópio
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  
  //mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  //mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  //Establecer configuración del motion detection
  mpu.setMotionDetectionThreshold(2);
  mpu.setMotionDetectionDuration(2);
  
  //Aun no sé para qué se utiliza.
  mpu.setInterruptPinLatch(true);  // Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
}

//Flag de evento
boolean plottear = false;
boolean hayFile = false;
//contador de tiempo
unsigned long start;
void loop() {
  // Leer datos del acelerometro
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  //Acciones a realizar si está ocurriendo un evento
  float acc;
  if(plottear){
     acc = sqrt(a.acceleration.x*a.acceleration.x + a.acceleration.y*a.acceleration.y + a.acceleration.z*a.acceleration.z);
     Serial.println(acc);
     // Aqui debe ir la escritura de la lectura en un archivo.
  }
  
  //Detectar evento y cambiar flag plottear
  if(mpu.getMotionInterruptStatus() && !plottear){
    start = millis();
    plottear = true;
  }
  //Finlizar evento despues de 3 segundos o dar 3 segundos más si 
  //Una alterantiva mejor puede ser llevar un contador y subirlo y bajarlo dependiendo de si hay movimiento o no, cambiar la flag cuando el contador llegue a cero
  if(millis()-start > 2000 && plottear){
    if(!mpu.getMotionInterruptStatus()){
      plottear = false;
    }
    else
      start = millis();      
  }  
}
