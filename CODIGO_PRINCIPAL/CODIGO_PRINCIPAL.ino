#define pino1_motor_E 5
#define pino2_motor_E 6
#define pino1_motor_D 9
#define pino2_motor_D 10
// foif
#define pino_sensor_E 2
#define pino_sensor_F 3
#define pino_sensor_D 4

int velocidade;
bool parede_frente;
bool parede_direita;
bool parede_esquerda;

class Motor{
public:
    int pino_1, pino_2;

    Motor(int p1, int p2)
    {
        this->pino_1 = p1;
        this->pino_2 = p2;

        pinMode(pino_1, OUTPUT);
        pinMode(pino_2, OUTPUT);

        digitalWrite(pino_1, LOW); 
        digitalWrite(pino_2, LOW); 
    }

    // Funções do motor:
    void frente(int v)
    {
        analogWrite(pino_1, v);
        digitalWrite(pino_2, LOW);
    }
    void re(int v)
    {
        digitalWrite(pino_1, LOW);
        analogWrite(pino_2, v);
    }
    void parar()
    {
        digitalWrite(pino_1, LOW);
        digitalWrite(pino_2, LOW);
    }
    void freiar(int v)
    {
        analogWrite(pino_2, v/4);
        analogWrite(pino_2, v/4);
    }
};

// Criação do objeto para cada motor:
Motor *motor_direito = new Motor(pino1_motor_D, pino2_motor_D);
Motor *motor_esquerdo = new Motor(pino1_motor_E, pino2_motor_E);

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
void Freiar(int v)
{
    Serial.println("Freiando");
    motor_direito->freiar(v);
    motor_esquerdo->freiar(v);
}

// Função que irá decidir para qual lado virar:
void escolher_lado(bool frente, bool esquerda, bool direita)
{
    if (frente == false) // -1 ocorre se der erro/mal contato.
    {
        if (esquerda == true && direita == true) // não encontrou parade em nenhum dos lado
        {
            Direita_vira(velocidade);
            delay(1000);
        }
        else if (esquerda == true && direita == false) // caminho livre a esquerda
        {
            Esquerda_vira(velocidade);
            delay(1000);
        }
        else if (esquerda == false && direita == true) // caminho livre a direita
        {
            Direita_vira(velocidade);
            delay(1000);
        }
        else if (esquerda == false && direita == false) // caminho sem inda  da a meia volta
        {
            Esquerda_vira(velocidade);
            delay(1000);
        }
    }
    else
    {
        Frente(velocidade);  
    }
}

void setup()
{
    pinMode(pino_sensor_F, INPUT);
    pinMode(pino_sensor_D, INPUT);
    pinMode(pino_sensor_E, INPUT);

    velocidade = 150;
    Serial.begin(9600);      
}

void loop()
{
    parede_frente = digitalRead(pino_sensor_F);
    parede_esquerda = digitalRead(pino_sensor_E);
    parede_direita = digitalRead(pino_sensor_D);
    Serial.println(parede_esquerda);
    Serial.println(parede_frente);
    Serial.println(parede_direita);
    escolher_lado(parede_frente, parede_esquerda, parede_direita);
}
