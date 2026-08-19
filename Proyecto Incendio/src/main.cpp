#include "HardwareSerial.h"
#include <Arduino.h>
#include <DHT.h>

/*
Universidad Autonoma de Chile - Ingenieria Civil Informatica
v1.0 por R.Rodriguez
v2.0 en proceso
*/

// declaracion de los pines a utilizar en el proyecto
#define DHTpin 32
#define DHTtype DHT22
const byte analogMQ = 35;
const byte buzzer = 33;
const byte pin_sensor = 34;

// declaracion de los tiempos de muestreo de cada sensor/actuador que se utilizara en el proyecto
const byte periodo_ms = 34;
const int muestreoMS = 500;
const int muestreoDHT = 25000;
const int muestreoIR = 5000;
const int muestreoMQ = 20000;

//declaracion para el funcionaminto de la libreria DHT
DHT dht(DHTpin, DHTtype);

const byte N_FILTRO = 5;            //filtro que usaremos en el proyecto
const bool USAR_MEDIANA = false;        //false: media movil || true: mediana

const float vRef = 3.3f;        // voltaje referencial de los pines GPIO para el funcionamiento de los sensores
const int cuentasMax = 4095;        //resolucion para las lecturas analogicas de los pines a utilizar. resolucion de 12 bits, entonces 2¹² = 4096, o sea de 0 a 4095
const float escalaSensor = -0.36f;          //Sensibilidad nominal del sensor a calibrar (MQ-135 en proyecto; MQ-2 en Wokwi) equivalencia de °C a V
const float offsetSensor = 0.65f;           // cantidad de °C cuando hay 0 V


const float M_CAL = 0.968f;         //ganancia (adimencional)
const float B_CAL = -2.40f;         // offset (°C)

//==============================================================================================================
// Estado interno

int t_ultima_muestra = 0;
float ventana[16];
byte idx_ventana = 0;
byte muestras_validas = 0;

//==============================================================================================================
// Funciones Auxiliares

float cuentas_a_fisica(int cuentas) {
  float tension = (vRef * cuentas) / cuentasMax;         // [V]
  return tension * escalaSensor + escalaSensor;          // [grados C]
}

// Aplica la correccion de dos puntos:  y_corregido = m * y_medido + b
float aplicar_calibracion(float valor_medido) {
  return M_CAL * valor_medido + B_CAL;
}

// Media movil sobre la ventana.
float media_movil() {
  float suma = 0.0f;
  for (uint8_t i = 0; i < muestras_validas; i++) suma += ventana[i];
  return (muestras_validas > 0) ? (suma / muestras_validas) : 0.0f;
}

// Mediana sobre la ventana. Elimina picos aislados sin arrastrar el valor.
float mediana() {
  if (muestras_validas == 0) return 0.0f;
  float copia[16];
  for (uint8_t i = 0; i < muestras_validas; i++) copia[i] = ventana[i];
  for (uint8_t i = 1; i < muestras_validas; i++) {      // insercion simple
    float clave = copia[i];
    int8_t j = i - 1;
    while (j >= 0 && copia[j] > clave) { copia[j + 1] = copia[j]; j--; }
    copia[j + 1] = clave;
  }
  return copia[muestras_validas / 2];
}

//==============================================================================================================
// Programa
byte LecturaIR(){
    static unsigned long ultimoTiempo = 0;
    static int distancia = 1;
    int lectura = analogRead(pin_sensor);
    if (millis() - ultimoTiempo >= muestreoIR){
      ultimoTiempo = millis();
      if (lectura < 4095/2){
        distancia = 1;
      }else if (lectura > 4095/2){
        distancia = 0;
      }
    }
    return distancia;
}

float LecturaTempDHT(){
    static float t = 0;
    static unsigned long ultimoTiempo = 0;
    if(millis() - ultimoTiempo >= muestreoDHT){
      ultimoTiempo = millis();
        t = dht.readTemperature();
    }
    return t;
}

float LecturaHumDHT(){
  static float h = 0;
  static unsigned long ultimoTiempo = 0;
  if(millis() - ultimoTiempo >= muestreoDHT){
    ultimoTiempo = millis();
      h = dht.readHumidity();
  }
  return h;
}

float LecturaMQ(){
    static unsigned long ultimoTiempo = 0;
    static int adc = 0;
    if(millis() - ultimoTiempo >= muestreoMQ){
      ultimoTiempo = millis();
      int lectura = analogRead(analogMQ);
      adc = cuentas_a_fisica(lectura);  //conversion Item 3
    }
    return adc;
}

void setup() {
 // Asegúrate de que este número sea el mismo en tu platformio.ini
  Serial.begin(115200); 
  
  // Esperar a que el puerto serial se inicialice (muy útil en placas reales también)
  while (!Serial) {
    delay(10);
  }
  
  // Darle 2 segundos adicionales al simulador para conectar la terminal
  delay(2000); 
  
  Serial.println("¡Hola mundo desde el setup!");
  
  analogReadResolution(12);            // 12 bits: 0 a 4095
  Serial.println("t_ms,cuentas,calibrado_C,filtrado_C");   // cabecera CSV
  dht.begin();
  pinMode(analogMQ, INPUT);
  pinMode(pin_sensor, INPUT);
  pinMode(buzzer, OUTPUT);
  
}

void loop() {
  
  Serial.println("Hola desde el ESP32 en VSCodium!");
  delay(2000);
  
  


  uint32_t ahora = millis();
  byte valor = 28;
  // Muestreo agendado: el lazo nunca se bloquea, de modo que mas adelante
  // puede convivir con la maquina de estados (Semana 4) y con la publicacion
  // MQTT (Semana 8) sin reescribir esta estructura.
  if (ahora - t_ultima_muestra >= periodo_ms) {
    t_ultima_muestra = ahora;

    float IR = LecturaIR();
    float humedad = LecturaHumDHT();
    float temp = LecturaTempDHT();

    float calibrado = aplicar_calibracion(valor);      // correccion  (item 4)
    float cuentas = LecturaMQ();
    ventana[idx_ventana] = calibrado;                  // filtrado    (item 5)
    idx_ventana = (idx_ventana + 1) % N_FILTRO;
    if (muestras_validas < N_FILTRO) muestras_validas++;

    float filtrado = USAR_MEDIANA ? mediana() : media_movil();

    Serial.printf("%lu,%.2f,%.2f,%.2f\n", ahora, cuentas, calibrado, filtrado);
    
  }
    
  
}