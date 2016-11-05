/**
 * Medida de Temperatura usando LM35 e Termopar MTK-01
 * 
 * @authors Rafael, Camila, Pamella
 * @version 0.1 'Teste Leitura'
 */


#define PIN_LM35 A0

void setup() {
  // Inicia Porta Serial
  Serial.begin(9600);
}

void loop() {
  int temp1 = analogRead(PIN_LM35);
  Serial.println(temp1);

  delay(100);
}`
