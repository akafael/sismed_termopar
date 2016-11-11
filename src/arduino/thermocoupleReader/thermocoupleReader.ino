/**
 * Medida de Temperatura usando LM35 e Termopar MTK-01
 * 
 * @authors Rafael, Camila, Pamella
 * @version 0.9 'Filtros Implementados'
 */

// Bibliotecas ---------------------------------------------------------------------------------
#include <math.h>

// Constantes ----------------------------------------------------------------------------------

// Configuração Pinos
#define PIN_LM35      A0
#define PIN_MTK01     A1

#define GAIN_LM35      6
#define GAIN_MTK01     101

#define A_LM35      100*(5/1024)
#define A_MTK01     410*(5/1024)

#define INTERVAL      200  //ms
#define SIZE_BUFFER   10
#define SIZE_BUFFER_F 10   // Parâmentro Filtro Média Móvel
#define ALPHA         0.8  // Parâmentro Filtro Média Móvel Ponderada

// Parâmetros Filtro de Kalman
#define KALMAN_A        1
#define KALMAN_B        0
#define KALMAN_CONTROL  0
#define KALMAN_R        0.5
#define KALMAN_C        1
#define KALMAN_Q        10
#define KALMAN_COV      10000

// Variáveis Globais  --------------------------------------------------------------------------

// Timer
unsigned long lasttime = millis();

// Contadores
int k;                            // Buffer
int index;                        // Absoluto

// Buffer Dados Brutos
float bufferLM35[SIZE_BUFFER];
float bufferMTK01[SIZE_BUFFER];

float bufferTemp[SIZE_BUFFER];

// Buffer Dados Filtrados
float bufferAvgFilter[SIZE_BUFFER];
float bufferMvAvgFilter[SIZE_BUFFER];
float bufferMvAvgExpFilter[SIZE_BUFFER];
float bufferKalmanFilter[SIZE_BUFFER];

float pMvAvgExpFilter[4]; // Pesos Filtro Média Móvel Ponderada

float kalman_cov = 10000;  // Parâmetro Filtro de Kalman

// Setup ---------------------------------------------------------------------------------

void setup() {  
  // Inicia Porta Serial
  Serial.begin(57600);

  // Define Portas Sensores
  pinMode(PIN_LM35,INPUT);
  pinMode(PIN_MTK01,INPUT);

  // Inicia Filtros e buffers
  for(k=0;k<SIZE_BUFFER;k++){
    bufferLM35[k] = analogRead(PIN_LM35);
    bufferMTK01[k] = analogRead(PIN_MTK01);
    bufferAvgFilter[k] = 0;
    bufferMvAvgFilter[k] = bufferLM35[k];
    bufferMvAvgExpFilter[k] = bufferLM35[k];
  }

  // Zera Contadores
  index=1;
  k=0;

  // Calcula Pesos Filtro Média Móvel Ponderada
  pMvAvgExpFilter[0] = pow(ALPHA,3);
  pMvAvgExpFilter[1] = (1-ALPHA)*pow(ALPHA,2);
  pMvAvgExpFilter[2] = (1-ALPHA)*pow(ALPHA,1);
  pMvAvgExpFilter[3] = (1-ALPHA);
}

// Loop ----------------------------------------------------------------------------------

void loop() {
  // Loop com período fixo de Amostragem
  if (millis() - lasttime >= INTERVAL) {
    // Ajusta Timer
    lasttime = millis();
    
    // Ler o valor dos sensores
    bufferLM35[k] = analogRead(PIN_LM35);
    bufferMTK01[k] = analogRead(PIN_MTK01);

    // Calcula a Temperatura
    float temp1 = bufferLM35[k]*A_LM35;   // Temperatura de Referência ( Ambiente )
    float temp2 = bufferMTK01[k]*A_MTK01; // Medida da Variação da Temperatura
    bufferTemp[k] =  temp1 + temp2;       // Compensação por Software

    // Filtragem
    filterAverenge(bufferLM35,index,k);
    filterMovingAverenge(bufferLM35,k);
    filterMovingAverengeExp(bufferLM35,k);
    filterKalman(bufferLM35,k);

    // Envia via Serial
    Serial.print(lasttime);
    Serial.print(' ');
    Serial.print(bufferLM35[k]);
    Serial.print(' ');
    Serial.print(bufferMTK01[k]);
    Serial.print(' ');
    Serial.print(bufferAvgFilter[k]);
    Serial.print(' ');
    Serial.print(bufferMvAvgFilter[k]);
    Serial.print(' ');
    Serial.print(bufferMvAvgExpFilter[k]);
    Serial.print(' ');
    Serial.println(bufferKalmanFilter[k]);

    // Incrementa Contadores
    index = index+1;
    k = (k<(SIZE_BUFFER-1))?k+1:0;
  }
}

// Funções Auxiliares ---------------------------------------------------------------------

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
  bufferMvAvgExpFilter[k] = bufferMvAvgExpFilter[a3]*pMvAvgExpFilter[0];
  bufferMvAvgExpFilter[k] += buffer[a2]*pMvAvgExpFilter[1];
  bufferMvAvgExpFilter[k] += buffer[a1]*pMvAvgExpFilter[2];
  bufferMvAvgExpFilter[k] += buffer[k]*pMvAvgExpFilter[3];
}

/**
 * Filtro de Kalman
 * 
 * @author Rafael Lima
 */
 void filterKalman(float buffer[],int k){
    // Calcula Endereços
    int a1 = (k>0)?k-1:SIZE_BUFFER_F-1;
  
    float m = KALMAN_A*bufferKalmanFilter[a1] + KALMAN_B*KALMAN_CONTROL;
    float S = KALMAN_A*kalman_cov*KALMAN_A + KALMAN_R;
    float K = S*KALMAN_C/(KALMAN_C*S*KALMAN_C + KALMAN_Q);
    
    bufferKalmanFilter[k] = m + K*(buffer[k] - KALMAN_C*m);
    kalman_cov = (1 - K*KALMAN_C)*S;
 }
