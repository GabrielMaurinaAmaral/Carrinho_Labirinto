#include <Servo.h>
Servo servo;
#define pino_velocidade_m1 3
#define pino1_motor1 4
#define pino2_motor1 5
#define pino_velocidade_m2 6
#define pino1_motor2 7
#define pino2_motor2 8
#define pino_servo 10
#define pino_trigger 11
#define pino_echo 12
#define tempo_giro 1000
#define tempo_leitura 1000
int velocidade;
float distancia_minima;
class Motor{
public:
    int pino_1, pino_2, pino_velocidade;
    Motor(int p1, int p2, int pv){
        this->pino_1 = p1;
        this->pino_2 = p2;
        this->pino_velocidade = pv;
        pinMode(pino_1, OUTPUT);
        pinMode(pino_2, OUTPUT);
        pinMode(pino_velocidade, OUTPUT);
        digitalWrite(pino_1, LOW);
        digitalWrite(pino_2, LOW);
        analogWrite(pino_velocidade, 0);
    }
    void frente(int v){
        digitalWrite(pino_1, HIGH);
        digitalWrite(pino_2, LOW);
        analogWrite(pino_velocidade, v);
    }
    void re(int v){
        digitalWrite(pino_1, LOW);
        digitalWrite(pino_2, HIGH);
        analogWrite(pino_velocidade, v);
    }
    void parar(){
        digitalWrite(pino_1, LOW);
        digitalWrite(pino_2, LOW);
        analogWrite(pino_velocidade, 0);
    }
    void freiar(){
        digitalWrite(pino_1, HIGH);
        digitalWrite(pino_2, HIGH);
        analogWrite(pino_velocidade, velocidade / 2); 
    }
};
Motor *motor_direito = new Motor(pino1_motor1, pino2_motor1, pino_velocidade_m1);
Motor *motor_esquerdo = new Motor(pino1_motor2, pino2_motor2, pino_velocidade_m2);
void direita_vira(int v){
    motor_direito->frente(v);
    motor_esquerdo->re(v);
}
void esquerda_vira(int v){
    motor_direito->re(v);
    motor_esquerdo->frente(v);
}
void frente(int v){
    motor_direito->frente(v);
    motor_esquerdo->frente(v);
}
void re(int v){
    motor_direito->re(v);
    motor_esquerdo->re(v);
}
void parar(){
    motor_direito->parar();
    motor_esquerdo->parar();
}
void freiar(){
    motor_direito->freiar();
    motor_esquerdo->freiar();
}
void escolher_lado(){
    float direita, esquerda;
    delay(tempo_leitura / 2); 
    servo.write(0);           
    delay(tempo_leitura / 2); 
    digitalWrite(pino_trigger, LOW);
    delayMicroseconds(5);
    digitalWrite(pino_trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(pino_trigger, LOW);
    direita = pulseIn(pino_echo, HIGH); 
    direita = direita / 58;
    delay(tempo_leitura); 
    servo.write(180);     
    delay(tempo_leitura); 
    digitalWrite(pino_trigger, LOW);
    delayMicroseconds(5);
    digitalWrite(pino_trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(pino_trigger, LOW);
    esquerda = pulseIn(pino_echo, HIGH); 
    esquerda = esquerda / 58;    
    delay(tempo_leitura / 2);     
    servo.write(90);          
    delay(tempo_leitura / 2); 
    if (direita > esquerda){
        direita_vira(velocidade);
        delay(tempo_giro);
    }
    else{
        esquerda_vira(velocidade);
        delay(tempo_giro);
    }
}
void setup(){
    pinMode(pino_trigger, OUTPUT);     
    pinMode(pino_echo, INPUT);     
    velocidade = 255;       
    distancia_minima = 100; 
    servo.attach(pino_servo); 
    servo.write(90);          
    Serial.begin(9600); 
}
void loop(){
    float distancia_cm; 
    digitalWrite(pino_trigger, LOW); 
    delayMicroseconds(5);
    digitalWrite(pino_trigger, HIGH);
    delayMicroseconds(10);           
    digitalWrite(pino_trigger, LOW); 
    distancia_cm = pulseIn(pino_echo, HIGH);
    distancia_cm = distancia_cm / 58;
    if (distancia_cm >+ distancia_minima && distancia_cm != -1){
        parar();
        escolher_lado();
    }
    else
        frente(velocidade);
}
