#include <Wire.h>
#include "SPIFFS.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include<ESP8266_multipart.h>

char* ssid     = "CEIS";
char* password = "CEIS.2022";

Adafruit_MPU6050 mpu;
void setup() {
  //Empezar y esperar el monitor serial
  Serial.begin(115200);
  while (!Serial)
    delay(100);
  delay(1000);
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  Serial.println(F("Inicializando sistema"));
  //Comprobar chip MPU6050 acelerometro
  if (!mpu.begin()) {
    Serial.println(F("Failed to find MPU6050 chip"));
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 encontrado");
  //Establecer el rango del acelerómetro y del giroscópio
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  //mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  //mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  //Establecer configuración del motion detection
  mpu.setMotionDetectionThreshold(2);
  mpu.setMotionDetectionDuration(20);
  //Aun no sé para qué se utiliza.
  mpu.setInterruptPinLatch(true);  // Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
    Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}

int nro_files = 0;

File file;
//Flag de evento
boolean plottear = false;
boolean hayFile = false;
//contador de tiempo
unsigned long start;
void loop() {
//  Serial.println("habrá file?: "+hayFile);
  if(!hayFile){
//    Serial.println("NO");
    // Leer datos del acelerometro
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
  
    //Acciones a realizar si está ocurriendo un evento
    float acc;
    if(plottear){
      //calcular aceleración modulo de vector
       acc = sqrt(a.acceleration.x*a.acceleration.x + a.acceleration.y*a.acceleration.y + a.acceleration.z*a.acceleration.z);
       Serial.println(acc);
       // Aqui debe ir la escritura de la lectura en un archivo.
       file.println( "{ \"time\": \" " +(String)millis() + "\", \"acc\": \" "+acc+"\"}");
    }
  
    
    //Detectar evento y cambiar flag plottear
    if(mpu.getMotionInterruptStatus() && !plottear){
      start = millis();
      plottear = true;
      //Abrir un archivo para escribir
      String filename = String("/evento")+nro_files+".txt";
      file = SPIFFS.open(filename, "w");
      if(!file){
        Serial.println("Fallo al abrir archivo para escribir");
        plottear = false;
        return;
      }
      file.println("[");
    }
    //Finlizar evento despues de 3 segundos o dar 3 segundos más si en ese momento hay movimiento. Puede fallar por que aunque haya movimiento, podría calzar 
    //justo un instante de no movimeinto
    //Una alterantiva mejor puede ser llevar un contador y subirlo y bajarlo dependiendo de si hay movimiento o no, cambiar la flag cuando el contador llegue a cero
    if(millis()-start > 2000 && plottear){
      if(!mpu.getMotionInterruptStatus()){
        plottear = false;
        file.println("]");
        file.close();
        hayFile = true;
      }
      else
        start = millis();      
    }
  
    if(plottear){
      file.println(",\n");
    }
  }
  else{
    Serial.println("HAY FILE");
    String filename = String("/evento")+nro_files+".txt";
    file = SPIFFS.open(filename, "r");

    if(!file){
      Serial.println("Failed to open file for reading");
      while(true);
      return;
    }
    Serial.println("TODO beum abriendi eu arqivo");
    Serial.print("Mandando archivo......");
    // put your setup code here, to run once:
    ESP8266_multipart multipart("192.168.1.107"); // Sets the host to myexampleserver.com and the port to 80
    multipart.setPort(5000); // Sets the port to 5000
    int status = multipart.sendFile("/files", file);
    Serial.print(status);
    file.close();

    hayFile = false;
    nro_files++;
    
    delay(1000);
  }
}
