# Motion Detection

MPU-6050 tiene un módulo dedicado a la detección de movimiento. Todas las librerías para interacturar con el sensor implementan metodos para utilizar este módulo:

## - setMotionDetectionThreshold()
    
    Configura el threshold de detección para la generación de Motion interrupt.
    
    La unidad utilizada es 1LSB = 2mg. El movimiento es detectado cuando el valor absoluto de cualquiera de las medidas de los ejes del acelerometro excede este threshold.
    
    Esta condición incrementa el contador de duración de Motion detection (Registro 32). La interrupción del Motion detection es gatillada cuando este contador alcanza el tiempo especificado en MOT_DUR (duración del movimiento).
    
## - setZeroMotionDetectionThreshold()
    
    Configura el threshold de detección para la generación de Zero Motion interrupt.

    La unidad utilizada es 1LSB = 2mg. Zero Motion es detectado cuando el valor absoluto de las medidas del acelerómeto para los 3 ejes son cada uno menor que este valor.

    Esta condición incrementa el contador de duración de Zero Motion. La interrupción del Zero Motion es gatillada cuando el contador alcanza el tiempo especificado en ZRMOT_DUR.
    
    Cuando una condición de motion-to-zero-motion es detectada el status será 1, y cuando una condición de zero-motion-to-motion es detectada, el status será 0.

Los siguientes 2 script son ejemplos de la implementación del motion detection:
- [github_example](github_example/github_example.ino) - corresponde a un ejemplo extraido de asdasd, que además de contener comentarios para cada metodo utilizado, implementa una forma de detección de movimiento.
- [cinco_sec_example](cinco_sec_detection/cinco_sec_detection.ino) - ejemplo de detección de movimiento y captura de datos por un periodo de tiempo.