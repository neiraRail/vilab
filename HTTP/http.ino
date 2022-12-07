#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "CEIS";
const char* password = "CEIS.2022";

//WiFiClient wifi;

void sendPost(){
  HTTPClient http;

  http.begin("http://129.151.100.69:8080/status"); //POST endpoint
    
  Serial.println("Haciendo POST request...");
  http.addHeader("Content-Type", "application/json"); //Json format
  int httpCode = http.POST("{\"text\":\"desde esp32\"}");
    
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  
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
void loop() {
  // put your main code here, to run repeatedly:  
  if(WiFi.status() == WL_CONNECTED){
    sendPost();
    sendGet();
  }
  delay(30000);

}
