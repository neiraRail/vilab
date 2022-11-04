#include "SPIFFS.h"
void printFiles() {
  String str = "";
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while (file) {
    str += file.name();
    str += "\r\n";
    file = root.openNextFile();
  }
  file.close();
  Serial.print(str);
}

void readFile(String filename){
  File file = SPIFFS.open(filename);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("Content:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(String filename){
  File file = SPIFFS.open(filename, "w");
  if(!file){
    Serial.println("Fallo opening file for writing");
    return;
  }
  file.print("hello world");
  file.close();
}

void setup() {
  Serial.begin(115200);
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  // Read file and write to Serial monitor.
  readFile("/read.txt");

  // Write "hello world" to file.
  writeFile("/write.txt");  

  // Print existing files in root directory
  printFiles();
}

void loop() {
  // put your main code here, to run repeatedly:

}
