 /**
 * Medida de Temperatura usando LM35 e Termopar MTK-01
 * 
 * @authors Rafael, Camila, Pamella
 * @version 0.12 'Teste Leitura multiplos sensores'
 */

// Bibliotecas ---------------------------------------------------------------------------------
#include <math.h>

// Constantes ----------------------------------------------------------------------------------

// Debug
#define LED 13
#define TIME_BLINK 200

// Configuração Pinos
#define PIN_LM35      A0
#define PIN_MTK01     A1

#define INTERVAL      500  //ms
#define SIZE_BUFFER   10
#define SIZE_BUFFER_F 10   // Parâmentro Filtro Média Móvel
#define ALPHA         0.8  // Parâmentro Filtro Média Móvel Ponderada

// Variáveis Globais  --------------------------------------------------------------------------

// Timer
unsigned long lasttime = millis();

// Contadores
int k;                            // Buffer
int index;                        // Absoluto

// Buffer Dados Brutos
float bufferLM35[SIZE_BUFFER];
float bufferMTK01[SIZE_BUFFER];

// Buffer Dados Filtrados
float bufferAvgFilter[SIZE_BUFFER];
float bufferMvAvgFilter[SIZE_BUFFER];
float bufferMvAvgExpFilter[SIZE_BUFFER];

float pMvAvgExpFilter[4]; // Pesos Filtro Média Móvel Ponderada

// Setup ---------------------------------------------------------------------------------

void setup() {
  // Prepara Led para Debug
  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);
  
  // Inicia Porta Serial
  Serial.begin(57600);

  // Define Portas Sensores
  pinMode(PIN_LM35,INPUT);
  pinMode(PIN_MTK01,INPUT);

  // Zera Buffers
  for(k=0;k<SIZE_BUFFER;k++){
    bufferLM35[k] = 0;
    bufferMTK01[k] = 0;
    bufferAvgFilter[k] = 0;
    bufferMvAvgFilter[k] = 0;
    bufferMvAvgExpFilter[k] = 0;
  }

  blink(4);

  // Zera Contadores
  index=1;
  k=0;

  // Calcula Pesos Filtro Média Móvel Ponderada
  pMvAvgExpFilter[0] = pow(ALPHA,3);
  pMvAvgExpFilter[1] = (1-ALPHA)*pow(ALPHA,2);
  pMvAvgExpFilter[2] = (1-ALPHA)*pow(ALPHA,1);
  pMvAvgExpFilter[0] = (1-ALPHA);
}

// Loop ----------------------------------------------------------------------------------

void loop() {
  // Loop com período fixo de Amostragem
  if (millis() - lasttime >= INTERVAL) {
    // Ajusta Timer
    lasttime = millis();
    
    // Ler o valor dos sensores
    float rawTemp1 = analogRead(PIN_LM35);
    float rawTemp2 = analogRead(PIN_MTK01);

    // Filtragem
    bufferLM35[k] = rawTemp1;
    bufferMTK01[k] = rawTemp2;

    filterAverenge(bufferLM35,index,k);
    filterMovingAverenge(bufferLM35,k);
    filterMovingAverengeExp(bufferLM35,k);

    // Envia via Serial
    Serial.print(lasttime);
    Serial.print(' ');
    Serial.print(rawTemp1);
    Serial.print(' ');
    Serial.println(bufferMvAvgExpFilter[k]);

    // Incrementa Contadores
    index = index+1;
    k = (k<(SIZE_BUFFER-1))?k+1:0;
  }
}

// Funções Auxliares ---------------------------------------------------------------------

/**
 * Filtro Média Simples
 * 
 * @author Rafael Lima
 */
 void filterAverenge(float buffer[],int i,int k){
    // Calcula Endereços
    int a1 = (k>0)?k-1:SIZE_BUFFER-1;
  
    //bufferAvgFilter[k] = ((i-1)*bufferAvgFilter[a1])/i + buffer[i]/i;
    bufferAvgFilter[k] = bufferAvgFilter[a1] + (buffer[k] - bufferAvgFilter[a1])/i;
 }

/**
 * Filtro de Média Móvel com tamanho definido por SIZE_BUFFER_F
 * 
 * @author Rafael Lima
 */
void filterMovingAverenge(float buffer[],int k){
  // Calcula Endereços
  int a1 = (k>0)?k-1:SIZE_BUFFER_F-1;
  int a2 = (k<(SIZE_BUFFER_F-1))?k+1:0; // O próximo termo é igual o i-n por ser um buffer circular

  // Aplica Filtro
  bufferMvAvgFilter[k] = bufferMvAvgFilter[a1] + (buffer[k]-buffer[a2])/SIZE_BUFFER_F;
}

/**
 * Filtro de Média Móvel Ponderada Exponencialmente
 * 
 * @author Rafael Lima
 */
void filterMovingAverengeExp(float buffer[],int k){
  // Calcula Endereços
  int a1 = (k>0)?k-1:SIZE_BUFFER_F-1;
  int a2 = (k>1)?k-2:SIZE_BUFFER_F+k-2;
  int a3 = (k>2)?k-3:SIZE_BUFFER_F+k-3;

  // Aplica Filtro
  bufferMvAvgExpFilter[k] = buffer[a3]*pMvAvgExpFilter[0];
  bufferMvAvgExpFilter[k] += buffer[a2]*pMvAvgExpFilter[1];
  bufferMvAvgExpFilter[k] += buffer[a1]*pMvAvgExpFilter[2];
  bufferMvAvgExpFilter[k] += buffer[k]*pMvAvgExpFilter[3];
}

/**
 * Debug Function - Blink Led 'n' times
 * @author Rafael
 */
void blink(int n){
  for(k=(n>=0)?n:0;k<n;k++){
    digitalWrite(LED,HIGH);
    delay(TIME_BLINK);
    digitalWrite(LED,LOW);
    delay(TIME_BLINK);
  }
}
