/*
 * Este programa sirve de plantilla para utilizar la librería
 * TaskScheduler en el curso IE0731: Sistemas en Tiempo discreto
 *
 * Se recomienda revisar la documentación que está
 * en el sitio virtual del curso, junto con la librería.
 * El sitio donde el autor de la librería aloja el código es el siguiente:
 * https://github.com/arkhipenko/TaskScheduler
 */
#include "Arduino.h"
#include <TaskScheduler.h> //Se carga la librería del Scheduler
Scheduler RealTimeCore; //Esto crea un objeto del tipo Scheduler (definido por
// la librería). En este caso, el objeto se llama RealTimeCore, pero se puede
// poner cualquier nombre

// Se deben definir los prototipos de las funciones de cada una de las tareas,
// en este caso se definen tres funciones. En inglés es lo que se llaman
// Callbacks. Se puede poner el nombre que quiera.
void tarea01Fun();

// Acá se crean las tareas, son de tipo Task definidos por la libreria
Task tarea01(100, TASK_FOREVER, &tarea01Fun, &RealTimeCore); // Tarea que se
// repite cada 100 milisegundos indefinidamente, de acuerdo con el Ts elegido

// Parametros necesarios del PID
int M;
int X;
int sp;
float Xesc;
float spesc;
int i=0;
int j=0;
// Constantes del PI
float kp=0.85;
float ti=0.019;
float ts=0.01;
int B=1;
float Iante=0;
float SPante=0;
float Yante=716;
float SPesc;
float Yesc;
float Sp;
float Y;
float I;
float P;
float U;
float Uesc;
float Upwm;

// Contenido de la función a ejecutarse.
void tarea01Fun() {
  // Señal M, viene de boton y lo lee A0
  M = analogRead(A0);
  Serial.print("Señal M:");
  Serial.println(M);


  if (M==1023){
    //Entra en modo manual
    if (i==0){
      Serial.println("Entre al modo Manual");
      i++;
      }
    // Señal X viene de pote y lo lee A1
    X=analogRead(A1);
    Serial.print("Señal X:");
    Serial.println(X);
    // Escalado enre 0 y 100
    Xesc=map(X,0,808,0,100);
    //Xesc=X/8.08;
    Serial.print("Señal Xesc:");
    Serial.println(Xesc);
    Serial.print("\n");
    }else{
      //Entra en modo Automatico
      if(j==0){
        Serial.println("Entre al modo Automático");
        j++;
        }
        // Definicion de ecuacion u del controlador
        //R=SP
        Sp=analogRead(A2);
        Serial.print("Señal SP:");
        Serial.println(Sp);
        //SPesc=Sp/10.23;
        SPesc=map(Sp,0,1023,0,100);
        Serial.print("SP escalado:");
        Serial.println(SPesc);

        // Salida de la planta, analogica
        Y=analogRead(A3);
        Serial.print("Y:");
        Serial.println(Y);
        Yesc=map(Y,204.6,1023,0,100);
        Serial.print("Yesc:");
        Serial.println(Yesc);

        P=kp*(B*Sp-Y);
        Serial.print("PPPPPPPPPPPPPPPPPPPPP          ");
        Serial.println(P);
        I=Iante+((kp*ts)/(ti))*(SPante-Yante);
        Serial.print("IIIIIIIIIIIIIIIIII             ");
        Serial.println(I);
        //Señal a pin PWM
        U=P+I;
        Serial.print("Señal U:                       ");
        Serial.println(U);
        //analogWrite()

        if(U>=1023){
         //Sature a 1023
         U=1023;
         Upwm=map(U,0,1023,0,255);
         analogWrite(3,Upwm);
         Serial.print("Señal Upwm:                       ");
         Serial.println(Upwm);
         Uesc=map(U,0,1023,0,100);
         Serial.print("Señal Uesc:                       ");
         Serial.println(Uesc);
        }else if(U<=0){
          //Sature a 0
          U=0;
          Upwm=map(U,0,1023,0,255);
          analogWrite(3,Upwm);
          Serial.print("Señal Upwm:                       ");
          Serial.println(Upwm);
          Uesc=map(U,0,1023,0,100);
          Serial.print("Señal Uesc:                       ");
          Serial.println(Uesc);
        }else{
          //Solo escriba
         Upwm=map(U,0,1023,0,255);
         analogWrite(3,Upwm);
         Serial.print("Señal Upwm:                       ");
         Serial.println(Upwm);
         Uesc=map(U,0,1023,0,100);
         Serial.print("Señal Uesc:                       ");
         Serial.println(Uesc);
        }


        // Actualizacion de valores anteriores
        Iante=I;
        SPante=Sp;
        Yante=Y;


//        //Señal SP valor deseado, viene de pote lo lee A2
//        sp=analogRead(A2);
//        Serial.print("Señal SP:");
//        Serial.println(sp);
//        // Escalado entre 0 y 100
//        spesc=sp/10.23;
//        Serial.print("Señal SPesc:");
//        Serial.println(spesc);
//        Serial.print("\n");

    }
}





void setup() {
  // El código que se ponga acá se ejecuta una única vez al inicio:
  Serial.begin(9600); //se inicia la comunicación serial a 9600 bauds
  RealTimeCore.init(); //Inicializa el scheduler
  Serial.println("Se inicializo el Scheduler");
  RealTimeCore.addTask(tarea01); //Se agrega la tarea 01 al scheduler
  Serial.println("Se agregaron las tareas al Scheduler");
  tarea01.enable(); // Se pone el flag de enable para la tarea 01. Por default, las tareas están desabilitadas

}

void loop() {
  // Acá va el código que se repite indefinidamente:
  RealTimeCore.execute(); // Cuando se usa un scheduler, esta instrucción es la única que debería estar en el loop

}
