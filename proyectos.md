**P8 --- Sistema de riego inteligente por zonas**

  -----------------------------------------------------------------------
  **Eje:** C · Agro           **Cupo:** 2        **Tutor:** Reinier
                              equipos            Rodríguez
  ----------------- --------- ------------------ ------------------------
                                                 

  -----------------------------------------------------------------------

**El problema**

Regar todo igual desperdicia agua: cada zona de un jardín o cultivo seca
a ritmo distinto. Tu equipo construirá un controlador de riego de 3
zonas independientes que riega cada una solo cuando lo necesita.

**Qué construye el equipo**

-   Edge: FSM por zona con 3 sensores de humedad de suelo y bombas
    independientes vía relé de 4 canales.

-   Hot path: dashboard por zona y comandos remotos de riego manual.

-   Cold path: consumo de agua por zona y frecuencia real de riego.

**Kit específico (además de la base de laboratorio)**

3 sensores de humedad de suelo capacitivos, tarjeta relé 4 canales, 2
mini bombas sumergibles (1--3 según zonas).

**Desafíos técnicos principales**

Tres lazos de control simultáneos sin bloquearse (aquí la FSM no
bloqueante se pone a prueba de verdad); umbrales distintos por sustrato;
medir el agua efectivamente entregada.

**Con tus propios datos podrás responder**

*¿Qué zona consume más agua y por qué? ¿Cuánto difiere el riego real del
que habrías programado con timer?*

**P9 --- Estación de triaje / signos vitales IoT**

  -----------------------------------------------------------------------
  **Eje:** D ·                **Cupo:** 1       **Tutor:** Reinier
  Salud                       equipo            Rodríguez
  ----------------- --------- ----------------- -------------------------
                                                

  -----------------------------------------------------------------------

**El problema**

En un punto de primeros auxilios, tomar pulso, oxigenación y temperatura
toma minutos por persona. Tu equipo construirá una estación que captura
esos tres signos en segundos y los registra --- como ejercicio de
ingeniería, no como dispositivo médico.

**Qué construye el equipo**

-   Edge: estación con pulso/SpO2 (MAX30102), temperatura sin contacto
    (MLX90614) y pantalla local.

-   Hot path: registro de cada medición en P16 con timestamp.

-   Cold path: análisis de repetibilidad y deriva de los sensores.

**Kit específico (además de la base de laboratorio)**

MAX30102 (pulso/SpO2), MLX90614 (temperatura IR sin contacto), display
OLED I2C; botón y buzzer de la base de laboratorio.

**Desafíos técnicos principales**

Lectura estable del MAX30102 (posición del dedo, luz ambiente); filtrado
de señal; diseñar el flujo de medición para que cualquier persona lo use
sin instrucciones.

**Con tus propios datos podrás responder**

*¿Cuánto varían dos mediciones consecutivas de la misma persona? ¿El
sensor deriva con el uso continuo?*

  -----------------------------------------------------------------------
  **⚠ PROTOTIPO EDUCATIVO SIN VALIDEZ CLÍNICA: este rótulo es obligatorio
  en el dispositivo, el póster, el repositorio y toda demostración. No
  diagnostica ni reemplaza instrumentos médicos.**
  -----------------------------------------------------------------------

  -----------------------------------------------------------------------

**P10 --- Monitor de adulto mayor / detección de caídas**

  -----------------------------------------------------------------------
  **Eje:** D ·                **Cupo:** 2        **Tutor:** Reinier
  Salud                       equipos            Rodríguez
  ----------------- --------- ------------------ ------------------------
                                                 

  -----------------------------------------------------------------------

**El problema**

Una caída en soledad puede pasar horas sin ser detectada. Tu equipo
construirá un dispositivo portátil que detecta el patrón de una caída y
dispara una alerta, más un botón de pánico manual --- priorizando la
privacidad de quien lo usa.

**Qué construye el equipo**

-   Edge: dispositivo con acelerómetro/giroscopio (MPU6050), algoritmo
    de detección de caída, buzzer y botón de pánico; alimentación por
    batería.

-   Hot path: alerta inmediata vía Node-RED cuando se detecta caída o
    pánico.

-   Cold path: análisis de falsos positivos por tipo de actividad.

**Kit específico (además de la base de laboratorio)**

MPU6050 (acelerómetro/giroscopio), buzzer, batería 18650 + cargador
TP4056 (portabilidad); botón de pánico de la base de laboratorio.

**Desafíos técnicos principales**

Distinguir caída real de sentarse brusco o trotar (umbral de
aceleración + cambio de orientación + inmovilidad posterior); autonomía
de batería; ergonomía del uso real.

**Con tus propios datos podrás responder**

*¿Qué actividades cotidianas generan falsos positivos? ¿Qué combinación
de umbrales los minimiza sin perder sensibilidad?*

  -----------------------------------------------------------------------
  **⚠ PROTOTIPO EDUCATIVO SIN VALIDEZ CLÍNICA. Además: el dispositivo
  monitorea a una persona --- la ética de datos y el consentimiento de
  quien lo porta son parte del diseño, no un anexo.**
  -----------------------------------------------------------------------

  -----------------------------------------------------------------------

**P18 --- Red sísmica con alerta temprana**

  ------------------------------------------------------------------------
  **Eje:** H ·                   **Cupo:** 1       **Tutor:** Reinier
  Riesgo                         equipo            Rodríguez
  ----------------- ------------ ----------------- -----------------------
                                                   

  ------------------------------------------------------------------------

**El problema**

Chile es país sísmico y los sistemas de alerta temprana son redes de
sensores que detectan la onda antes de que la sientas. Tu equipo
construirá el modelo educativo de esa red: 1--3 nodos con acelerómetro
que detectan vibración anómala y disparan la alerta.

**Qué construye el equipo**

-   Edge: nodos con MPU6050 midiendo vibración continua, detección de
    evento por umbral adaptativo, alerta local con buzzer.

-   Hot path: los nodos publican a P16; una regla en Node-RED
    correlaciona nodos para confirmar el evento.

-   Cold path: ruido de fondo del edificio y catálogo de eventos
    detectados.

**Kit específico (además de la base de laboratorio)**

MPU6050 (acelerómetro/giroscopio) por nodo, buzzer, display OLED I2C.
Proyecto multi-nodo (1--3 nodos).

**Desafíos técnicos principales**

Separar un evento real del ruido (un portazo, alguien trotando):
detección tipo STA/LTA simplificada; marcar el tiempo de cada nodo de
forma comparable; tasa de muestreo suficiente sin saturar la red.

**Con tus propios datos podrás responder**

*¿Cómo cambia el ruido de fondo del edificio por hora? ¿Qué eventos
cotidianos disparan falsas alarmas?*

  -----------------------------------------------------------------------
  **⚠ MODELO EDUCATIVO, NO INSTRUMENTO SISMOLÓGICO: este rótulo es
  obligatorio en todo material. El sistema no detecta ni predice sismos
  reales.**
  -----------------------------------------------------------------------

  -----------------------------------------------------------------------

**P22 --- Estación de radiación UV**

  -----------------------------------------------------------------------
  **Eje:** H ·                **Cupo:** 2       **Tutor:** Miguel Ángel
  Ambiental                   equipos           Cochea
  ----------------- --------- ----------------- -------------------------
                                                

  -----------------------------------------------------------------------

**El problema**

El índice UV que muestra el pronóstico es de otra comuna y de un modelo,
no de tu patio. Tu equipo construirá una estación que mide la radiación
UV local, la convierte a índice UV y registra la exposición acumulada
del día.

**Qué construye el equipo**

-   Edge: sensor UV con conversión a índice UV y semáforo de exposición
    en OLED.

-   Hot path: curva de UV en vivo y alerta de índice extremo.

-   Cold path: curvas diarias y efecto de la nubosidad a lo largo de
    semanas.

**Kit específico (además de la base de laboratorio)**

Sensor UV GY-ML8511 (o VEML6075/LTR390 I2C según reposición), display
OLED I2C.

**Desafíos técnicos principales**

Calibrar la salida analógica a índice UV con una referencia y luz solar
real; la dependencia del ángulo de incidencia; registrar exposición
acumulada, no solo instantánea.

**Con tus propios datos podrás responder**

*¿A qué hora exacta peakea el UV en tu punto de medición? ¿Cuánto atenúa
una nube o una sombra parcial?*

  -----------------------------------------------------------------------
  **⚠ El sensor GY-ML8511 está agotado en MCI (en cotización de
  reposición o alternativa I2C). El proyecto parte con simulación Wokwi
  si el sensor llega después de la Semana 3.**
  -----------------------------------------------------------------------

  -----------------------------------------------------------------------

**P23 --- Detección temprana de incendios forestales**

  -----------------------------------------------------------------------
  **Eje:** H ·                **Cupo:** 2       **Tutor:** Miguel Ángel
  Riesgo                      equipos           Cochea
  ----------------- --------- ----------------- -------------------------
                                                

  -----------------------------------------------------------------------

**El problema**

Los incendios forestales se combaten en los primeros minutos: después,
ya es tarde. Tu equipo construirá un nodo de detección temprana que
fusiona temperatura, humo y llama para alertar con la menor tasa posible
de falsas alarmas.

**Qué construye el equipo**

-   Edge: nodo con DHT22 + MQ-2 (humo) + sensor de llama IR y lógica de
    fusión de señales, con alerta local.

-   Hot path: alerta remota inmediata vía Node-RED cuando dos o más
    señales coinciden.

-   Cold path: línea base de cada sensor y su deriva; catálogo de falsas
    alarmas.

**Kit específico (además de la base de laboratorio)**

DHT22, sensor de llama IR, buzzer; MQ-2 de la base de laboratorio
(MQ-135 disponible si el MQ-2 no alcanza).

**Desafíos técnicos principales**

Fusión de señales: ninguna basta sola (el MQ-2 se dispara con
desodorante, el IR con el sol); definir la lógica de confirmación;
caracterizar la línea base y su deriva.

**Con tus propios datos podrás responder**

*¿Cuánto deriva la línea base del MQ-2 en una semana? ¿Qué eventos
domésticos gatillan falsas alarmas?*

  -----------------------------------------------------------------------
  **⚠ SEGURIDAD: toda prueba con humo usa una fuente segura y supervisada
  (varilla de incienso o similar). Prohibido fuego abierto.**
  -----------------------------------------------------------------------

  -----------------------------------------------------------------------

**P24 --- Monitor inteligente de colmenas**

  -----------------------------------------------------------------------
  **Eje:** H ·               **Cupo:** 1 equipo **Tutor:** Miguel Ángel
  Agro/Bio                                      Cochea
  ----------------- -------- ------------------ -------------------------
                                                

  -----------------------------------------------------------------------

**El problema**

Un apicultor que abre la colmena para revisarla la estresa; uno que no
la revisa puede perderla. Tu equipo construirá el monitor que responde
sin abrir: peso (¿están produciendo?), temperatura interna (¿la cría
está bien?) y actividad acústica (¿la colonia está sana?).

**Qué construye el equipo**

-   Edge: nodo con celda de carga (peso total), DHT22 (clima interno) y
    micrófono (nivel de actividad), pensado para operar sin
    intervención.

-   Hot path: dashboard de la colmena y alertas (caída brusca de peso =
    posible enjambrazón o robo).

-   Cold path: el ciclo diario de peso (pecoreo) y su relación con el
    clima --- el hallazgo más elegante del catálogo.

**Kit específico (además de la base de laboratorio)**

Celda de carga + HX711, DHT22, micrófono MAX4466, display OLED I2C.

**Desafíos técnicos principales**

Calibración de la celda de carga (tara, linealidad, deriva térmica);
medir actividad acústica como nivel, no audio; robustez para operar
semanas sin intervención.

**Con tus propios datos podrás responder**

*¿Se ve el ciclo diario de pecoreo en la curva de peso? ¿La actividad
acústica anticipa cambios de comportamiento?*

  -----------------------------------------------------------------------
  **⚠ Si no hay acceso a colmena real, el sistema se valida con una
  colmena simulada (peso variable + fuente de calor): el pipeline de
  datos es idéntico.**
  -----------------------------------------------------------------------

  -----------------------------------------------------------------------

**P28 --- Trampa inteligente de plagas agrícolas**

  -----------------------------------------------------------------------
  **Eje:** I · Agro           **Cupo:** 1       **Tutor:** Miguel Ángel
                              equipo            Cochea
  ----------------- --------- ----------------- -------------------------
                                                

  -----------------------------------------------------------------------

**El problema**

El monitoreo de plagas se hace contando insectos a mano, trampa por
trampa, una vez por semana. Tu equipo construirá la trampa que se cuenta
sola: cada ingreso queda registrado con fecha y hora, y opcionalmente
fotografiado.

**Qué construye el equipo**

-   Edge: trampa con sensor IR de paso contando ingresos, y ESP32-CAM
    capturando imagen por evento (variante avanzada).

-   Hot path: conteo acumulado y eventos en vivo en el dashboard.

-   Cold path: actividad de la plaga por hora y día --- el dato que el
    conteo manual semanal jamás entrega.

**Kit específico (además de la base de laboratorio)**

Sensor IR de conteo/paso, ESP32-CAM + base de programación (variante de
clasificación por imagen).

**Desafíos técnicos principales**

Conteo confiable (rebotes y dobles conteos del sensor IR); captura por
evento sin agotar la ESP32-CAM; operación autónoma en terreno (energía,
intemperie).

**Con tus propios datos podrás responder**

*¿A qué horas es más activa la plaga? ¿El conteo automático coincide con
el conteo manual de validación?*
