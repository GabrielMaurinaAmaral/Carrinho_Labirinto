#include <Servo.h> // Biblioteca para utilizar o módulo do servo motor.

// Pino do potenciômetro que tem função de mudar a velocidade:
#define pino_potenciometro A0

// Pinos do motor DC para o pneu direito:
#define pino_velocidade_m1 1
#define pino1_motor1 2
#define pino2_motor1 3

// Pinos do motor DC para o pneu esquerdo:
#define pino_velocidade_m2 4
#define pino1_motor2 5
#define pino2_motor2 6

// Pinos do sensor ultrassônico:
#define pino_trigger_F 7
#define pino_echo_F 8

#define pino_trigger_D 9
#define pino_echo_D 10

#define pino_trigger_E 11
#define pino_echo_E 12

// Pino do botão que liga e desliga:
#define pino_botao 13

// Tempo que o servo motor espera para parar de girar (nesse caso esta definido como 1 segundo):
#define tempo_giro 1000

// Tempo de espera para parar a leitura:
#define tempo_leitura 1000

// Variáveis globais:
int estado;             // Variável para leitura do botão que liga e desliga.
int guardar_estado;     // Variável que armazena os valores do botão.
int velocidade;         // Pode variar de 0(0%) a 255(100%) de potência do motor.
float distancia_minima; // Distância mínima a que o objeto chega para virar.
float distancia_frente;
float distancia_direita;
float distancia_esquerda;

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
void escolher_lado(int frente, int esquerda, int direita)
{
    // analisa se encontrou parede em frente
    if (frente < distancia_minima && frente != -1) // -1 ocorre se der erro/mal contato.
    {
        if (esquerda > distancia_minima && direita > distancia_minima) // não encontrou parade em nenhum dos lado
            if (esquerda > direita)                                    // ver aquer tem a mior distancia livre de objetos se detectados
                Esquerda_vira(velocidade);
            else
                Direita_vira(velocidade);
        else if (esquerda > distancia_minima && direita < distancia_minima) // caminho livre a esquerda
            Esquerda_vira(velocidade);
        else if (esquerda < distancia_minima && direita > distancia_minima) // caminho livre a direita
            Direita_vira(velocidade);
        else if (esquerda < distancia_minima && direita < distancia_minima) // caminho sem inda  da a meia volta
            Esquerda_vira(velocidade), Esquerda_vira(velocidade);
    }
    else
    {
        Frente(velocidade);
    }
}

// função que retorna a distancia em cm da parede
float usar_sensor(int pino_trigger, int pino_echo)
{
    digitalWrite(pino_trigger, LOW);
    delayMicroseconds(5);
    digitalWrite(pino_trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(pino_trigger, LOW);

    return pulseIn(pino_echo, HIGH) / 58;
}

void setup()
{
    pinMode(pino_botao, OUTPUT);        // Define o pino_botao como saída de dados (manda informações).
    pinMode(pino_potenciometro, INPUT); // Define o potenciômetro como entrada de dados.

    pinMode(pino_trigger_F, OUTPUT); // Configura o pino_trigger_F como saída de dados.
    pinMode(pino_echo_F, INPUT);     // Configura o pino_echo_F como entrada de dados.

    pinMode(pino_trigger_E, OUTPUT);
    pinMode(pino_echo_E, INPUT);

    pinMode(pino_trigger_D, OUTPUT);
    pinMode(pino_echo_D, INPUT);

    estado = 0;
    guardar_estado = 0;
    velocidade = 255;      // 100% da velocidade possivel.
    distancia_minima = 50; // A 100 cm de distância do objeto, ele muda de distância.

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
        Serial.print("velocidade: ");
        Serial.println(velocidade);

        distancia_frente = usar_sensor(pino_trigger_F, pino_echo_F);
        Serial.print("distancia_frente: ");
        Serial.println(distancia_frente);

        distancia_esquerda = usar_sensor(pino_trigger_E, pino_echo_E);
        Serial.print("distancia_esquerda: ");
        Serial.println(distancia_esquerda);

        distancia_direita = usar_sensor(pino_trigger_D, pino_echo_D);
        Serial.print("distancia_direita: ");
        Serial.println(distancia_direita);

        escolher_lado(distancia_frente, distancia_esquerda, distancia_direita);
    }
    else
        Serial.println("Desligado");
}