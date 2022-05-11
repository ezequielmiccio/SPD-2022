
/*
 *	Trabajo Práctico 1: Cronómetro Binario
 *
 *	Al iniciar la simulacion, todos los led deben comenzar apagados.
 *	Cada 1 segundo que transcurra, se deben encender los led
 *	de acuerdo al numero que representan.
 *
 *	Ejemplo:
 *
 *	El numero 15 (0000001111) se representaria encendiendo los
 *	leds AMARILLO y NARANJA.
 *	Al pasar al numero 16 (0000010000), se deben apagar todos
 *	los led anteriores y encenderse uno de los leds VERDES.
 *	Notese, en el ejemplo los 0 representan los led apagados
 *	y los 1 los led encendidos).
 *
 *	-------------------------------------------------------
 *
 *	Al presionarse el boton START, debe iniciar el cronometro.
 *	Volver a presionarlo hace que la secuencia se detenga.
 *  (Como si se hubiese presionado pausa).
 *	Al presionarse el boton RESET, el cronometro
 *	debe reiniciarse y arrancar de 0.
 *
 *	Tip: Modularizar la función que controla el encendido de los
 *	LEDS y de ser posible, todo el código para evitar repetir lineas lo mas posible.
 *  Usar millis para controlar el tiempo del contador para que el cambio de los
 *	leds encendidos sea perceptible para el ojo humano y
 *	documentar cada función creada en el código.
 *	Un breve comentario que diga que es lo que hace esa función
 *  y de corresponder, que retorna).
*/

//##### Una Ayudita #####
//--- Defines ---//
#define ZERO_TO_HERO 0          //? To start the counter from 0
#define BUTTON_START 2
#define BUTTON_RESET 3
#define FIRST_LED 4             //? First pin of the leds
#define LAST_LED 13             //? Last pin of the leds
#define BASE_MILLI_SECONDS 1000 //? Secods expresed in miliseconds
#define MAX_SECONDS 1023        //! Max amount of secods to show
//--- End Defines ---//

int pines[]={4,5,6,7,8,9,10,11,12,13};

void setup()
{
  	Serial.begin(9600);

  	for(int i = 0; i < 11;i++)
    {
      pinMode(pines[i],OUTPUT);
    }

  	pinMode(BUTTON_RESET,INPUT);
  	pinMode(BUTTON_START,INPUT);

}

unsigned long millisAntes = ZERO_TO_HERO;

int timer = ZERO_TO_HERO;
int bin = ZERO_TO_HERO;
int binario = ZERO_TO_HERO;
int flag;
int num;

String segundos = "Seconds: ";
String binarios = " | Binary: ";

void loop()
{
  unsigned long millisAhora = millis();

  if(millisAhora - millisAntes >= BASE_MILLI_SECONDS)
  {
    	cronoBinario();
    	millisAntes = millisAhora;
  }
  delay(20);
}

//Funcion que me prende los LEDs
void prenderLed()
{
    binario++;

    if(binario >= ZERO_TO_HERO && binario <= MAX_SECONDS)
    {
        num = binario;

        for(int i=0; i < 10; i++)
        {
            if(num % 2 == 1)
            {
              digitalWrite(pines[i], HIGH);
            }
            else
            {
              digitalWrite(pines[i],LOW);
            }
          	num = num /2;
        }
    }
}
void start()
{
  int botonStart = digitalRead(BUTTON_START);

  if(botonStart)
  {
    flag = !flag;
  }
  if(flag && binario <= 1023)
  {
    prenderLed();
    timer++;
	mostrarConsola();
  }
}

int flagReset = 0;

void reset()
{
  int botonReset = digitalRead(BUTTON_RESET);

  if(botonReset)
  {
    flagReset = 1;
  }
  if(flagReset)
  {
    flagReset = 0;
    flag = 1;
    timer = ZERO_TO_HERO;
    binario = ZERO_TO_HERO;
  }
}
void mostrarConsola()
{
  	if(binario <= MAX_SECONDS)
  	{
  		Serial.println(segundos + timer + binarios+
                 			 		digitalRead(13)+
                             		digitalRead(12)+
                             		digitalRead(11)+
                             		digitalRead(10)+
                             		digitalRead(9)+
                             		digitalRead(8)+
                             		digitalRead(7)+
                             		digitalRead(6)+
                             		digitalRead(5)+
                 			 		digitalRead(4));
	}
}
void apagarLed()
{
  	for(int i = 0; i < 11;i++)
    {
       digitalWrite(pines[i],LOW);
    }
}

void cronoBinario()
{
  	start();
    reset();

  	if(binario == MAX_SECONDS)
    {
       apagarLed();
    }
}
