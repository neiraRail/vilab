#include <Wire.h>
#include "SPIFFS.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include<ESP8266_multipart.h>

char* _ssid     = "CEIS";
char* _password = "CEIS.2022";
char* _server   = "192.168.1.107";
int   _port     = 5000;

int _windowTime = 2000;

Adafruit_MPU6050 mpu;

void iniciarSPIFFS(){
  Serial.println("Incializando SPIFFS...");
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  Serial.println("SPIFFS montado correctamente");
}

void iniciarMPU(){
  Serial.println("Buscando acelerómetro MPU6050...");
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
}

void iniciarWIFI(){
  Serial.print("Intentando conectando a la siguiente red: ");
  Serial.println(_ssid);
  WiFi.begin(_ssid, _password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
    Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  //Empezar y esperar el monitor serial
  Serial.begin(115200);
  while (!Serial)
    delay(100);
  delay(1000);
  
  iniciarSPIFFS();
  iniciarMPU();
  iniciarWIFI();  
}

int nro_files = 0; //Contador de archivos enviados
File file; //Archivo
//Flag de evento
boolean evento = false;
boolean hayFile = false; //Definirá si se debe enviar el archivo o capturar
//contador de tiempo
unsigned long start;
void loop() {
  if(!hayFile){
//    Serial.println("NO");
    // Leer datos del acelerometro
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
  
    // Detectar evento y cambiar flag evento
    // Se crea un nuevo archivo
    if(mpu.getMotionInterruptStatus() && !evento){
      start = millis();
      evento = true;
      //Abrir un archivo para escribir
      String filename = String("/evento")+nro_files+".txt";
      file = SPIFFS.open(filename, "w");
      if(!file){
        Serial.println("Fallo al abrir archivo para escribir");
        evento = false;
        return;
      }
      file.println("[");
    } 
    
    
    //Acciones a realizar si está ocurriendo un evento
    if(evento){
      // Puede ser una función que a partir de los sensor_event a, g y temp retorne el string que será escrito en el archivo.
      //calcular aceleración modulo de vector
      float acc = sqrt(a.acceleration.x*a.acceleration.x + a.acceleration.y*a.acceleration.y + a.acceleration.z*a.acceleration.z);
      Serial.println(acc);
      // Aqui debe ir la escritura de la lectura en un archivo.
      file.println( "{ \"time\": \" " +(String)millis() + "\", \"acc\": \" "+acc+"\"}");
    }
    
    //Finlizar evento despues de x segundos o dar x segundos más si en ese momento hay movimiento. 
    //Puede fallar por que aunque haya movimiento, podría calzar justo un instante de no movimiento
    //Una alterantiva mejor puede ser llevar un contador y subirlo y bajarlo dependiendo de si hay 
    //movimiento o no, cambiar la flag cuando el contador llegue a cero.
    //_windowTime es el tiempo mínimo de captura de evento.
    if(millis()-start > _windowTime && evento){
      // Finalizar evento
      if(!mpu.getMotionInterruptStatus()){
        evento = false;
        file.println("]");
        file.close();
        hayFile = true;
      }
      //Si aún hay movimiento, se reinicia el contador para dar x segundos más.
      else
        start = millis();      
    }
    
    //Para escribir la coma, solo ni en este ciclo no se cerró el archivo.
    if(evento){
      file.println(",\n");
    }
  }
  // Esto se ejecuta cuando hay un archivo.
  // La lectura se pausa hasta que el archivo se haya enviado.
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
    ESP8266_multipart multipart(_server);
    multipart.setPort(_port); // Sets the port to 5000
    int status = multipart.sendFile("/files", file);
    Serial.print(status);
    file.close();

    //Si el archivo fue enviado correctamente entonces eliminar archivo.
    if(status!=0){
      SPIFFS.remove(filename); //Retorna true si fue eliminado correctamente.
      hayFile = false; //Volver a capturar datos
      nro_files++; //Aumentar contador
    }
    else
    ´Serial.println("No fue posible enviar el archivo, se intentarà nuevamente...");
  }
}
