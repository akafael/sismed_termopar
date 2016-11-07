/**
 * Medida de Temperatura usando LM35 e Termopar MTK-01
 * 
 * @authors Rafael, Camila, Pamella
 * @version 0.12 'Teste Leitura multiplos sensores'
 */


#define PIN_LM35      A0
#define PIN_MTK01     A1
#define INTERVAL      500  //ms

unsigned long lasttime = millis();

void setup() {
  // Inicia Porta Serial
  Serial.begin(9600);

  // Define Portas Sensores
  pinMode(PIN_LM35,INPUT);
  pinMode(PIN_MTK01,INPUT);
}

void loop() {
  // Loop com período fixo de Amostragem
  if (millis() - lasttime >= INTERVAL) {
    // Ajusta Timer
    lasttime = millis();
    
    // Ler o valor dos sensores
    int rawTemp1 = analogRead(PIN_LM35);
    int rawTemp2 = analogRead(PIN_MTK01);

    // Envia via Serial
    Serial.print(lasttime);
    Serial.print(' ');
    Serial.print(rawTemp1);
    Serial.print(' ');
    Serial.println(rawTemp2);
  }
}
