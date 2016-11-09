 /**
 * Medida de Temperatura usando LM35 e Termopar MTK-01
 * 
 * @authors Rafael, Camila, Pamella
 * @version 0.12 'Teste Leitura multiplos sensores'
 */


#define PIN_LM35      A0
#define PIN_MTK01     A1
#define INTERVAL      500  //ms
#define SIZE_BUFFER   10

unsigned long lasttime = millis();

int bufferLM35[SIZE_BUFFER];
int bufferMTK01[SIZE_BUFFER];
int k;
int i;                        // Contador Leituras

void setup() {
  // Inicia Porta Serial
  Serial.begin(57600);

  // Define Portas Sensores
  pinMode(PIN_LM35,INPUT);
  pinMode(PIN_MTK01,INPUT);

  // Zera Buffer
  for(k=0;k<SIZE_BUFFER;k++){
    bufferLM35[k] = 0;
    bufferMTK01[k] = 0;
  }

  // Zera Contador
  i=0;
}

void loop() {
  // Loop com período fixo de Amostragem
  if (millis() - lasttime >= INTERVAL) {
    // Ajusta Timer
    lasttime = millis();
    
    // Ler o valor dos sensores
    int rawTemp1 = analogRead(PIN_LM35);
    int rawTemp2 = analogRead(PIN_MTK01);

    // Filtragem
    bufferLM35[k] = rawTemp1;
    bufferMTK01[k] = rawTemp2;

    int filteredData = filterMovingAverenge(bufferLM35,k);

    // Envia via Serial
    Serial.print(k);
    Serial.print(' ');
    Serial.print(rawTemp1);
    Serial.print(' ');
    Serial.println(filteredData);

    // Incrementa Contadores
    i = i+1;
    k = (k<(SIZE_BUFFER-1))?k+1:0;
  }
}

/**
 * Filtro de Média Móvel com tamanho definido por SIZE_BUFFER
 * 
 * @author Rafael Lima
 */
int filterMovingAverenge(int buffer[],int i){
  // Calcula Endereços
  int a1 = (i>0)?i-1:SIZE_BUFFER-1;
  int a2 = (i<(SIZE_BUFFER-1))?i+1:0; // O próximo termo é igual o i-n por ser um buffer circular

  // Aplica Filtro
  int filteredData = buffer[a1] + (buffer[i]-buffer[a2])/SIZE_BUFFER;
  
  return filteredData;
}

