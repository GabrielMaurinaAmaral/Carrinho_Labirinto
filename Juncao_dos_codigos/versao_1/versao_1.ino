// Biblioteca para utilizar o módulo do servo motor.
#include <Servo.h> 

// Pino do potenciômetro que tem função de mudar a velocidade:
#define pino_potenciometro A0

#define pino_velocidade_motor 12
// Pinos do motor DC para o pneu direito:
#define pino1_motor1 5
#define pino2_motor1 6

// Pinos do motor DC para o pneu esquerdo:
#define pino1_motor2 8
#define pino2_motor2 9

// Pinos do sensor ultrassônico:
#define pino_sensor_E 1
#define pino_sensor_F 2
#define pino_sensor_D 3


// Pino do botão que liga e desliga:
#define pino_botao 13

// Variáveis globais:
int estado;           // Variável para leitura do botão que liga e desliga.
int guardar_estado; // Variável que armazena os valores do botão.
int velocidade;           // Pode variar de 0(0%) a 255(100%) de potência do motor.
bool parede_frente;
bool parede_direita;
bool parede_esquerda;

// Classe do motor:
class Motor{
public:
    int pino_1, pino_2, pino_velocidade;

    // Método construtor do motor:
    Motor(int p1, int p2, int pv)
    {
        this->pino_1 = p1;
        this->pino_2 = p2;
        this->pino_velocidade = pv;

        // Definição dos pinos 1, 2, e de velocidade como saída de dados (output):
        pinMode(pino_1, OUTPUT);
        pinMode(pino_2, OUTPUT);
        pinMode(pino_velocidade, OUTPUT);

        // A função digitalWrite(pino, valor) muda o estado do pino, para HIGH(1) ou LOW(0).
        digitalWrite(pino_1, LOW); // Pino 1 começa desligado/em baixa tensão, logo começa parado.
        digitalWrite(pino_2, LOW); // Pino 2 começa desligado/em baixa tensão, logo começa parado.

        // Começa com valor analógico 0, podendo variar de 0 a 255(0% a 100% do motor). No caso, 100% corresponde a 140RPM em 4,5V.
        analogWrite(pino_velocidade, 0);
    }

    // Funções do motor:
    void frente(int v)
    {
        digitalWrite(pino_1, HIGH);
        digitalWrite(pino_2, LOW);
        analogWrite(pino_velocidade, v);
    }
    void re(int v)
    {
        digitalWrite(pino_1, LOW);
        digitalWrite(pino_2, HIGH);
        analogWrite(pino_velocidade, v);
    }
    void parar()
    {
        digitalWrite(pino_1, LOW);
        digitalWrite(pino_2, LOW);
        analogWrite(pino_velocidade, 0);
    }
    void freiar()
    {
        digitalWrite(pino_1, HIGH);
        digitalWrite(pino_2, HIGH);
        analogWrite(pino_velocidade, velocidade / 2); // Corta a velocidade para a metade (50%).
    }
};

// Criação do objeto para cada motor:
Motor *motor_direito = new Motor(pino1_motor1, pino2_motor1, pino_velocidade_motor);
Motor *motor_esquerdo = new Motor(pino1_motor2, pino2_motor2, pino_velocidade_motor);

// Funções para simplificar a função "escolher_lado":
void Direita_vira(int v)
{
    Serial.println("Virando para direita");
    motor_direito->frente(v);
    motor_esquerdo->re(v);
}
void Esquerda_vira(int v)
{
    Serial.println("Virando para esquerda");
    motor_direito->re(v);
    motor_esquerdo->frente(v);
}
void Frente(int v)
{
    Serial.println("Andando para frente");
    motor_direito->frente(v);
    motor_esquerdo->frente(v);
}
void Re(int v)
{
    Serial.println("Dando re");
    motor_direito->re(v);
    motor_esquerdo->re(v);
}
void Parar()
{
    Serial.println("Parando");
    motor_direito->parar();
    motor_esquerdo->parar();
}
void Freiar()
{
    Serial.println("Freiando");
    motor_direito->freiar();
    motor_esquerdo->freiar();
}

// Função que irá decidir para qual lado virar:
void escolher_lado(bool frente, bool esquerda, bool direita)
{
       if (frente == true) // -1 ocorre se der erro/mal contato.
    {
        if (esquerda == false && direita == false) // não encontrou parade em nenhum dos lado
            Serial.println("Esquerda ou  Direita");
        else if (esquerda == false && direita == true) // caminho livre a esquerda
            Serial.println("Esquerda");
        else if (esquerda == false && direita == true) // caminho livre a direita
            Serial.println("Direita");
        else if (esquerda == true && direita == true) // caminho sem inda  da a meia volta
            Serial.println("meia volta");
    }
    else
    {
        Serial.println("Frente");
    }
}

void setup()
{
    pinMode(pino_botao, OUTPUT);        // Define o pino_botao como saída de dados (manda informações).
    pinMode(pino_potenciometro, INPUT); // Define o potenciômetro como entrada de dados.

    pinMode(pino_sensor_F, INPUT);
    pinMode(pino_sensor_D, INPUT);
    pinMode(pino_sensor_E, INPUT);

    estado = 0;
    guardar_estado = LOW;
    velocidade = 255;      // 100% da velocidade possivel.

    Serial.begin(9600); // Configura a taxa de transferência em bits por segundo (baud rate) para transmissão serial, no caso, configura a taxa de dados para 9600 bps.
    // delay(5000);      // 5 segundos antes do robô começar. OBS: PRECISA SER CONFIGURADO PRA COMEÇAR COM UM BOTÃO.
}

void loop()
{
    estado = digitalRead(pino_botao); // Lê estado HIGH (ligado) ou LOW (desligado).

    // Verificar o botão se foi pressionado:
    if (estado == HIGH)
        guardar_estado = !guardar_estado; // Inverte o valor da variável.
        // delay(5000);                       // OBS: evitar erro.

    if (guardar_estado == HIGH) // Ligar sistema pra começar tudo.
    {
        velocidade = map(analogRead(pino_potenciometro), 0, 1023, 0, 255);

        parede_frente = digitalRead(pino_sensor_F);
        parede_esquerda = digitalRead(pino_sensor_E);
        parede_direita = digitalRead(pino_sensor_D);

        escolher_lado(parede_frente, parede_esquerda, parede_direita);
    }
    else
        Serial.println("Desligado");
}
