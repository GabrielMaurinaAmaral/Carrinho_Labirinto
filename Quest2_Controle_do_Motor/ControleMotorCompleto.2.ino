/**
 ******************************************************************************
 * @file    ControleMotorCompleto.ino
 * @authors Caroline, Gabriel Amaral, Joao Ferreira, Lucas Alves e Pedro.
 * @brief   Código para controle de motor pelo sensor de presença ultrassônico.
 ******************************************************************************
 *
 **/

#include <HCSR04.h> // Inclusão da biblioteca do sensor de presença.
#define PINO_PWM 3  // Inicializa a saída PWM ~3.
#define PINO2_PWM 9 // Inicializa a saída PWM2 ~9.
#define BOTAO 4     // Botão ligado/desligado.

unsigned int valor_pwm = 0; // Variável que vai controlar o PWM. 0 = 0% e 255 = 100% da potência do motor.
int distancia = 0;          // Variável que recebe a distância do sensor.
char estado_motor = 0;      // Variável de controle do estado_motor do motor (Acelerar/Freiar).

int estado_botao = 0;
int guardar_estado = LOW;

HCSR04 hc(5, 6); // Inicialização do sensor.

void setup()
{
    Serial.begin(9600);         // Inicializa Serial.
    pinMode(BOTAO, OUTPUT);     // Configura o botão como entrada de sinal.
    pinMode(PINO2_PWM, OUTPUT); // Configura o PINO2_PWM como saída de sinal.
    pinMode(PINO_PWM, OUTPUT);  // Configura o PINO_PWM como saida de sinal.
    analogWrite(PINO_PWM, 0);   // Configura como 0 o PWM.
    analogWrite(PINO2_PWM, 0);  // Configura como 0 o PWM.
}

void loop()
{
    estado_botao = digitalRead(pino_botao);

    if (estado_botao == HIGH)
    {
        guardar_estado = !guardar_estado; // Inverte o valor da variável.
        delay(5000);                      // OBS: evitar erro.
    }

    if (estado_botao == HIGH)
    {
        switch (estado_motor) // Máquina de estado_motor do motor.
        {
        case 0: // Aumenta a velocidade do motor.
            if (valor_pwm < 255)
            {                                     // Como 255 é o maior valor possível, o robô só vai aumentar a velocidade se esse valor for inferior ao máximo.
                valor_pwm += 5;                   // O incremento deve ser um valor múltiplo para não ultrapassar ou decrementar mais do que é possível.
                analogWrite(PINO_PWM, valor_pwm); // "Escreve" a saída do PWM como igual a valor_pwm.
            }
            break;

        case 1: // Diminui a velocidade do motor.
            if (valor_pwm > 0)
            {                                     // Como 0 é o menor valor possível, o robô só vai diminuir a velocidade se esse valor for maior do que o mínimo.
                valor_pwm -= 5;                   // O incremento deve ser um valor múltiplo para não ultrapassar ou decrementar mais do que é possível.
                analogWrite(PINO_PWM, valor_pwm); // "Escreve" a saída do PWM como igual a valor_pwm.
            }
            break;

        default:
            break;
        }

        distancia = hc.dist(); // Função da biblioteca do sensor que retorna a distância.
        Serial.println(hc.dist());
        delay(100);

        if (distancia > 10)
        { // Se o sensor estiver acima de 10 de valor de distância, vai para o estado_motor de aceleração.
            estado_motor = 0;
        }
        else
        { // Caso contrário, ele começa a diminuir a velocidade.
            estado_motor = 1;
        }
    }
    else
    {
        Serial.println("Desligado");
    }
}