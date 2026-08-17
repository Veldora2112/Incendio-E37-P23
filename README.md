## Detección Temprana de Incendios Forestales

### Participantes:
- Daniel Quiroz
- Gabriel Salazar
- Antoine Prouvay
- Estela Garrido
- Vanessa Marchat

### Proyecto

#### Problematica
Los incendios forestales se deben de combatir en los primeros minutos, luego ya es tarde.
El equipo debe de construir un nodo de detección temprana que fusiona temperatura, humo y llama para alertar con la menor tasa de posible de falsas alarmas.

#### Sensores
- Sensor de temperatura y humedad: DHT-22
- Sensor de gases: MQ-135
- Sensor de llama: IR

### Enlace
El enlace al proyecto desarrollado en wokwi es:
- https://wokwi.com/projects/472186039648407553

##### Resultados del ítem 6.

Valores de calibración: m = 0.968, b = -2.40
Tolerancia declarada: +- 0.5 °C
Criterio de elección de N: Se elige una ventana de N = 5 porque permite la filtración de ruido aleatorio del ADC sin generar un gran retardo en el nodo.
