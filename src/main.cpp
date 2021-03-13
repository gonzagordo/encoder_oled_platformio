#include <Arduino.h>
/* 
            menu con encoder y oled evolucionable a dimer 
*/

//  INCLUDES
#include<Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_BusIO_Register.h>
#include <configuracion.h>
#include <control_potencia.h>

//constructor del driver oled 
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//variables globales del proyecto
boolean giro_derechas =false;
boolean giro_izquierdas =false;
int posicion_encoder = 0;  // Posicion del encoder
int ant_posicion = 0;               // Valor anterior de la posicion del encoder
int duty =0;                        // contador de gobierno de potencia 
unsigned long contador = 0;         // cuenta los pasos por zero
unsigned long anterior = 0;         // historico del valor del contador
boolean giro_detectado = false;     //activado por la interrupcion del giro del encoder


//++++++++++++++++++++prototipos++++++++++++++++++++
void dimmer(int potencia);

void dimmer(int tabla, int potencia_seleccionada);

void encoder();

void pulso ();

void control_pantalla();

//++++++++++++++++++++++++++++++++++++++++++++++++++


void setup() {
  
  pinMode(DT, INPUT);   
  pinMode(CLK, INPUT);    
  pinMode(btn, INPUT_PULLUP);    // Entrada con resistencia Pullup para el Switch del encoder
  pinMode(LED, OUTPUT); 

  //inicializa oled 
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  oled.display();
  delay(1000);
  oled.setTextColor(SSD1306_WHITE);
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setCursor(0,0);
  oled.print("ZEROOO_4");
  oled.display();

  // vincula la interrupcion del giro delencoder  
  attachInterrupt(digitalPinToInterrupt(DT), encoder, FALLING);  // Se habilita interrupcion en pin 3 modo LOW
  
  //vincula la interuupcion al boton/sensor de cero
  attachInterrupt(digitalPinToInterrupt(btn), pulso, FALLING);  // Se habilita interrupcion en pin 2 modo FALLING
 
 
  Serial.begin(9600);   // incializa puerto serie a 9600 bps
  Serial.println("zero cross detector_v2");  // Mensaje inicial en Puerto Serie
  delay(1000);
  oled.clearDisplay();
  oled.setTextSize(4);
  oled.setCursor(40,16);
  oled.print("0");
  oled.display();
   
}


 
void loop() {

  dimmer(4);

  if (contador > anterior){    //debug de rerepresentacion de potencia conteo de pulsos  
    anterior= contador;
    Serial.write(35);
    if (contador % 100 == 0){
        Serial.print(contador/100);
        Serial.print("-");
        Serial.println(duty);
    }
  } 

  control_pantalla();
  


}//final+++++++++++++++++++

//+++++++++++++++++++subrutinas+++++++++++++++++++++

void dimmer(int potencia){    //modula la potencia de salida por apagado y encendido de pulsos de red

  static boolean cambio = true;
  if (duty ==0) {
    if (cambio){
      cambio=false;
      Serial.print("^");  // aki se encenderia el pwm es solo debug
      duty=1;
    } 

    else{
      cambio=true;
      Serial.print("v");  // aki se encenderia el pwm es solo debug
      duty=potencia;
    }     
  }
}

void dimmer(int tabla, int potencia_seleccionada){

  
}

void encoder()  {             //Funcion que determina el sentido del giro del encoder
  
  static unsigned long ultimaInterrupcion = 0;  // variable static con ultimo valor de tiempo de interrupcion
  unsigned long tiempoInterrupcion = millis();  // variable almacena valor de func. millis

  if (tiempoInterrupcion - ultimaInterrupcion > 100) {  // No lee posibles rebotes en pulsos menores a 5 mseg.
    if (digitalRead(CLK) == HIGH)     // si CLK es HIGH, rotacion a la derecha
    {
      posicion_encoder++ ;        // incrementa posicion del menu en 1
      giro_derechas = true;
    }
    else {          // si CLK es LOW, rotacion a la izquierda
      posicion_encoder-- ;        // decrementa posicion del menu en 1
      giro_izquierdas = true;
    }

      ultimaInterrupcion = tiempoInterrupcion;  //actualiza valor del tiempo tiempo
  }
}

void pulso ()   {             //Funcion que incrementa pulsos con lecturas de la red
  contador++;
  duty = max(0,duty--);            //decrementa el valor de duty impidiendo valores negativos
}

void control_pantalla () {    //funcion que muestra valores e interaccion con el encoder 
  
  if (posicion_encoder!=ant_posicion){

    Serial.print("posicion encoder ");
    Serial.println(posicion_encoder);
    ant_posicion=posicion_encoder;
    oled.clearDisplay();
    oled.setCursor(40,16);
    oled.print(String(posicion_encoder));
    oled.display();  
  }

}

//+++++++++++++++++++subrutinas+++++++++++++++++++++
