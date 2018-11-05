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
Scheduler RealTimeCore; //Esto crea un objeto del tipo Scheduler (definido por la librería). En este caso, el objeto se llama RealTimeCore, pero se puede poner cualquier nombre

// Se deben definir los prototipos de las funciones de cada una de las tareas, en este caso se definen tres funciones. En inglés es lo que se llaman Callbacks. Se puede poner el nombre que quiera.
void Task_LeerPotM();
void Task_LeerPotA();
void tarea03Fun();

void Task_Action();
void Task_ReadSensor();


int Boton = 0;
int M = 0;      // Variable que maneja el Potenciometro para control Manual

int sp = 0;    // Set Point
float Y = 0;    // Salida de la Planta

// Parametros PID
float kp = 0.0;  
float ti = 0.0;
float td = 0.0;
float ts = 0.1;     // Tiempo de Muestreo (s)
float alpha = 0.0;

float Beta = 0.0;    
float Y_ant = 0.0;

float I_ant = 0.0; 
float D_ant = 0.0;


float P = 0.0;
float I = 0.0;
float D = 0.0;

float U = 0.0;
float U_ant = 0.0;
float U_pwm = 0.0;


// Acá se crean las tareas. Las tareas son objetos del tipo Task definidos por la librería:
Task Task_LeerM(500, TASK_FOREVER, &Task_LeerPotM, &RealTimeCore); //Tarea que se repite cada 1000 milisegundos indefinidamente
Task Task_LeerSP(500, TASK_FOREVER, &Task_LeerPotA, &RealTimeCore); //Tarea que se repite cada 3000 milisegundos indefinidamente
//Task tarea03(5000, 3, &tarea03Fun, &RealTimeCore); //Tarea que se repite sólo tres veces cada 5000 milisegundos
Task Action_PID(100, TASK_FOREVER, &Task_Action, &RealTimeCore); // Tarea para accion del PID 

// Ahora se deben definir explícitamente las funciones

// Esta función se encarga de leer el valor del potenciómetro Manual
void Task_LeerPotM(){
  M = analogRead(A4);
  M = map(M,217,1023,0,100);
  Serial.print("Señal Pot Manual: ");
  Serial.print(M);
  Serial.print("\n");
}

void Task_LeerPotA(){
  sp = analogRead(A5);
  sp = map(sp,220,1023,0,100);  
  Serial.print("Señal Pot Auto: ");
  Serial.print(sp);
  Serial.print("\n");
}

void Task_ReadSensor(){
  Y = analogRead(A1);
  Y = map(Y,206,1023,0,100);
  Serial.print("Señal Sensor: ");
  Serial.print(Y);
}

void Task_Action(){ 
  int Salida_Manual;
  Boton = analogRead(A3);
  
  if(Boton == 0){
    Serial.print("Modo Manual \n");
    Salida_Manual = map(M,0,100,0,255);
    analogWrite(A0,Salida_Manual);
    Serial.print("Señal de Control es: ");
    Serial.print(A0);
    Serial.print("\n");
  }
  if(Boton == 1023){
    int error;
    Serial.print("Modo Automatico \n");
    error = Beta*sp-Y;
    P = kp*(error);
    I = I_ant+(kp*ts/ti)*error;
    I_ant = I;
    D = ((alpha*td)/(alpha*td + ts))*D_ant - ((kp*td)/(alpha*td+ts))*(Y-Y_ant);
    D_ant = D; 
    U = P+I+D;
    if(U>=1023){
      U= 1023;
      U_pwm = map(U,0,1023,0,255);
      analogWrite(0,U_pwm);
    }
    else if(U<=0){
      U=0;
      U_pwm = map(U,0,1023,0,255);
      analogWrite(0,U_pwm);
    }
  }
}

void tarea03Fun(){
     Y = analogRead(A1); 
     
}

void setup() {
  
  // El código que se ponga acá se ejecuta una única vez al inicio:
  Serial.begin(9600); //se inicia la comunicación serial a 9600 bauds
  RealTimeCore.init(); //Inicializa el scheduler
  
  Serial.println("Se inicializo el Scheduler");
  
  RealTimeCore.addTask(Task_LeerM); //Se agrega la tarea 01 al scheduler
  RealTimeCore.addTask(Task_LeerSP); //Se agrega la tarea 02 al scheduler
 // RealTimeCore.addTask(tarea03); //Se agrega la tarea 03 al scheduler
  RealTimeCore.addTask(Action_PID); // Se agrega la tarea de Accion al scheduler
  
  Serial.println("Se agregaron las tareas al Scheduler");
  Task_LeerM.enable(); // Se pone el flag de enable para la tarea 01. Por default, las tareas están desabilitadas
  Task_LeerSP.enable(); // Se pone el flag de enable para la tarea 02. Por default, las tareas están desabilitadas
  
  Action_PID.enable(); // Activo 
}

void loop() {
  // Acá va el código que se repite indefinidamente:
  RealTimeCore.execute(); // Cuando se usa un scheduler, esta instrucción es la única que debería estar en el loop
}
