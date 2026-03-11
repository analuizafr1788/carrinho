// -------------------- Pinos sensores --------------------
const int TRIG_F = 12;   // Frontal - TRIG
const int ECHO_F = 13;   // Frontal - ECHO

// -------------------- Pinos motores ---------------------
const int PIN_MOTOR_IN1 = 2;
const int PIN_MOTOR_IN2 = 3;
const int PIN_MOTOR_IN3 = 4;
const int PIN_MOTOR_IN4 = 5;

// -------------------- Parâmetros de navegação -----------
const int LIMIAR_F_CM   = 20;   // distância (cm) para considerar obstáculo à frente
const int RECUO_MS      = 300;  // quanto tempo recua antes de girar
const int GIRO_DIR_MS   = 350;  // quanto tempo gira à direita (ajuste fino)
const int LOOP_DELAY_MS = 60;   // intervalo do loop para estabilizar leituras

// -------------------- Funções de motor ------------------
// Mover o robo para a frente
void mover_frente(void){
  digitalWrite(PIN_MOTOR_IN1, HIGH);
  digitalWrite(PIN_MOTOR_IN2, LOW);
  digitalWrite(PIN_MOTOR_IN3, HIGH);
  digitalWrite(PIN_MOTOR_IN4, LOW);
}

// Mover o robo para tras
void mover_tras(void){
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, HIGH);
  digitalWrite(PIN_MOTOR_IN3, LOW);
  digitalWrite(PIN_MOTOR_IN4, HIGH);
}

// Parar
void parar(void){
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, LOW);
  digitalWrite(PIN_MOTOR_IN3, LOW);
  digitalWrite(PIN_MOTOR_IN4, LOW);
}

// Girar à direita
void girar_direita(void){
  digitalWrite(PIN_MOTOR_IN1, HIGH);
  digitalWrite(PIN_MOTOR_IN2, LOW);
  digitalWrite(PIN_MOTOR_IN3, LOW);
  digitalWrite(PIN_MOTOR_IN4, HIGH);
}

// -------------------- Ultrasom (HC-SR04) ----------------
long distancia_cm(int trigPin, int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long dur = pulseIn(echoPin, HIGH, 25000UL);
  if (dur == 0) return 400;  // fora de alcance
  return (long)(dur / 58UL);
}

// -------------------- Setup -----------------------------
void setup(){
  // Motores
  pinMode(PIN_MOTOR_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IN2, OUTPUT);
  pinMode(PIN_MOTOR_IN3, OUTPUT);
  pinMode(PIN_MOTOR_IN4, OUTPUT);
  parar();

  // Sensor ultrassom frontal
  pinMode(TRIG_F, OUTPUT);
  pinMode(ECHO_F, INPUT);

  // Serial
  Serial.begin(9600);
  delay(200);
  Serial.println("Inicializado. Sensor frontal ativo (D12/D13)");
}

// -------------------- Loop principal --------------------
void loop(){
  long distF = distancia_cm(TRIG_F, ECHO_F); // Frente

  Serial.print("Frente [cm]: ");
  Serial.println(distF);

  // Lógica de desvio: se tem obstáculo à frente, recua e gira à direita
  if (distF <= LIMIAR_F_CM){
    parar();         delay(100);
    mover_tras();    delay(RECUO_MS);
    parar();         delay(100);
    girar_direita(); delay(GIRO_DIR_MS);
    parar();         delay(50);
  } else {
    mover_frente();
  }

  delay(LOOP_DELAY_MS);
}
