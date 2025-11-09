// === PINES ===
const int boton = 21;
const int led_amarillo = 23;
const int led_verde = 22;

// Pines de sensores
const int s0 = 36;
const int s1 = 35;
const int s2 = 32;
const int s3 = 33;

// === VARIABLES DE UMBRALES ===
int umbral_negro_s0, umbral_negro_s1, umbral_negro_s2, umbral_negro_s3; 
int umbral_blanco_s0, umbral_blanco_s1, umbral_blanco_s2, umbral_blanco_s3; 
int umbral_calibrado_s0, umbral_calibrado_s1, umbral_calibrado_s2, umbral_calibrado_s3; 

int umbral_minimo_s0 = 1000;
int umbral_minimo_s1 = 1000;
int umbral_minimo_s2 = 1000;
int umbral_minimo_s3 = 1000;

int umbral_maximo_s0 = 0;
int umbral_maximo_s1 = 0;
int umbral_maximo_s2 = 0;
int umbral_maximo_s3 = 0;

// === Animacion ===
void animacion_confirmacion() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(led_verde, LOW);
    digitalWrite(led_amarillo, LOW);
    delay(500);
    digitalWrite(led_verde, HIGH);
    digitalWrite(led_amarillo, HIGH);
    delay(500);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_amarillo, LOW);
  }
}

void esperando_confirmacion_lectura() {
  digitalWrite(led_amarillo, LOW);
  digitalWrite(led_verde, HIGH);
  
  //Esperando el boton.
  while (digitalRead(boton) == HIGH) {}  
  
  digitalWrite(led_amarillo, HIGH);
  digitalWrite(led_verde, LOW);
}

// === LECTURA PROMEDIO DE SENSORES ===
void lectura_sensores(int muestras = 500, 
                      int *umbral_s0 = nullptr, 
                      int *umbral_s1 = nullptr, 
                      int *umbral_s2 = nullptr, 
                      int *umbral_s3 = nullptr) {
  
  int min_s0 = 4095, max_s0 = 0;
  int min_s1 = 4095, max_s1 = 0;
  int min_s2 = 4095, max_s2 = 0;
  int min_s3 = 4095, max_s3 = 0;

  for (int i = 0; i < muestras; i++) {
    int val_s0 = analogRead(s0);
    int val_s1 = analogRead(s1);
    int val_s2 = analogRead(s2);
    int val_s3 = analogRead(s3);

    if (val_s0 < min_s0) min_s0 = val_s0;
    if (val_s0 > max_s0) max_s0 = val_s0;
    if (val_s1 < min_s1) min_s1 = val_s1;
    if (val_s1 > max_s1) max_s1 = val_s1;
    if (val_s2 < min_s2) min_s2 = val_s2;
    if (val_s2 > max_s2) max_s2 = val_s2;
    if (val_s3 < min_s3) min_s3 = val_s3;
    if (val_s3 > max_s3) max_s3 = val_s3;

    delay(5); // Estabilidad
  }

  if (umbral_s0) *umbral_s0 = (min_s0 + max_s0) / 2;
  if (umbral_s1) *umbral_s1 = (min_s1 + max_s1) / 2;
  if (umbral_s2) *umbral_s2 = (min_s2 + max_s2) / 2;
  if (umbral_s3) *umbral_s3 = (min_s3 + max_s3) / 2;
}

// === CALIBRACION ===
void umbrar_negro() {
  lectura_sensores(500, &umbral_negro_s0, &umbral_negro_s1, &umbral_negro_s2, &umbral_negro_s3);
}

void umbrar_blanco() {
  lectura_sensores(500, &umbral_blanco_s0, &umbral_blanco_s1, &umbral_blanco_s2, &umbral_blanco_s3);
}

void calcular_umbral_calibrado() {
  umbral_calibrado_s0 = (umbral_blanco_s0 + umbral_negro_s0) / 2; 
  umbral_calibrado_s1 = (umbral_blanco_s1 + umbral_negro_s1) / 2; 
  umbral_calibrado_s2 = (umbral_blanco_s2 + umbral_negro_s2) / 2; 
  umbral_calibrado_s3 = (umbral_blanco_s3 + umbral_negro_s3) / 2;
}

// Calibracion general con promedio
void calibracion_umbrales(int num_lecturas = 5) {
  for (int i = 0; i < num_lecturas; i++) {
    // Negro
    esperando_confirmacion_lectura();
    Serial.println("Lectura de la linea");
    umbrar_negro();
    
    delay(1000);
    animacion_confirmacion();
    
    // Blanco
    esperando_confirmacion_lectura();
    Serial.println("Lectura del blanco");
    umbrar_blanco();
    delay(1000);
    animacion_confirmacion();
    Serial.println("Calculando umbral");
    // Calcula umbral
    calcular_umbral_calibrado();
    
    // Actualiza maximos y minimos
    if (umbral_calibrado_s0 > umbral_maximo_s0) umbral_maximo_s0 = umbral_calibrado_s0;
    if (umbral_calibrado_s0 < umbral_minimo_s0) umbral_minimo_s0 = umbral_calibrado_s0;

    if (umbral_calibrado_s1 > umbral_maximo_s1) umbral_maximo_s1 = umbral_calibrado_s1;
    if (umbral_calibrado_s1 < umbral_minimo_s1) umbral_minimo_s1 = umbral_calibrado_s1;

    if (umbral_calibrado_s2 > umbral_maximo_s2) umbral_maximo_s2 = umbral_calibrado_s2;
    if (umbral_calibrado_s2 < umbral_minimo_s2) umbral_minimo_s2 = umbral_calibrado_s2;

    if (umbral_calibrado_s3 > umbral_maximo_s3) umbral_maximo_s3 = umbral_calibrado_s3;
    if (umbral_calibrado_s3 < umbral_minimo_s3) umbral_minimo_s3 = umbral_calibrado_s3;
  }

  // Calcula los umbrales finales promedio
  umbral_calibrado_s0 = (umbral_minimo_s0 + umbral_maximo_s0) / 2;
  umbral_calibrado_s1 = (umbral_minimo_s1 + umbral_maximo_s1) / 2;
  umbral_calibrado_s2 = (umbral_minimo_s2 + umbral_maximo_s2) / 2;
  umbral_calibrado_s3 = (umbral_minimo_s3 + umbral_maximo_s3) / 2;

  animacion_confirmacion();
  Serial.println("✅ Calibración completada");
  Serial.println("--------------------------------------");
  Serial.printf("S0: %d | S1: %d | S2: %d | S3: %d\n", umbral_calibrado_s0, umbral_calibrado_s1, umbral_calibrado_s2, umbral_calibrado_s3);
  Serial.println("--------------------------------------");
}

// === SETUP ===
void setup() {
  Serial.begin(115200);
  pinMode(boton, INPUT_PULLUP);
  pinMode(led_amarillo, OUTPUT);
  pinMode(led_verde, OUTPUT);

  Serial.println("Iniciando calibración...");
  calibracion_umbrales(5);
}

// === LOOP PRINCIPAL ===
void loop() {
  int val_s0 = analogRead(s0);
  int val_s1 = analogRead(s1);
  int val_s2 = analogRead(s2);
  int val_s3 = analogRead(s3);

  Serial.print("S0: "); Serial.print(val_s0);
  Serial.print(" | S1: "); Serial.print(val_s1);
  Serial.print(" | S2: "); Serial.print(val_s2);
  Serial.print(" | S3: "); Serial.println(val_s3);

  delay(300);
}
