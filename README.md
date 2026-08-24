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
Criterio de elección de N: Se elige una ventana de N = 5 porque permite la filtración de ruido aleatorio del ADC sin generar un gran retardo en el nodo


**Semana 4 (17 al 21 de agosto de 2026) · GT2, ítem 1 · Familias G (llama IR) y F (MQ-2)**

Este bloque se copia al README del repositorio del equipo y se completa en la
Sesión B. P23 tiene dos sensores de familias distintas en el mismo ítem 1: cada
uno se registra con el procedimiento de su familia. En ninguna de las dos existe
par (m, b), de modo que la tabla del formato general se adapta según indica la
cápsula «Del simulador al sensor físico».

**Prohibido el fuego abierto.** Toda la verificación del sensor de llama se hace
con la fuente luminosa entregada por el docente de la sección.

---

```markdown
## Verificacion fisica de los sensores (Semana 4, item 1 GT2)

### A. Sensor de llama IR — Familia G (de evento)
Referencia: fuente luminosa patron entregada por <docente>, tipo <descripcion>

Condiciones (se fijan ANTES de medir y no se modifican):
| Condicion                           | Valor declarado |
|-------------------------------------|-----------------|
| Fuente patron                       | <tipo y potencia aproximada> |
| Posicion del potenciometro          | <marca fisica; fijada antes de la primera serie> |
| Iluminacion ambiente                | <sala sin sol directo / con ventana / etc.> |
| Tension de alimentacion             | 3,3 V (salida DO medida antes de conectar: <V>) |

Tolerancia declarada ANTES de verificar:
| Criterio                                        | Tolerancia aceptada |
|-------------------------------------------------|---------------------|
| Detecciones sobre presentaciones a la distancia de trabajo | <p. ej. 10 de 10> |

Series (10 presentaciones por serie):
| Distancia (cm) | Angulo (grados) | Presentaciones | Detecciones | Observacion |
|----------------|-----------------|----------------|-------------|-------------|
| 10             | 0               | 10             | <10>        | <estable>   |
| 20             | 0               | 10             | <10>        | <estable>   |
| 40             | 0               | 10             | <0>         | <sin deteccion>  |
| 60             | 0               | 10             | <0>         | <sin deteccion>  |
| 20             | 30              | 10             | <8 a 10>    | <estable / inclinando la luz>|
| 20             | 60              | 10             | <0 a 2>     | <inestable / fuera del cono> |

Nivel sin estimulo: <1>   Nivel con la fuente (NIVEL_ACTIVO): <0>
Alcance util declarado: <20 cm>   Cono de deteccion declarado: <30 grados>

### B. MQ-2 — Familia F (relativo con linea base)
Referencia: aire limpio del laboratorio tras la estabilizacion del calefactor

Condiciones:
| Condicion                          | Valor declarado |
|------------------------------------|-----------------|
| Alimentacion del modulo            | 5 V |
| Acondicionamiento de la salida     | 1 k en serie + 100 nF a masa, SIN divisor |
| RL del modulo (medida con ohmetro) | <kohm> (2,0 en los ejemplares del laboratorio) |
| El potenciometro afecta la salida analogica | si / no (comprobado girando un cuarto de vuelta) |
| Canal empleado                     | GPIO 34 (ADC1), atenuacion 11 dB, lectura en mV calibrados |
| Ventilacion del recinto            | <ventana abierta / cerrada> |
| Temperatura y humedad (DHT22)      | inicio: <°C / %HR>   final: <°C / %HR> |
| Duracion del registro              | <min> |

Tolerancia declarada ANTES de verificar:
| Criterio                                   | Tolerancia aceptada |
|--------------------------------------------|---------------------|
| Pendiente para declarar estabilizado        | <mV por segundo> |

Resultado:
| Parametro                            | Valor medido | Origen |
|--------------------------------------|--------------|--------|
| Tiempo de estabilizacion             | <60 s>       | Medicion en aire limpio tras calentar |
| Linea base en tension                | <39.5 mV>    | Monitor serie |
| Linea base en resistencia (RS0)      | <251.25 kohm>| Calculado por el ESP32 en reposo |
| Dispersion de RS en la meseta        | <±25.0 kohm> | Fluctuacion observada <227-273kohm> |
| RS con el estimulo de prueba         | <6.92 kohm>  | Algodon con alcohol a 2 cm|
| Razon alcanzada (RS_estimulo / RS0)  | <0.028>      | Calculo directo |
| Razon de alarma propuesta            | <0.10>       | Umbral de disparo para deteccion de gas |

### C. DHT22 — Familia B (magnitud entregada)
Referencia: termometro de la sala entregado por <docente>

Tolerancia declarada ANTES de verificar: desviacion aceptada <±°C>

| Parametro                          | Valor |
|------------------------------------|-------|
| Promedio de 10 lecturas (sensor)   | <°C>  |
| Referencia de sala                 | <°C>  |
| Desviacion observada               | <°C>  |
| Decision                           | se acepta sin corregir / se corrige |
| M_CAL declarado                    | <1,0 salvo justificacion> |
| B_CAL declarado                    | <°C>  |
| Lecturas invalidas en la corrida   | <n>   |
| Humedad relativa registrada        | <%>   |

Intervalo minimo de 2 s respetado: si / no
Nota: con una sola condicion de referencia solo es defendible ajustar B_CAL.
Modificar M_CAL exige dos condiciones distintas, que esta sesion no provee.

### Contraste con la GT1 (simulacion)
En la GT1 el equipo calibro un sensor analogico simulado con un par (m, b).
Ninguno de los dos sensores principales de P23 admite ese procedimiento: uno
entrega un veredicto binario y el otro una salida relativa sin unidades. Lo que
se traslada de la GT1 es el metodo: declarar referencia y tolerancia antes de
medir, y registrar la desviacion respecto del comportamiento esperado.

Desviacion respecto del comportamiento ideal declarado:
- Llama: <detecciones faltantes> de <presentaciones> a la distancia de trabajo.
- MQ-2: <si la meseta se alcanzo dentro del tiempo previsto, si o no>.

### Limitaciones registradas
- El MQ-2 responde a humo y a gases combustibles (GLP, propano, metano,
  hidrogeno, alcohol) sin distinguir entre ellos. Es el sensor adecuado para el
  fenomeno del proyecto, pero el nodo reporta desviacion respecto de su linea
  base, no una concentracion absoluta de humo.
- RS se calcula como RL*(VC - V)/V con la RL medida en el propio modulo. El
  valor de VC se supone 5,0 V; si la alimentacion real difiere, RS se desplaza
  de forma proporcional. La razon RS/RS0 no se ve afectada, y por eso el
  criterio de alarma se expresa como razon y no como resistencia absoluta.
- La hoja de datos del MQ-2 exige un acondicionamiento inicial de muchas horas
  para una linea base estable entre sesiones, y la lectura se desplaza con la
  temperatura y la humedad. La linea base de esta sesion vale para este
  ejemplar, este ambiente y esta jornada; no es una concentracion absoluta ni
  comparable con la de otro equipo.
- El sensor de llama responde a cualquier fuente infrarroja intensa. El alcance
  registrado depende de la fuente patron empleada y no es transferible a una
  llama real.
- P23 es un prototipo educativo. No es un sistema de deteccion de incendios.
```

---

