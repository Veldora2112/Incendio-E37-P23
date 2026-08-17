## Sensor DHT 22
Para el sensor DHT22, se usa la biblioteca "DHT sensor library" de Adafruit junto con el Adafruit Unified Sensor,
lo primero se incluye "#include <DHT.h>", se define el pin y el modelo con "#define DHTPIN 4" y luego con "#define
DHTTYPE DHT22", y se instancia el objeto con "DHT dht(DHTPIN, DHTTYPE);". En la función de setup se inicia con
"dht.begin()", y en el loop() las lecturas se obtienen con "dht.readTemperature()" para temperatura y
"dht.readHumidity()" para humedad.
Muestreo cada: 20-30 s
## Sensor IR
El sensor IR, tampoco necsita bibloteca, porque da una señal directa, en el código lo define con el pin
 "define IR_PIN 18", en la función "setup()" se inicia como entrada con el "pinMode(IR_PIN, INPUT);"
y en el "loop()" la lectura para la deteccion se obtiene con "int llama = digitalRead(IR_PIN);",
esto devuelve un 0 si detecta radiacion infrarroja de una llama o un 1 si esta normal.
Muestreo cada: 5-10 s

## Sensor MQ-135 y MQ-2
El sensor MQ-135 y MQ-2, no necesitan biblioteca, porque se leen directamente, en los comandos se definen con
"#define MQ_PIN 34", conectándolo a un canal del ADC1 (GPIO 32 a 39) para que no interfiera cuando se use WiFi.
En la función setup() se configura como entrada con "pinMode(MQ_PIN, INPUT);", y en el "loop()" la lectura se
obtiene con "int rawADC = analogRead(MQ_PIN);", lo que da valores de 0 a 4095 en el conversor de 12 bits del ESP32
Muestreo cada: 20 s
