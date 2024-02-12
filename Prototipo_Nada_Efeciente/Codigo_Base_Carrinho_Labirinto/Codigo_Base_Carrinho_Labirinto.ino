#include <Servo.h> // Biblioteca para utilizar o módulo do servo motor.

// Pino do potenciômetro que tem função de mudar a velocidade:
#define pino_potenciometro A0

// Pinos do motor DC para o pneu direito:
#define pino_velocidade_m1 3
#define pino1_motor1 4
#define pino2_motor1 5

// Pinos do motor DC para o pneu esquerdo:
#define pino_velocidade_m2 6
#define pino1_motor2 7
#define pino2_motor2 8

// Pino ao qual o servo motor está conectado:
#define pino_servo 10

// Pinos do sensor ultrassônico:
#define pino_trigger 11
#define pino_echo 12

// Pino do botão que liga e desliga:
#define pino_botao 13

// Tempo que o servo motor espera para parar de girar (nesse caso esta definido como 1 segundo):
#define tempo_giro 1000

// Tempo de espera para parar a leitura:
#define tempo_leitura 1000

// Criando e definindo servo:
Servo servo;

// Variáveis globais:
int estado = 0;           // Variável para leitura do botão que liga e desliga.
int guardar_estado = LOW; // Variável que armazena os valores do botão.
int velocidade;           // Pode variar de 0(0%) a 255(100%) de potência do motor.
float distancia_minima;   // Distância mínima a que o objeto chega para virar.

// Classe do motor:
class Motor
{
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
Motor *motor_direito = new Motor(pino1_motor1, pino2_motor1, pino_velocidade_m1);
Motor *motor_esquerdo = new Motor(pino1_motor2, pino2_motor2, pino_velocidade_m2);

// Funções para simplificar a função "escolher_lado":
void direita_vira(int v)
{
    Serial.println("Virando para direita");
    motor_direito->frente(v);
    motor_esquerdo->re(v);
}
void esquerda_vira(int v)
{
    Serial.println("Virando para esquerda");
    motor_direito->re(v);
    motor_esquerdo->frente(v);
}
void frente(int v)
{
    Serial.println("Andando para frente");
    motor_direito->frente(v);
    motor_esquerdo->frente(v);
}
void re(int v)
{
    Serial.println("Dando re");
    motor_direito->re(v);
    motor_esquerdo->re(v);
}
void parar()
{
    Serial.println("Parando");
    motor_direito->parar();
    motor_esquerdo->parar();
}
void freiar()
{
    Serial.println("Freiando");
    motor_direito->freiar();
    motor_esquerdo->freiar();
}

// Função que irá decidir para qual lado virar:
void escolher_lado()
{
    float direita, esquerda;

    delay(tempo_leitura2); // Tempo de espera para virar o servo para a direita.
    servo.write(0);        // Olhando para a direita, com 0°.
    delay(tempo_leitura);  // Tempo de espera para o sensor começar a ler.

    digitalWrite(pino_trigger, LOW);
    delayMicroseconds(5);
    digitalWrite(pino_trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(pino_trigger, LOW);

    direita = pulseIn(pino_echo, HIGH); // Armazena o valor lido.
    direita = direita / 58;

    delay(tempo_leitura*2); // Tempo de espera para o servo virar para a esquerda.
    servo.write(180);     // Olha para esquerda, em 180°.
    delay(tempo_leitura*2); // Tempo de espera antes do servo começar a ler.

    digitalWrite(pino_trigger, LOW);
    delayMicroseconds(5);
    digitalWrite(pino_trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(pino_trigger, LOW);

    esquerda = pulseIn(pino_echo, HIGH); // Armazena o valor lido pelo sensor.
    esquerda = esquerda / 58;            // Converte para centímetros (cm).

    delay(tempo_leitura); // Tempo de espera para virar o servo para frente.
    servo.write(90);          // Olha para frente, em ângulo de 90°.
    delay(tempo_leitura); // Tempo de espera para o servo começar a decidir para onde virar.

    // Verificação do que o sensor está recebendo de cada lado:
    Serial.print("DIREITA..: ");
    Serial.println(direita);
    Serial.print("ESQUERDA.: ");
    Serial.println(esquerda);

    // Verificação do lado para o qual ele vai virar:
    if (direita > esquerda)
    {
        direita_vira(velocidade);
        delay(tempo_giro);
    }
    else
    {
        esquerda_vira(velocidade);
        delay(tempo_giro);
    }
    Serial.println("------------------------");
}

void setup()
{
    pinMode(pino_botao, OUTPUT);        // Define o pino_botao como saída de dados (manda informações).
    pinMode(pino_potenciometro, INPUT); // Define o potenciômetro como entrada de dados.

    pinMode(pino_trigger, OUTPUT); // Configura o pino_trigger como saída de dados.
    pinMode(pino_echo, INPUT);     // Configura o pino_echo como entrada de dados.

    velocidade = 255;       // 100% da velocidade possivel.
    distancia_minima = 100; // A 100 cm de distância do objeto, ele muda de distância.

    servo.attach(pino_servo); // Pino ao qual o servo vai mandar os dados.
    servo.write(90);          // Função servo.write serve para girar o servo. No caso 90° vai ser para a frente do robô.

    Serial.begin(9600); // Configura a taxa de transferência em bits por segundo (baud rate) para transmissão serial, no caso, configura a taxa de dados para 9600 bps.
    // delay(5000);      // 5 segundos antes do robô começar. OBS: PRECISA SER CONFIGURADO PRA COMEÇAR COM UM BOTÃO.
}

void loop()
{
    estado = digitalRead(pino_botao); // Lê estado HIGH (ligado) ou LOW (desligado).

    // Verificar o botão se foi pressionado:
    if (estado == HIGH)
    {
        guardar_estado = !guardar_estado; // Inverte o valor da variável.
        // delay(5000);                       // OBS: evitar erro.
    }

    if (guardar_estado == HIGH) // Ligar sistema pra começar tudo.
    {
        float distancia_cm; // Variável que ficará "lendo" até a distância ser menor que a distancia_minima.
        velocidade = map(analogRead(pino_potenciometro), 0, 1023, 0, 255);
        Serial.print("Velocidade:");
        Serial.println(velocidade);

        digitalWrite(pino_trigger, LOW); // Começa desligado.
        delayMicroseconds(5);
        digitalWrite(pino_trigger, HIGH); // Liga.
        delayMicroseconds(10);            // Tempo de exposição do laser.
        digitalWrite(pino_trigger, LOW);  // Desliga

        distancia_cm = pulseIn(pino_echo, HIGH); // Armazena o valor lido do sensor, que foi o tempo que o laser levou pra ir e voltar.
        distancia_cm = distancia_cm / 58;        // Converte para centímetros (cm).

        Serial.print("Distancia em cm: ");
        Serial.println(distancia_cm); // Verifica qual informação distancia_cm está recebendo em cm;

        if (distancia_cm < distancia_minima && distancia_cm != -1) // -1 ocorre se der erro/mal contato.
        {
            parar();         // Função que para o carrinho para decidir o lado para o qual ele irá.
            escolher_lado(); // Função que faz o carrinho decidir que lado ele vai ir.
        }
        else
        {
            frente(velocidade); // O carrinho vai continuar andando para frente até encontrar uma parede.
        }
    }
    else
    {
        Serial.println("Desligado");
    }
}
