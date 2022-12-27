#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include "Lectura.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>

char* _ssid     = "railxalkan";         //Nombre de la red
char* _password = "familiarailxalkan";  //Contraseña de la red
char* _server   = "129.151.100.69";     //IP del servidor de envio de archivos
int   _port     = 8080;                 //Puerto en el que esta escuchando el servidor de envio

Adafruit_MPU6050 mpu;

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
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G); //Rango del acelerómetro
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);       //Rango del giroscópio

  //Establecer filtros del acelerómetro
  //mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  //mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  
  //Establecer configuración del motion detection
  mpu.setMotionDetectionThreshold(2);   //Threshold para la detección de movimiento (G)
  mpu.setMotionDetectionDuration(2); //Duración mínima del evento para la detección (ms)
  
  //Aun no sé para qué se utiliza en.
  mpu.setInterruptPinLatch(true);  // Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
}

void _restart(){
  ESP.restart();
}

void _iniciarWIFI(){
  unsigned long mytime;
  
  Serial.print("Intentando conectando a la siguiente red: ");
  Serial.println(_ssid);
  WiFi.begin(_ssid, _password);
  mytime = millis();
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if(millis() - mytime > 5000){
        _restart();
      }
  }
    Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void _probarAPI(){
  HTTPClient http;

  http.begin("http://129.151.100.69:8080/status"); //GET endpoint
    
  Serial.println("Probando API..."); //Json format
  int httpCode = http.GET();
    
  String payload = http.getString();
  if (httpCode > 0){
      
    Serial.println(httpCode);
    Serial.println(payload);
  }
  else{
    Serial.println(httpCode);
      
    Serial.println(payload);
    Serial.println("Error");
    _restart();
  }
  http.end();
}

void setup() {
  //Empezar y esperar el monitor serial
  Serial.begin(115200);
  while (!Serial)
    delay(100);
  delay(1000);
  
  _iniciarMPU();
  _iniciarWIFI();

  _probarAPI();

  Serial.println("Comenzando benchmark");
  Serial.println("Se mediran los tiempos de envío de archivos \ncon mediciones de 100, 200, 500 y 1000 vectores");
  Serial.println("");
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

/* Transformar una medición a JSON
 * Utiliza un objeto Lectura por lo que es compatible con cualquier acelerómetro
 * retorna String con objeto Json.
 */
String _registroAJson(Lectura lectura){
  String json = "{ \"time\":\"" +(String)millis() + "\",";
  json += "\"acc_x\":\""+(String)lectura.getAcc()[0]+"\",";
  json += "\"acc_y\":\""+(String)lectura.getAcc()[1]+"\",";
  json += "\"acc_z\":\""+(String)lectura.getAcc()[2]+"\",";
  json += "\"gyr_x\":\""+(String)lectura.getGyro()[0]+"\",";
  json += "\"gyr_y\":\""+(String)lectura.getGyro()[1]+"\",";
  json += "\"gyr_z\":\""+(String)lectura.getGyro()[2]+"\",";
  json += "\"temp\":\""+(String)lectura.getTemp()+"\"}";
  return json;
}

/* Lectura de datos utiliza la librería Adafruit_MPU6050 por lo que es exclusiva para el acelerometro mpu6050
 *  recibe la referencia a un objeto lectura y le ingresa los valores.
 */
void _leerDatos(Lectura* lectura){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  lectura->setValues(a.acceleration.v, g.gyro.v, temp.temperature);
}


int _enviarJson(String json){
  
  HTTPClient http;
  http.begin("http://129.151.100.69:8080/events"); //GET endpoint
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(json);
  String payload = http.getString();
  
  if (httpCode > 0){
    Serial.println(httpCode);
    Serial.println(payload);
  }
  else{
    Serial.println(httpCode);
      
    Serial.println(payload);
    Serial.println("Error");
  }
  http.end();
  return 0;
}

// Tiempos equivalentes a 30HZ:
//                 1s  2s   5s  10s
int segundos[4] = {30, 60, 150, 300};
unsigned long start;      //contador de tiempo.
void loop() {
  for(int i=0;i<4;i++){
    Serial.print("Benchmark de ventana con ");
    Serial.print(segundos[i]);
    Serial.println(" mediciones");
    //Comenzamos a contar
    start = millis();
    
    //Documento Json para guardar datos
    String json = "[";
    
    for(int j=0; j<=segundos[i];j++){
      //Leer datos
      Lectura lectura;
      _leerDatos(&lectura);
      
      //Imprimir en archivo
      //Serial.println(lectura.getAcc()[0]);
      json += _registroAJson(lectura);
      
      if(j<segundos[i]){
        //Si no se finaliza, se pone una coma
        json += ",\n";
      }
    }
    // Finalizar evento
    json += "]";
    
    //Enviar archivo
    unsigned long comienzoLectura = millis();
    Serial.println("lrgo del json: "+(String)json.length());
    Serial.println("Mandando json...");
    unsigned long comienzoEnvio = millis();
    _enviarJson(json);
    comienzoEnvio = millis() - comienzoEnvio;
    Serial.println("");
    comienzoLectura = millis() - comienzoLectura;

    Serial.println("Resultados benchmark para "+(String)(segundos[i]) + " mediciones");
    Serial.print("Tiempo completo: ");
    Serial.println(comienzoLectura);
    Serial.print("Tiempo de envío: ");
    Serial.println(comienzoEnvio);
    
  }
  Serial.println("Benchmark finalizado");
  for(;;);
}
