#include "HardwareSerial.h"
#include <Arduino.h>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <ratio>
#include "DHTesp.h"
#include <iostream>

DHTesp dhtSensor;
const byte DHT_PIN = 15;
const byte MQ = 34;
const byte buzzer = 33;
const byte canalBuzzer = 0;
const byte IR = 35;

enum ESTADO {VIGILANCIA, SOSPECHA, ALERTA_CONFIRMADA, ERROR};
ESTADO estadoActual = VIGILANCIA;
ESTADO anterior;
//Codigo dado por el profesor
//=================================================================================
const uint8_t N_FILTRO = 5;
const bool USAR_MEDIANA = false;
const float V_REF        = 3.3f;       // [V] tension de referencia del ADC
const uint16_t CUENTAS_MAX = 4095;     // ADC de 12 bits: 0 a 4095
const float ESCALA_SENSOR = -0.36f;    // [grados C / V] sensibilidad nominal
const float OFFSET_SENSOR = 0.65f;  

//Estado interno
//=================================================================================
int t_ultima_muestra = 0;
float ventana[N_FILTRO];
byte idx_ventana = 0;
int indice =0;
bool ventana_llena = false;


//Contador de muestras validas e invalidas
//============================================================================
unsigned DHTmuestrasValidas;
unsigned MQmuestrasValidas;
unsigned IRmuestrasValidas;
unsigned DHTmuestrasInvalidas;
unsigned DHTlocalesInvalidas;
unsigned MQmuestrasInvalidas;
unsigned IRMuestasInvalidas;


//Tiempo de muestreo de cada sensor
//============================================================================
const int muestreoDHT = 2000; //valor normal de 60000
const int muestreoIR = 500;
const int muestreoMQ = 3000;

//Contadores de muestreo para cada sensor
//============================================================================
unsigned long muestraMQ;
unsigned long muestraIR;
unsigned long muestraDHT;


//Tiempos de espera
//============================================================================
const int tiempoMaximo = 100000;
const int actualizacion = 600000;


//Funciones basicas para el sensor MQ-2
//============================================================================
//Declaraciones necesarias para funciones del sensor MQ-2
const float VC_MV = 5000.0;  
const float RL_KOHM = 2.0;  
int leer_mv(){
    long suma;
    for (int i= 0; i < 5; i++) suma += analogReadMilliVolts(MQ);
    return (int)(suma/5);
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


//Funciones para el sensor DHT-22
//============================================================================
//Inicio de variables globales para dht
float temp;
float tempAnterior;
float humedad;
float tempActual;
void lecturasDHT(){
	TempAndHumidity data = dhtSensor.getTempAndHumidity();
	temp = data.temperature;
	humedad = data.humidity;
	if(isnan(temp) || isnan(humedad)){
		DHTlocalesInvalidas+=1;
		DHTmuestrasInvalidas+=1;
		Serial.println("lectura invalida DHT");
		return;
	}
	Serial.printf("La temperatura es: %.2f y la humedad es: %.2f\n", temp, humedad);
}

void actualizacionTemp(float temp){
	if (isnan(temp)) tempActual = temp;
	Serial.print("La temperatura actual es: ");
	Serial.println(tempActual);
}

float resta() {
	return tempActual - tempAnterior;
}
//Funciones para el sensor IR
//============================================================================
int desc;
int nivelIR(){
    int lectura = digitalRead(IR);
	Serial.printf("El sensor IR esta actualmente en: %d", lectura);
    return lectura;
}


//Funciones Generales
//============================================================================
void lecturaMQ(){
	int suma = leer_mv();
	int filtrado = filtrar(suma);
	int resistencia = resistencia_kohm(filtrado);
	if (resistencia != -1) MQmuestrasValidas +=1;
	else MQmuestrasInvalidas +=1;
	Serial.printf("Los datos de MQ son: %d, %d.\n", filtrado, resistencia);
	
}
/*
bool isEsperaMaxima(){
    static unsigned long tiempo;
    static bool flag = false;
    if (estadoActual == SOSPECHA){
        tiempo += millis();
        flag = false;
    	if (tiempo >= tiempoMaximo){
        	tiempo = 0;
        	flag = true;
    	}
	}
	return flag;
}
*/
bool isEsperaMaxima() {
    static unsigned long tiempoInicio = 0;
    static bool midiendo = false;

    if (estadoActual == SOSPECHA) {
        // Registra la marca de tiempo solo la primera vez que entra a este estado
        if (!midiendo) {
            tiempoInicio = millis(); 
            midiendo = true;
        }
        
        // Calcula la diferencia para ver si se alcanzó el tiempo máximo
        if (millis() - tiempoInicio >= tiempoMaximo) {
            midiendo = false; // Se reinicia el estado para futuros usos
            return true;
        }
    } else {
        // Si el estado cambia a VIGILANCIA o ALERTA, se aborta y reinicia el temporizador
        midiendo = false; 
    }
    
    return false;
}
/*
void avisar(bool encender) {              // funcion para buzzer en caso de que placa no acepte ledc
  if (encender) tone(buzzer, 2000);
  else noTone(buzzer);
}
*/

void avisar(bool encender){         //funcion para buzzer / ledc
    if (encender) {
    ledcWriteTone(canalBuzzer, 2000);     // Emite el tono de 2000 Hz
    } else {
    ledcWriteTone(canalBuzzer, 0);        // Frecuencia 0 para silenciar (equivale a noTone)
  }
}

const char* cambiarNombre(ESTADO e) {
  switch (e) {
    case VIGILANCIA: return "VIGILANDO";
    case SOSPECHA:  return "SOSPECHA";
    case ALERTA_CONFIRMADA: return "ALERTA";
    case ERROR: return "ERROR";
  }
  return "?";
}
//Funcion para poder ver el cambio de nombre a lo largo de la comunicacion serial
void cambioEstado(ESTADO actual){
    estadoActual = actual;
    if (actual != anterior){
		DHTlocalesInvalidas = 0;
		anterior = actual;
		Serial.printf("Estado anterior: %s | Estado actual: %s\n", cambiarNombre(anterior), cambiarNombre(actual));
	}
}

bool lectura = true;
void setup(){
	Serial.begin(115200);
	dhtSensor.setup(DHT_PIN, DHTesp::DHT11);
	analogSetPinAttenuation(MQ, ADC_11db);
	pinMode(IR, INPUT_PULLUP);
	ledcSetup(canalBuzzer, 2000, 8);
	ledcAttachPin(buzzer, canalBuzzer);

	//llamada por primera vez para obtener las primeras lecturas
	
}

void loop(){
	unsigned long time = millis();
	unsigned long ultimoTimeAc = 0;
	static bool ac;
	static bool senal;
	
	if (lectura){
		Serial.println("Primera Lectura");
		lecturasDHT();
		actualizacionTemp(temp);
		lecturaMQ();
		desc = nivelIR();
		lectura = false;
		Serial.printf("Estado actual: %s\n", cambiarNombre(estadoActual));
	}
		

	if (time - muestraMQ > muestreoMQ){
		muestraMQ = millis();
		lecturaMQ();
	}

	if (time - muestraDHT > muestreoDHT){
		muestraDHT = millis();
		lecturasDHT();
	}

	if (time - ultimoTimeAc > actualizacion)actualizacionTemp(temp);

	if (time - muestraIR > muestreoIR){
		muestraIR = millis();
		ac = nivelIR();
	}

	switch(estadoActual){
		case VIGILANCIA:{
			senal = false;
			if (resta() > 5 || ac != desc) cambioEstado(SOSPECHA);
			else if( resta() > 5 && ac != desc) cambioEstado(ALERTA_CONFIRMADA);
			else if(DHTlocalesInvalidas >= 3){
				DHTlocalesInvalidas = 0;
				cambioEstado(ERROR);
			}
			break;
		}
		case SOSPECHA:{
			senal = false;
			if (resta() > 5 && ac != desc) cambioEstado(ALERTA_CONFIRMADA);
			else if(isEsperaMaxima()) cambioEstado(VIGILANCIA);
			else if(DHTlocalesInvalidas >= 3){
				DHTlocalesInvalidas = desc;
				cambioEstado(ERROR);
			}
			break;
		}
		case ALERTA_CONFIRMADA:{
			senal = true;
			avisar(senal);
			if (resta() < 3 && ac == desc) cambioEstado(VIGILANCIA);
			else if(resta() < 5 || ac == desc) cambioEstado(SOSPECHA);
			else if(DHTlocalesInvalidas >= 3){
				DHTlocalesInvalidas = 0;
				cambioEstado(ERROR);
			}
			break;
		}
		case ERROR:{
			if (DHTlocalesInvalidas < 3 ) cambioEstado(VIGILANCIA);
			break;
		}
	}
}