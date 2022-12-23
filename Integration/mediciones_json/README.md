# 1

Utiliza detección de movimiento para detectar un evento y grabar por 2 segundos (configurable) un evento de vibración.
Una vez grabado en un archivo localmente, se envía por HTTP hacia un servidor.

Este código funciona en un solo core y es síncrono, es decir, la detección de eventos se bloquea mientras se envíe el
archivo

## Metodos encpasulados
Este código contiene todo el código dependiente de la placa ESP32 utilizada y del acelerómetro MPU6050 encapsulado en funciones para que sea simple modificarlo.

Estos métodos son:
### Metodos de SETUP

- **_iniciarSPIFFS()**: Inicializa el sistema de archivos de la placa ESP32, que gracias al objeto SPIFFS permite abrir, leer y escribir archivos.

- **_iniciarMPU()**: Inicializa el acelerómetro.
- **_restart()**: Reinicia la placa.
- **_iniciarWIFI()**: Inicializa el WIFI de la placa.

### Metodos del LOOP
- **_enviarArchivo()**: Envía el archivo a un servidor usando el protocolo HTTP.
- **_eliminarArchivo()**: Elimina un archivo del sistema de archivos de la placa.
- **_abrirArchivo()**: Abre el archivo en modo escritura o lectura
- **_detectarEvento()**: Detecta si está ocurriendo un evento de vibraciones.
- **_leerDatos()**: Captura los datos del acelerómetro y los guarda en un objeto Lectura.

## Objeto Lectura
Utiliza un nuevo objeto Lectura para guardar los datos de un acelerómetro y giroscópio.



