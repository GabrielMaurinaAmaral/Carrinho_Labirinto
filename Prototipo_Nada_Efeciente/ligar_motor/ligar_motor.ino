#define pino_botao 3

int estado = 0;
int guardar_estado = LOW;

void setup()
{
    pinMode(led, OUTPUT);
}

void loop()
{
    estado = digitalRead(pino_botao); // Lê estado HIGH(ligado) ou LOW(desligado) do botão.
    
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
