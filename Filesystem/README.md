# Filesystem
Este script muestra como utilizar la librería ```SPIFFS``` para buscar, leer y escribir archivos en una tarjeta ESP32. Aunque esta librería sólo funcione en la tarjeta ESP32, este código también es compatible con tarjetas ESP8266 y el único cambio necesario sería cambiar la librería ```SPIFFS``` por la librería ```FS```.
- [Documentación SPIFFS](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/spiffs.html)
- [Documentación FS](https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html)

Para subir archivos desde nuestro computador local, debemos utilizar la extensión de Arduino IDE llamada [Sketch Data Uploader](https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/).