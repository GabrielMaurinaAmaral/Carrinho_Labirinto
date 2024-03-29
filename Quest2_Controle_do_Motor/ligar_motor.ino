/**
  ******************************************************************************
  * @file    ligar_motor.ino
  * @authors Caroline, Gabriel Amaral, João Ferreira, Lucas Alves e Pedro.
  * @brief   Código para ligar e desligar o sistema caso o botão utilizado só fique no estado HIGH (ligado) enquanto está sendo pressionado.
  ******************************************************************************
  *
  **/

#define pino_botao 3

int estado = 0;
int guardar_estado = LOW;

void setup()
{
    pinMode(pino_botao, OUTPUT); 
    pinMode(led, OUTPUT);
}

void loop()
{
    estado = digitalRead(pino_botao); // Lê estado HIGH (ligado) ou LOW (desligado) do botão.
    
// Verifica se o botão foi pressionado:
    if (estado == HIGH)
    {
        guardar_estado = !guardar_estado; // Inverte o valor da variável.
        delay(5000);                       // OBS: evitar erro.
    }

    if (guardar_estado == HIGH) // Liga o sistema para começar tudo.
    {
        Serial.println("Ligado");
    }
    else
    {
        Serial.println("Desligado");
    }
}
