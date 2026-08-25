#include "HardwareSerial.h"
#include <Arduino.h>
#include <atomic>
#include <cstdint>
#include <ratio>
#include "DHTesp.h"
#include <iostream>
// Declaracion para el funcionamiento del Sesnor DHT22
const int DHT_PIN = 15;
DHTesp dhtSensor;
const byte MQ = 34; // pin analogico del sensor MQ

const byte buzzer = 33; // Pin del Buzer
const byte canalBuzzer = 0;
const byte IR = 35; // Pin del Sensor IR



//Definicion de los ESTADOS
enum ESTADO {VIGILANCIA, SOSPECHA, ALERTA_CONFIRMADA, ERROR};
ESTADO estadoActual = VIGILANCIA;
ESTADO anterior;
const uint8_t  N_FILTRO     = 5;       
const bool     USAR_MEDIANA = false;   // false: media movil | true: mediana

// Constantes de conversion cuentas -> magnitud fisica.
// ORIGEN: hoja de datos del sensor. SUSTITUYAN por las suyas y documenten.
const float V_REF        = 3.3f;       // [V] tension de referencia del ADC
const uint16_t CUENTAS_MAX = 4095;     // ADC de 12 bits: 0 a 4095
const float ESCALA_SENSOR = -0.36f;    // [grados C / V] sensibilidad nominal
const float OFFSET_SENSOR = 0.65f;      // [grados C] a 0 V, segun hoja de datos

// Constantes de la CALIBRACION DE DOS PUNTOS (item 4 de la pauta).
// Se obtienen midiendo, no se copian de otro equipo.
const float M_CAL = 0.968f;            // ganancia  (adimensional)
const float B_CAL = -2.40f;            // offset    [grados C]
// ---------------------------------------------------------------------------
// Estado Interno
int t_ultima_muestra = 0;
float ventana[N_FILTRO];
byte idx_ventana = 0;
int indice = 0;
bool ventana_llena = false;
unsigned muestras_validasMQ = 0;
byte MQlocales_invalidas;
unsigned muestras_invalidasMQ = 0;
unsigned muestas_validasDHT = 0;
unsigned muestras_invalidasDHT = 0;
const unsigned short out = 10000;

//=============================
//INICIO DE DATOS DE MQ-2
//configuracion para MQ-2
const float VC_MV = 5000.0;  
const float RL_KOHM = 2.0;  
//-------------------------------------------
//Funciones Auxiliares para MQ-2
int leer_mv() {                           // promedio de 8 lecturas, en mV
  long suma = 0;                          // calibradas de fabrica
  for (int i = 0; i < 5; i++) suma += analogReadMilliVolts(MQ);
  return (int)(suma / 5);
}

int filtrar(int valor) {                  // media movil de N_FILTRO muestras
  ventana[indice] = valor;
  indice = (indice + 1) % N_FILTRO;
  if (indice == 0) ventana_llena = true;

  int tope = ventana_llena ? N_FILTRO : indice;
  long suma = 0;
  for (int i = 0; i < tope; i++) suma += ventana[i];
  return (int)(suma / tope);
}

float resistencia_kohm(int mv) {          // divisor RS-RL del modulo
  if (mv <= 0) return -1.0;               // lectura invalida
  return RL_KOHM * (VC_MV - mv) / mv;
}
/*
Funcionamiento en loop()
  unsigned long ahora = millis();

  if (ahora - t_previo >= PERIODO_MS) {
    t_previo = ahora;
    n_muestra++;

    int crudo = leer_mv();
    int filtrado = filtrar(crudo);
    float rs = resistencia_kohm(filtrado);

    Serial.printf("%lu,%lu,%d,%d,%.2f\n",
                  n_muestra, ahora / 1000, crudo, filtrado, rs);
*/
//FIN DATOS DE MQ-2
//===========================================================================
//INICIO DE DATOS DE IR
//Configuraciones extras para el funcionamiento del Sensor IR
//  pinMode(PIN_LLAMA, INPUT_PULLUP);       Se declara con INPUT_PULLUP en setuo
// y al ser digital tiene solo dos posibles valores
//=======================================
/*INICIO DE DATOS DEL SESNSOR DHT22*/
/*Funcionamiento el loop (Para toma de medidas)

TempAndHumidity  data = dhtSensor.getTempAndHumidity();
    Serial.println("Temp: " + String(data.temperature, 2) + "°C");
    Serial.println("Humidity: " + String(data.humidity, 1) + "%");
    Serial.println("---");

*/

// --------------------------------------------------------------------------
//Definicion del tiempo de muestreo para cada sensor
const int muestreoMS = 500;
const int muestreoDHT = 2000;
const int muestreoIR = 500;
const int muestreoMQ = 3000;

unsigned long muestraMQ = 0;
unsigned long muestraIR = 0;
unsigned long muestraDHT = 0;
// ---------------------------------------------------------------------------
//Funcion provisional ya que no hay sensor IR en Wokwi
byte LecturaIR(){ 
    static unsigned long ultimoTiempo = 0;
    static int distancia = 1;
    int lectura = analogRead(IR);
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
int lecturaMQ(){
    int mV = leer_mv();
    int filtrado = filtrar(mV);
    return filtrado;
}
// {VIGILANCIA, SOSPECHA, ALERTA_CONFIRMADA, ERROR};
void cambioEstado(ESTADO actual){
    estadoActual = actual;
    const char* nombre = "";
    if (actual != anterior){
        anterior = actual;
        switch (actual){
            case VIGILANCIA:{
                nombre = "VIGILANCIA";
                break;
            }
            case SOSPECHA:{
                nombre = "SOSPECHA";
                break;
            }
            case ALERTA_CONFIRMADA:{
                nombre = "ALERTA CONFIRMADA";
                break;
            }
            case ERROR:{
                nombre = "ERROR";
                break;
            }
        }
        Serial.println(nombre);
    }
    // Agregar print para ver el cambio de estado 
}
/*
void avisar(bool encender) {              // zumbador pasivo: tono, no nivel
  if (encender) tone(buzzer, 2000);
  else noTone(buzzer);
}
*/

void avisar(bool encender) {              // zumbador pasivo: tono, no nivel
  if (encender) {
    ledcWriteTone(canalBuzzer, 2000);     // Emite el tono de 2000 Hz
  } else {
    ledcWriteTone(canalBuzzer, 0);        // Frecuencia 0 para silenciar (equivale a noTone)
  }
}

void setup() {
    Serial.begin(115200);
    dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
    analogSetPinAttenuation(MQ, ADC_11db);
    //pinMode(IR, INPUT_PULLUP); 
    pinMode(IR, INPUT);
    //pinMode(buzzer, OUTPUT);
    // Inicializa el canal 0 a 2000Hz con resolución de 8 bits
    
    ledcSetup(canalBuzzer, 2000, 8);
    ledcAttachPin(buzzer, canalBuzzer);
    

}

void loop() {
    /*
    1. Recopilacion de datos
    2. FMS (funcion interna la cual debe de cambiar el estado, no cambiarlo manualmente )
    3. Actuador
    Composicion del loop para un correcto funcionamiento, 
    */

    unsigned long tiempo = millis();
    int MQfiltrado;
    float resistenciaMQ;
    byte estadoIR;
    byte t;
    byte h;
    unsigned short tiempo_sospecha;
    bool senal;
    //primer condicional: Ingesta de datos de MQ
    if (tiempo - muestraMQ > muestreoMQ){
        muestraMQ = tiempo;
        MQfiltrado = lecturaMQ();
        resistenciaMQ = resistencia_kohm(MQfiltrado);

        if (resistenciaMQ == -1.0) {
            muestras_invalidasMQ += 1;
            MQlocales_invalidas += 1;
        }
        else muestras_validasMQ += 1;
        //agregar print para json
    }
    
    //segundo condicional: recopilacion de dato del IR
    if (tiempo - muestraIR > muestreoIR){
        //estadoIR = digitalRead(IR);
        estadoIR = LecturaIR();
    }


    //tercer condicional: recopilacion de datos de DHT
    if (tiempo - muestraDHT > muestreoDHT){
        TempAndHumidity  data = dhtSensor.getTempAndHumidity();
        t = data.temperature;
        h = data.humidity;
        //agregar condicional para aumentar muestras invalidas
        //agregar print para json
    }

    switch (estadoActual) {
        case VIGILANCIA:
            senal = false;
            if (resistenciaMQ <= 3.5 || estadoIR == 0) cambioEstado(SOSPECHA);
            else if (resistenciaMQ <= 3.5 && estadoIR == 0 ) cambioEstado(ALERTA_CONFIRMADA);
            else if(muestras_invalidasMQ == 3){
                MQlocales_invalidas = 0;
                cambioEstado(ERROR);
            }
            break;
        case SOSPECHA:
            senal = false;
            tiempo_sospecha = millis();
            if (resistenciaMQ <= 3.5 && estadoIR == 0 ) cambioEstado(ALERTA_CONFIRMADA);
            else if (tiempo_sospecha >= out) cambioEstado(VIGILANCIA);
            break;
            // implementar contador para cambiar a vigilancia
        case ALERTA_CONFIRMADA:
            senal = true;
            avisar(senal);
            if (estadoIR == 1 && resistenciaMQ >= 4) cambioEstado(VIGILANCIA);
            else if (resistenciaMQ >= 1.7 && estadoIR == 1) cambioEstado(SOSPECHA);
            break;
        case ERROR:
            if (MQlocales_invalidas < 3 ) cambioEstado(VIGILANCIA);
            break;
    }

}
