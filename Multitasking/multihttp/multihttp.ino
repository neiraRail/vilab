#include <WiFi.h>
#include <HTTPClient.h>
//Objetos task
TaskHandle_t Task1;
TaskHandle_t Task2;

const char* ssid = "CEIS";
const char* password = "CEIS.2022";

void sendGet(){
  HTTPClient http;

  http.begin("http://129.151.100.69:8080/status"); //GET endpoint
    
  Serial.println("Haciendo GET request..."); //Json format
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
  }
  http.end();
}

void initWiFi(){
  WiFi.mode(WIFI_OFF); //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA); //This line hides the viewing of ESP as wifi hotspot
  Serial.println("");
  WiFi.begin(ssid, password);
  Serial.println("Conectando al Wifi.");
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  //If connection successful show IP address in serial monitor
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  initWiFi();


  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}

//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    Serial.println(millis());
    delay(1000);
  } 
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    if(WiFi.status() == WL_CONNECTED){
      sendGet();
    }
    delay(10000);
  }
}

void loop() {
  
}
