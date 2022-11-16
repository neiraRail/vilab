#include <Wire.h>
#include "SPIFFS.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ESP32_multipart.h>

char* _ssid     = "***";           //Nombre de la red
char* _password = "***";      //Contraseña de la red
char* _server   = "****";  //IP del servidor de envio de archivos
int   _port     = 5000;             //Puerto en el que esta escuchando el servidor de envio

int _windowTime     = 2000;                   //Tiempo mínimo para capturar datos
int _rangoAcc       = MPU6050_RANGE_4_G;      //Rango del acelerómetro
int _rangoGir       = MPU6050_RANGE_250_DEG;  //Rango del giroscópio
int _motionDetThold = 2;                      //Threshold para la detección de movimiento (G)
int _motionDetDur   = 20;                     //Duración mínima del evento para la detección (ms)

Adafruit_MPU6050 mpu;

void _iniciarSPIFFS(){
  Serial.println("Incializando SPIFFS...");
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  Serial.println("SPIFFS montado correctamente");
}

void _iniciarMPU(){
  Serial.println("Buscando acelerómetro MPU6050...");
  if (!mpu.begin()) {
    Serial.println(F("Failed to find MPU6050 chip"));
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 encontrado");
  
  //Establecer el rango del acelerómetro y del giroscópio
  mpu.setAccelerometerRange(_rangoAcc);
  mpu.setGyroRange(_rangoGir);

  //Establecer filtros del acelerómetro
  //mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  //mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  
  //Establecer configuración del motion detection
  mpu.setMotionDetectionThreshold(_motionDetThold);
  mpu.setMotionDetectionDuration(_motionDetDur);
  
  //Aun no sé para qué se utiliza en.
  mpu.setInterruptPinLatch(true);  // Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
}

void _iniciarWIFI(){
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
  
  _iniciarSPIFFS();
  _iniciarMPU();
  _iniciarWIFI();  
}

/*  Implementación de enviar archivo. Esta utiliza la librería ESP32_multipart exclusiva de ESP32.
 *  Se puede adaptar siempre y cuando reciba un objeto File y retorne un 0 cuando no se ha podido enviar.
 */
int _enviarArchivo(File file){
  ESP32_multipart multipart(_server);
  multipart.setPort(_port);
  int status = multipart.sendFile("/files", file);
  Serial.print(status);
  return status;
}
/*  Implementación de eliminar archivo. Utiliza la librería SPIFFS exclusiva de ESP32
 *  Se puede adaptar y utilizar cualquier otro metodo que elimine el archivo y retorne
 *  true o false
 */
 boolean _eliminarArchivo(String filename){
  //Retorna true si fue eliminado correctamente.
  return SPIFFS.remove(filename);
 }
/*  Implementación de abrir archivo. Utliza lalibrería SPIFFS exclusiva de ESP32
 *  Se puede adaptar y utilizar un metodo que abra para abrir como lectura y escritura
 *  recibe filename(String) y 'r' o 'w'(Char)
 */
 File _abrirArchivo(String filename, char modo){
  if(modo == 'r')
    return SPIFFS.open(filename, "r");
  else
    return SPIFFS.open(filename, "w");
 }

/* Detectar evento
 * Utiliza la librería Adafruit_MPU6050 exclusiva para el acelerómetro MPU6050
 * Internamente esta función calcula si el valor absoluto de cualquiera de los 3
 * ejes del acelerómetro supera el threshold indicado (en este caso 2).
 * Retorna true o false.
 */
 boolean _detectarEvento(){
  return mpu.getMotionInterruptStatus();
 }

int nro_files = 0;        //Contador de archivos enviados.
File file;                //Archivo de evento.
boolean evento = false;   //Define si está ocurriendo un evento o no.
boolean hayFile = false;  //Definirá si se debe enviar un archivo o capturar vibración.
unsigned long start;      //contador de tiempo.

void loop() {
  if(!hayFile){
    // Leer datos del acelerometro
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    //Acciones a realizar si está ocurriendo un evento
    if(evento){
      // NOTA: Puede ser una función que a partir de los sensor_event a, g y temp retorne el string que será escrito en el archivo.
      
      // Calcular aceleración modulo de vector
      float acc = sqrt(a.acceleration.x*a.acceleration.x + a.acceleration.y*a.acceleration.y + a.acceleration.z*a.acceleration.z);
      // Serial.println(acc);
      // Aqui debe ir la escritura de la lectura en un archivo.
      file.println( "{ \"time\": \" " +(String)millis() + "\", \"acc\": \" "+acc+"\"}");

      
      //Finlizar evento despues de x segundos o dar x segundos más si en ese momento hay movimiento. 
      //NOTA: Puede fallar por que aunque haya movimiento, podría calzar justo un instante de no movimiento
      //Una alterantiva mejor puede ser llevar un contador y subirlo y bajarlo dependiendo de si hay 
      //movimiento o no, cambiar la flag cuando el contador llegue a cero.
      //_windowTime es el tiempo mínimo de captura de evento.
      if(millis()-start > _windowTime){
        // Finalizar evento
        if(!_detectarEvento()){
          evento = false;
          file.println("]");
          file.close();
          hayFile = true;
        }
        //Si aún hay movimiento, se reinicia el contador para dar x segundos más.
        else
          start = millis();      
      }
      // Si el evento no ha terminado, escribir coma.
      else
        file.println(",\n");
    }
    else{ // !evento
      if(_detectarEvento()){
        // Detectar evento y cambiar flag evento
        // Se crea un nuevo archivo
        start = millis();
        evento = true;
        //Abrir un archivo para escribir
        String filename = String("/evento")+nro_files+".txt";
        file = _abrirArchivo(filename, 'w');
        if(!file){
          Serial.println("Fallo al abrir archivo para escribir");
          evento = false;
          return;
        }
        file.println("[");
      } 
    }
  }
  else{
    // Esto se ejecuta cuando hay un archivo, es decir
    // hayFile == true
    // La lectura se pausa hasta que el archivo se haya enviado.
    // Serial.println("HAY FILE");
    
    String filename = String("/evento")+nro_files+".txt";
    file = _abrirArchivo(filename, 'r');
    if(!file){
      Serial.println("Fallo al abrir archivo para leer, se intentará nuevamente...");
      return;
    }
    Serial.println("Archivo abierto");
    Serial.print("Mandando archivo...");
    //Esta implementación se puede adaptar para otra placa.
    int status = _enviarArchivo(file)
    file.close();
    //Si el archivo fue enviado correctamente entonces eliminar archivo.
    if(status!=0){
      _eliminarArchivo(filename);
      
      hayFile = false;         //Volver a capturar datos
      nro_files++;             //Aumentar contador de archivos enviados
    }
    else
    ´Serial.println("No fue posible enviar el archivo, se intentarà nuevamente...");
  }
}
