#include<ESP8266_multipart.h>
//#include "SPIFFS.h"

File file;

char* ssid     = "railxalkan";
char* password = "familiarailxalkan";


void setup() {
  Serial.begin(115200);
  delay(2000);
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // put your main code here, to run repeatedly:
//  delay(2000);
//  WiFi.disconnect();
  delay(2000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  file = SPIFFS.open("/cuatro.txt", "r");
  
  if(!file){
    Serial.println("Failed to open file for reading");
    while(true);
    return;
  }
  Serial.println("TODO beum abriendi eu arqivo");
  delay(1000);
  Serial.println("Enviando archivo....");
  ESP8266_multipart multipart("192.168.18.3"); // Sets the host to myexampleserver.com and the port to 80
  delay(1000);
  multipart.setPort(5000); // Sets the port to 8772
  delay(1000);
  int status = multipart.sendFile("/files", file);
  Serial.print(status);
  file.close();
  delay(2000);
 
}

void loop() {
  

}
