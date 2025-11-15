int led_naranga = 2; //Sensor 1
int led_azul = 3;    //Sensor 2
int led_blanco = 5;  //Sensor 3
int led_verde = 4;    //Sensor 4
int umbral_manual = A0; //Potenciometro

//Entrada de sensores
int s0 = A1;
int s1 = A2;
int s2 = A3;
int s3 = A4;

int valor_potenciomentro = 0;
int valor_s0 = 0;
int valor_s1 = 0;
int valor_s2 = 0;
int valor_s3 = 0;

int S0 = 0;
int S1 = 0;
int S2 = 0;
int S3 = 0;

int velocidad = 200;
int umbral = 100;

//motor 1
const int IN1 = 12;
const int IN2 = 11;

//motor 2
const int IN3 = 10;
const int IN4 = 9;


/*Funciones*/
/******************/
void adelante(){
  digitalWrite(led_naranga , LOW);
  digitalWrite(led_azul , LOW);
  digitalWrite(led_blanco , LOW);
  digitalWrite(led_verde , LOW);       
  
  analogWrite(IN1, 0);
  analogWrite(IN2, velocidad + 10);
  analogWrite(IN3, velocidad + 10);
  analogWrite(IN4, 0);
}

void derecha(){
  digitalWrite(led_azul,HIGH);
  digitalWrite(led_blanco, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_naranga , LOW);   

  analogWrite(IN1, velocidad / 2);
  analogWrite(IN2, 0);
  analogWrite(IN3, velocidad);
  analogWrite(IN4, 0);
  
}

void derecha_pronunciada(){
  digitalWrite(led_naranga , HIGH);
  digitalWrite(led_azul , HIGH);
  digitalWrite(led_blanco, LOW);
  digitalWrite(led_verde, LOW);
  
  // Motor izquierdo (A)
  analogWrite(IN1, 0);  // reversa
  analogWrite(IN2, velocidad);

  // Motor derecho (B)
  analogWrite(IN3, velocidad);
  analogWrite(IN4, 0);  // adelante
}

void izquierda(){
  digitalWrite(led_blanco , HIGH);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_naranga , LOW);
  digitalWrite(led_azul , LOW);     
    
  analogWrite(IN1, 0);
  analogWrite(IN2, velocidad / 2);
  analogWrite(IN3, 0);
  analogWrite(IN4, velocidad);
}

void izquierda_pronunciada(){
  digitalWrite(led_blanco, HIGH);
  digitalWrite(led_verde, HIGH);
  digitalWrite(led_naranga , LOW);
  digitalWrite(led_azul , LOW);   
      
  // Motor izquierdo (A)
  analogWrite(IN1, velocidad);  // reversa
  analogWrite(IN2, 0);

  // Motor derecho (B)
  analogWrite(IN3, 0);
  analogWrite(IN4, velocidad);  // adelante
}

void detener(){
  digitalWrite(led_naranga , HIGH);
  digitalWrite(led_azul , HIGH);
  digitalWrite(led_blanco , HIGH);
  digitalWrite(led_verde , HIGH);       
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}
/******************/

void setup() {
  Serial.begin(115200);
  pinMode(led_naranga, OUTPUT); 
  pinMode(led_azul, OUTPUT); 
  pinMode(led_blanco, OUTPUT); 
  pinMode(led_verde, OUTPUT); 
  pinMode(umbral_manual, INPUT);
 
}

void loop() {
  
  valor_potenciomentro = analogRead(umbral_manual);
  umbral=analogRead(umbral_manual);
  valor_s0  = analogRead(s0);
  valor_s1  = analogRead(s1);
  valor_s2  = analogRead(s2);
  valor_s3  = analogRead(s3);

  /*Umbral manual del potenciometro*/
  
  Serial.print("umbral :");
  Serial.println(valor_potenciomentro);

  S0 = (valor_s0 > umbral) ? 1 : 0;
  S1 = (valor_s1 > umbral) ? 1 : 0;
  S2 = (valor_s2 > umbral) ? 1 : 0;
  S3 = (valor_s3 > umbral) ? 1 : 0;

   /*Casos por si el carrito detecta ninguna o todos  los sensores en lineas negras*/
   if(!S0 && !S1 && !S2 && !S3) adelante();

   if(!S1 && S2) izquierda();
   
   if(S1 && !S2) derecha();

   if(!S0 && !S1 && S2 && S3) izquierda_pronunciada();   
 
   if(S0 && S1 && !S2 && !S3)derecha_pronunciada();
   
   if(S0 && S1 && S2 && S3) detener();


  
  //Serial.print("Sensor :");
  //Serial.print(valor_s0);

  //Serial.print("\t Sensor :");
  //Serial.print(valor_s1);

  //Serial.print("\t Sensor :");
  //Serial.print(valor_s2);

 //Serial.print("\t Sensor :");
  //Serial.println(valor_s3);
  delay(100);
}