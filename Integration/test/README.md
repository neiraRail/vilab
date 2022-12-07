# test

Este código separa el módulo del acelerómetro a una interfaz que puede ser implementada para dar soporte a cualquier tipo de acelerómetro.
Esta interfaz cuenta con los metodos: iniciar(), leerDatos() y detectarMovimiento();

En esta carpeta se encuentra la implementación de esta interfaz para el acelerómetro MPU6050 utilizando la libería AdadfruitMPU6050.

Además, se creó una clase llamada VilabUnit que representa un módulo de lectura de vibraciones la cual puede ser inicializada entregandole un objeto que implemente IAcelerometro.

La interfaz IAcelerometro tiene una dependencia con la clase Lectura.