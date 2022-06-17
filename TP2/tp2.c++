#include <LiquidCrystal.h>
#include <Keypad.h>
#include <ctype.h>
#include <string.h>

//============ DEFINES ========================================

#define LED_ROJO 10
#define LED_VERDE 9
#define SUCCESS 1
#define FAIL 0
#define ZERO_TO_HERO 0

#define NO_BUTTON 0
#define BUTTON_RESET 1
#define BUTTON_TEST 2
#define BUTTON_CONFIG 3

//================ LCD ===================================

LiquidCrystal lcd(8,13,A0,A1,A2,A3);

//================  KEYPAD ===============================

const byte FILAS = 4;
const byte COLUMNAS = 4;
 
char keys[FILAS][COLUMNAS] = {
   { '1','2','3', 'A' },
   { '4','5','6', 'B' },
   { '7','8','9', 'C' },
   { '*','0','#', 'D' }
};

byte pinesFilas[FILAS] = {7, 6, 5, 4};
byte pinesColumnas[COLUMNAS] = {3, 2, 1, 0};

Keypad teclado = Keypad(makeKeymap(keys),pinesFilas,pinesColumnas, FILAS, COLUMNAS);

char tecla;
char password[8];
char default_password[8] ="1C2022";
byte indice = ZERO_TO_HERO;

//=================  LED ===============================

int estadoLed = LOW;
unsigned long tiempoEncendido = ZERO_TO_HERO;
unsigned long tiempoApagado = ZERO_TO_HERO;

//=================  TIMER ==============================

unsigned long tiempo1 = ZERO_TO_HERO;
unsigned long tiempo2 = ZERO_TO_HERO;
unsigned long tiempoSegundos = ZERO_TO_HERO;
int contFive = ZERO_TO_HERO;

//=============== BANDERAS ==============================

int flag = 1;
int flagReset = 0;
int flagTest = 0;
int flagConfig = 0;
int flagPassOk = 0;

//============ BOTON ====================================

int botonAntes = ZERO_TO_HERO;

//=======================================================

void setup()
{ 
  lcd.begin(16,2);
  lcd.print("PASSWORD:");
  tiempo1 = millis();

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);

  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(A4, INPUT);
}
//------------------------------------------------------
void loop()
{
  lcd.setCursor(9,0);
 	
  if(flag && contFive == 0)
  {
  	pass();
  }
  
  if(flag && !flagConfig)
  {
  	cerradura();
  }
  
  botonera();
		
  delay(15);
}

//============== FUNCIONES ====================================

//Funcion para parpadear los LEDs
void parpadearLed(int led)
{
  if((millis() - tiempoEncendido >= 100) && estadoLed)
  {
    estadoLed = !estadoLed;    
    digitalWrite(led,estadoLed);
    tiempoApagado = millis();
  }
  if((millis() - tiempoApagado >= 400) && !estadoLed)
  {
    estadoLed = !estadoLed;    
    digitalWrite(led,estadoLed);    
    tiempoEncendido = millis();
  }
}

//Funcion para mostrar si la clave es correcta o incorrecta
void passwordResult(int result)
{
  if(result)
  {
    lcd.setCursor(0,0);
    lcd.print("PASSWORD OK     ");
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("PASSWORD FAIL   ");
  }
}

//Funcion para resetear la cerradura
void reset()
{
  lcd.setCursor(0,0);
  lcd.print("PASSWORD:       ");
  strcpy(password,"      ");
  indice = 0;
  contFive = 0;
  flag = 0;
}

//Funcion cronometro 5 segundos
int timer(int start)
{
  	tiempo2 = millis();
  	if(tiempo2 > (tiempo1+1000))
    {       
      tiempo1 = millis();
      tiempoSegundos = tiempo1/1000;
      
      if(start)
      {
        contFive++;
      }
  	}
  	return contFive;
}

//Funcion principal cerradura
void cerradura()
{
  if(!strcmp(password,default_password))
  {
    passwordResult(SUCCESS);
    parpadearLed(LED_VERDE);
    if(timer(1) >= 5)
    {
  		reset();
        flag = 1;
    }
  }
  if(indice == 7 && !flagConfig)
  {
	parpadearLed(checkPassword());
    if(timer(1) >= 5)
    {
  		reset();
        flag = 1;
    }
  }
}

//Funcion clave
void pass()
{
  tecla = teclado.getKey();

  if(tecla) 
  {
    password[indice] = tecla;
    lcd.print(password);
    indice++;
  }
}

//Funcion para leer los botones, y retorna el boton que se presiona
int leerBoton()
{
  int botonReset = digitalRead(11);
  int botonTest = digitalRead(12);
  int botonConfig = analogRead(A4);
  
  if(botonReset)
  {
    return BUTTON_RESET;
  }
  if(botonTest)
  {
    return BUTTON_TEST ;
  }
  if(botonConfig)
  {
    return BUTTON_CONFIG;
  }
  return NO_BUTTON;
}

//Funcion boton anti rebote
void antiRebote()
{
  int botonAhora = leerBoton();
  
  if(botonAhora && botonAhora != botonAntes)
  {
     switch(botonAhora)
     {
       case BUTTON_RESET:
       flagReset = 1;
       break;
       case BUTTON_TEST:
       flagTest = 1;
       flag = 0;
       break;
       case BUTTON_CONFIG:
       digitalWrite(LED_ROJO,HIGH);
       flagConfig = 1;
       break;
     }
  }
  botonAntes = botonAhora;
}

//Funcion para configurar una nueva clave
void config()
{
  if(contFive == 0)
  {    
  	pass();
  }
  if(indice == 7)
  {
  	strcpy(default_password,password);
    lcd.setCursor(0,0);
    lcd.print("NEW PASSWORD OK ");
    flagPassOk = 1;
    digitalWrite(LED_ROJO,LOW);
  }
  
}

//Funcion para corroborar la clave
int checkPassword()
{
  if(!strcmp(password,default_password))
  {
    passwordResult(SUCCESS);
    return LED_VERDE;
  }
  else
  {
    passwordResult(FAIL);
    return LED_ROJO;
  }
  flagTest = 0;
}

//Funcion que realiza la accion de cada boton
void botonera()
{
  antiRebote();
  
  if(!flag && flagTest && !flagConfig)
  {
    parpadearLed(checkPassword());
    if(timer(1) >= 5)
    {
  		reset();
        flag = 1;
    }
  }
  if(flagReset)
  {
    reset();
    flagReset = 0;
    flag = 1;
  }
  if(flagConfig)
  {
    flag = 0;
    
    config();
    
    if(flagPassOk)
    {
      parpadearLed(LED_VERDE);
      if(timer(1) >= 5)
      {
        reset();
        flag = 1;
        flagConfig = 0;
        flagPassOk = 0;
      }
    }
  }
}