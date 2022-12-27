# Mediciones Json

Mide ventanas de 0.5, 1, 3 y 6 segundos para medir el tiempo de envío de los datos en un POST. En este ejemplo, las mediciones se van guardando en un String a medida que se van leyendo y por lo tanto se detectó un límite de alrededor de 300 vectores como máximo.

Los resultados de estas mediciones son los siguientes:

| Vectores    | Envío (ms)|
|------------:|----------:|
|     30      |    88     |
|     60      |   106     |
|    150      |   177     |
|    300      |   269     |

De los datos se puede notar que se alcanza una notable velocidad de envío, siempre y cuando consideremos la limitante de tamaño.

### Nota
Este código funciona en un solo core y es síncrono, es decir, la detección de eventos se bloquea mientras se envíe el
archivo
