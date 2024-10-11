#include <ESP32Servo.h>

// Pines del sensor ultrasónico
const int EchoPin = 33;   // Pin conectado al Echo del sensor
const int TriggerPin = 25; // Pin conectado al Trigger del sensor

// Pines de los motores
const int motorDelanteroAdelante = 12;  // OUT1
const int motorDelanteroAtras = 14;     // OUT2
const int motorTraseroAdelante = 27;    // OUT3
const int motorTraseroAtras = 26;       // OUT4

// Pines y configuración del servo
#define PINSERVO 32
Servo servoMotor;

void setup() {
  // Iniciar el monitor serie
  Serial.begin(115200);

  // Configurar pines de motores como salidas
  pinMode(motorDelanteroAdelante, OUTPUT);
  pinMode(motorDelanteroAtras, OUTPUT);
  pinMode(motorTraseroAdelante, OUTPUT);
  pinMode(motorTraseroAtras, OUTPUT);

  // Configurar pines del sensor ultrasónico
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  // Iniciar el servo
  servoMotor.attach(PINSERVO);
  servoMotor.write(90);  // Posicionar el servo al centro inicialmente
}

void loop() {
  float distancia = medirDistancia();
  Serial.print("Distancia: ");
  Serial.println(distancia);

  if (distancia > 50.0) {
    avanzar(500);  // No hay obstáculos, avanzar
  } else {
    detenerse(500);  // Hay un obstáculo, buscar una nueva salida
    explorarYBuscarSalida();
  }
}

// Función para medir la distancia con el sensor ultrasónico
float medirDistancia() {
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);

  unsigned long duration = pulseIn(EchoPin, HIGH);
  return float(duration) / 58.2;  // Convertir a cm
}

// Función para explorar y buscar una nueva salida
void explorarYBuscarSalida() {
  float distanciaIzquierda, distanciaDerecha;

  // Mover el servo a 45 grados a la izquierda y medir
  servoMotor.write(45);
  delay(500);
  distanciaIzquierda = medirDistancia();

  // Mover el servo a 135 grados a la derecha y medir
  servoMotor.write(135);
  delay(500);
  distanciaDerecha = medirDistancia();

  // Regresar el servo a su posición central
  servoMotor.write(90);

  // Decisiones basadas en las mediciones
  if (distanciaIzquierda > 20.0) {
    girarIzquierda(500);  // Si hay espacio a la izquierda, girar
  } else if (distanciaDerecha > 20.0) {
    girarDerecha(500);  // Si hay espacio a la derecha, girar
  } else {
    retroceder(1000);  // Si no hay espacio, retroceder
  }
}

// Funciones de control de movimiento
void avanzar(int tiempo) {
  moverMotores(HIGH, LOW, HIGH, LOW, tiempo);  // Avanzar
}

void retroceder(int tiempo) {
  moverMotores(LOW, HIGH, LOW, HIGH, tiempo);  // Retroceder
}

void girarIzquierda(int tiempo) {
  moverMotores(HIGH, LOW, LOW, HIGH, tiempo);  // Girar hacia la izquierda
}

void girarDerecha(int tiempo) {
  moverMotores(LOW, HIGH, HIGH, LOW, tiempo);  // Girar hacia la derecha
}

void detenerse(int tiempo) {
  detenerMotores();
  delay(tiempo);
}

void detenerMotores() {
  moverMotores(LOW, LOW, LOW, LOW, 0);  // Detener todos los motores
}

// Función para controlar el movimiento de los motores
void moverMotores(int delanteroAdelante, int delanteroAtras, int traseroAdelante, int traseroAtras, int tiempo) {
  digitalWrite(motorDelanteroAdelante, delanteroAdelante);
  digitalWrite(motorDelanteroAtras, delanteroAtras);
  digitalWrite(motorTraseroAdelante, traseroAdelante);
  digitalWrite(motorTraseroAtras, traseroAtras);
  
  if (tiempo > 0) {
    delay(tiempo);
    detenerMotores();  // Detener los motores después del tiempo especificado
  }
}


