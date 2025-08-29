bool state = LOW;
float dutyCycle = 20.0;
int OutValue = 0;  
int Suma=0;

const int iny1=5;  //salidas inyectores
const int iny2=6;
const int iny3=7;
const int iny4=8;

int salida15s= 0;
bool salidaon=false;
int inyecactivo=0;

volatile unsigned int cuenta_ = 0;
volatile unsigned int cuenta = 0;
volatile unsigned int cuenta2 = 0;    
volatile unsigned int cuenta3 = 0;   
volatile unsigned int cuenta4 = 0; 

float frecuencia=1200;
float frecuencia2=1200;   //Contador para establecer la frec al 20%
float duty=20;            //Contador para establecer el ciclo de trabajo al 20%
float porcentajeDuty=20;
int aumentoPwm=0;

const int buttonPin_Modo = 2; // Pin del botón modo 
const int buttonPin_frec1 = 4; // Pin del botón frec+
const int buttonPin_pwm1 = 3; // Pin del botón frec+

const int ledPin1 = 9;  // Pin del LED para el modo 1
const int ledPin2 = 17;  // Pin del LED para el modo 2
const int ledPin3 = 16;  // Pin del LED para el modo 3
volatile int modo = 1;   // Variable para almacenar el modo actual
const int numModes = 4;  // Número de modos disponibles

void setup() {
  
pinMode(iny1,OUTPUT);   //configuracion de pines de inyectores como salidas
pinMode(iny2,OUTPUT);
pinMode(iny3,OUTPUT);
pinMode(iny4,OUTPUT);

pinMode(buttonPin_Modo, INPUT_PULLUP); // Configura el pin del botón como entrada con resistencia pull-up
pinMode(buttonPin_frec1,INPUT_PULLUP);
pinMode(buttonPin_pwm1, INPUT_PULLUP);


pinMode(ledPin1, OUTPUT);             // Configura el pines del LED RGB como salida
pinMode(ledPin2, OUTPUT);              
pinMode(ledPin3, OUTPUT);

SREG = (SREG & 0b01111111); //Desabilitar interrupciones
TCCR2A = (1<<WGM21)|(0<<WGM20); // Mode CTC comparador
TIMSK2 = (1<<OCIE2A); // Local interruption OCIE2A
OCR2A =50;       // COMPARADOR A    50
SREG = (SREG & 0b01111111) | 0b10000000; // Habilitar interrupciones 

// Desactivar las salidas de los inyectores
digitalWrite(iny1,HIGH);               // Desactivar las salidas de los inyectores
digitalWrite(iny2,HIGH);
digitalWrite(iny3,HIGH);
digitalWrite(iny4,HIGH);
salidaon=true;

attachInterrupt(0, pin2IRQ, FALLING); //
}

void loop() {                 //ciclo loop
  updateMenu();
  
  switch (modo) {
       case 1:
        modo1();               //función modo 1
        break;
       case 2:
        modo2();               //función modo 2
        break;
       case 3:
        modo3();               //funcion modo 3
        break;
       case 4:
        modo4();              //funcion modo 4
        break;
       case 5:
        modo5();              //funcion modo 5
        break;        
  }
}

void modo1(){ 
 apagainyectores();
 // apagainyectores();
  digitalWrite(ledPin1, HIGH);               // PROBADOR  
  delay(400);
  digitalWrite(ledPin1, LOW);               // PROBADOR
  delay(400);
}

void modo2(){  
  //apagainyectores();
  ajustaFrecuencia();
  ajustaPWM();
  
  if(cuenta<duty){
      digitalWrite(iny1,LOW);
  }
  else if(cuenta>duty){
      digitalWrite(iny1,HIGH);
  }    
     
  if(cuenta2<(duty+(frecuencia/8))){
      digitalWrite(iny2,LOW);
  }
  else if(cuenta2>(duty+(frecuencia/8))){
      digitalWrite(iny2,HIGH);
  }    

  if(cuenta3<(duty+(frecuencia/8))){
      digitalWrite(iny3,LOW);
  }
  else if(cuenta3>(duty+(frecuencia/8))){
      digitalWrite(iny3,HIGH);
  }
    
  if(cuenta4<(duty+(frecuencia/8))){
     digitalWrite(iny4,LOW);
  }
  else if(cuenta4>(duty+(frecuencia/8))){
     digitalWrite(iny4,HIGH);
  }  
}

void modo3(){  //balance de inyectores 1seg
  // Frec selecciona el inyector
  // pwm activa el inyector por X segundos
  apagainyectores();
// delay(20);

  while (inyecactivo <= salida15s) {
    digitalWrite(ledPin3, LOW);
    delay(100);
    apagaLEDs(); 
    delay(300);
    inyecactivo=inyecactivo+1;

    if(salidaon==false){                 
      digitalWrite(ledPin3, LOW);
      digitalWrite(iny1+salida15s,LOW);   
      delay(1000);           // aproximadamente 1 segundos
      digitalWrite(iny4,HIGH);
      salidaon=true;
      apagaLEDs(); 
      delay(75);
    }

    static unsigned long ultimaInterrupcion4 = 0;
    unsigned long tiempoInterrupcion4 = millis();
    if(digitalRead(buttonPin_frec1) == 0 && (tiempoInterrupcion4 - ultimaInterrupcion4) > 50){  
  //    salidaon=false;
      salida15s=salida15s+1;
      if(salida15s>=4){
        salida15s=0;
      }
      ultimaInterrupcion4 = tiempoInterrupcion4;
    //delay(150);
    }  
     
    static unsigned long ultimaInterrupcion5 = 0;
    unsigned long tiempoInterrupcion5 = millis();
    if(digitalRead(buttonPin_pwm1) == 0 && (tiempoInterrupcion5 - ultimaInterrupcion5) > 150){ 
      salidaon=false;
      ultimaInterrupcion5 = tiempoInterrupcion5;
    //delay(150);
    }

 }
    apagaLEDs(); 
    inyecactivo=0;
    delay(350);
}


void modo4(){  //balance de inyectores 
  // Frec selecciona el inyector   // pwm activa el inyector por X segundos
  apagainyectores();
  // delay(20);

  while (inyecactivo <= salida15s) {
      analogWrite(ledPin1,151);
    analogWrite(ledPin2,69);
    analogWrite(ledPin3,242);
 
 // digitalWrite(ledPin2, LOW);
  //digitalWrite(ledPin3, LOW);
  delay(100);
  apagaLEDs(); 
  delay(300);
  inyecactivo=inyecactivo+1;

 if(salidaon==false){     
   // digitalWrite(ledPin2, LOW);            
    //digitalWrite(ledPin3, LOW);
    analogWrite(ledPin1,151);
    analogWrite(ledPin2,69);
    analogWrite(ledPin3,242);
    digitalWrite(iny1+salida15s,LOW);   
    delay(10000);           // aproximadamente 10 segundos
//    digitalWrite(iny4,HIGH);
    salidaon=true;
    apagaLEDs(); 
    delay(75);
  }

  static unsigned long ultimaInterrupcion6 = 0;
  unsigned long tiempoInterrupcion6 = millis();
 if(digitalRead(buttonPin_frec1) == 0 && (tiempoInterrupcion6 - ultimaInterrupcion6) > 50){  
//    salidaon=false;
    salida15s=salida15s+1;
    if(salida15s>=4){
      salida15s=0;
    }
    ultimaInterrupcion6 = tiempoInterrupcion6;
    //delay(150);
  }  
     
  static unsigned long ultimaInterrupcion7 = 0;
  unsigned long tiempoInterrupcion7 = millis();
  if(digitalRead(buttonPin_pwm1) == 0 && (tiempoInterrupcion7 - ultimaInterrupcion7) > 50){ 
    salidaon=false;
    ultimaInterrupcion7 = tiempoInterrupcion7;
    //delay(150);
    }

      }
 apagaLEDs(); 
 inyecactivo=0;
     delay(350);
}


void modo5(){

    for (dutyCycle = 20.0; dutyCycle < 100.0; dutyCycle++) {  //ciclo para el aumento de pwm

      analogWrite(iny1,dutyCycle*255/100);                    // Escritura de 
      delay(15+aumentoPwm);

      if(digitalRead(buttonPin_Modo) == 0){
        dutyCycle=100;        
      }
      
     static unsigned long ultimaInterrupcion8 = 0;
     unsigned long tiempoInterrupcion8 = millis();
      if(digitalRead(buttonPin_frec1) == 0&& (tiempoInterrupcion8 - ultimaInterrupcion8)>50){
        
        dutyCycle=100; 
        aumentoPwm=aumentoPwm+5;    
        if(aumentoPwm>=35){
          apagaLEDs();
          aumentoPwm=0; 
                 digitalWrite(ledPin1, LOW);
       delay(200);
       digitalWrite(ledPin1, HIGH);
       delay(100);   
       updateMenu();
       
        }
        ultimaInterrupcion8 = tiempoInterrupcion8;
      }
      
    }

    for ( dutyCycle ; dutyCycle>20; dutyCycle--) { 

        analogWrite(iny1,dutyCycle*255/100 );
        delay(15+aumentoPwm);
     
      if(digitalRead(buttonPin_Modo) == 0){
        dutyCycle=20;        
      }

     static unsigned long ultimaInterrupcion9 = 0;
     unsigned long tiempoInterrupcion9 = millis();
      if(digitalRead(buttonPin_frec1) == 0&& (tiempoInterrupcion9 - ultimaInterrupcion9)>50){
        
        dutyCycle=100; 
        aumentoPwm=aumentoPwm+5;    
        if(aumentoPwm>35){
          apagaLEDs();
          aumentoPwm=0; 
                 digitalWrite(ledPin1, LOW);
       delay(200);
       digitalWrite(ledPin1, HIGH);
       delay(100);   
       updateMenu();
       
        }
       ultimaInterrupcion9 = tiempoInterrupcion9; 
      }
      

    }
     
}

void changeMode() {
  modo = modo + 1;                  // Cambia el modo y asegura que esté dentro del rango de modos
  if(modo>=6){
     modo=1;
  }
}

void updateMenu() {                 // Apaga todos los LEDs
  apagaLEDs();
  
  if (modo == 1) {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
  }
  if (modo == 2) {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, HIGH);
  }
  
  if (modo == 3) {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, LOW);
  }
   if (modo == 4) {
    analogWrite(ledPin1,94);
    analogWrite(ledPin2,186);
    analogWrite(ledPin3,13);
  }

     if (modo == 5) {
    analogWrite(ledPin1,  255);
    analogWrite(ledPin2,50);
    analogWrite(ledPin3,100);
  }
   
}

void ajustaFrecuencia(){
   static unsigned long ultimaInterrupcion2 = 0;
  unsigned long tiempoInterrupcion2 = millis();
 
 if(digitalRead(buttonPin_frec1) == 0 && tiempoInterrupcion2 - ultimaInterrupcion2 > 350){ 
    frecuencia=frecuencia-300;
    if(frecuencia<=0){ 
       apagaLEDs(); 
       frecuencia=1200;
       digitalWrite(ledPin1, LOW);
       delay(100);
       digitalWrite(ledPin1, HIGH);
       delay(50);
        updateMenu();
    }
 ultimaInterrupcion2 = tiempoInterrupcion2;
   
 }
 
 frecuencia2=frecuencia+(frecuencia/8); 
}


void ajustaPWM(){                               //ajuste de ancho de pulso 
  static unsigned long ultimaInterrupcion3 = 0;
  unsigned long tiempoInterrupcion3 = millis();
  if(digitalRead(buttonPin_pwm1) == 0&& (tiempoInterrupcion3 - ultimaInterrupcion3) > 350){ 
      porcentajeDuty=porcentajeDuty+5;
      if(porcentajeDuty>=30){
        apagaLEDs(); 
        porcentajeDuty=10;
        digitalWrite(ledPin1, LOW);
        delay(100);
        digitalWrite(ledPin1, HIGH);
        delay(50);
        updateMenu();
      }
      ultimaInterrupcion3 = tiempoInterrupcion3;
  } 
  duty=(porcentajeDuty/100)*frecuencia;
}

void apagaLEDs() {
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);
}

void apagainyectores() {
  digitalWrite(iny1,HIGH);                       //establecer todas las salidas en bajo o desactivadas
  digitalWrite(iny2,HIGH);
  digitalWrite(iny3,HIGH);
  digitalWrite(iny4,HIGH);
}

void pin2IRQ() {             // INTERRUPCION MODO
  static unsigned long ultimaInterrupcion = 0;
  unsigned long tiempoInterrupcion = millis();
  if (tiempoInterrupcion - ultimaInterrupcion > 350) {

 changeMode();
  }
  ultimaInterrupcion = tiempoInterrupcion;
}

ISR(TIMER2_COMPA_vect){              //interrupcion de tipo TIMER
     
     cuenta++; 
     cuenta2++;
     cuenta3++;
     cuenta4++;
        
     if(cuenta > frecuencia){
        cuenta=0;
     }      
     if(cuenta2> frecuencia2){ 
        cuenta2=cuenta-1*(frecuencia/8);
     }
     if(cuenta3> frecuencia2){
        cuenta3=cuenta-3*(frecuencia/8);
     }             
     if(cuenta4> frecuencia2){ 
        cuenta4=cuenta-5*(frecuencia/8);
     } 
     
}

ISR(TIMER1_COMPA_vect){ 
cuenta_++; 

     if(cuenta_ > frecuencia){
        cuenta_=0;
     }  
}
