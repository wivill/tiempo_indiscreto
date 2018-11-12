#include "Arduino.h"
#include <TaskScheduler.h> //Se carga la librería del Scheduler
Scheduler RealTimeCore; //Esto crea un objeto del tipo Scheduler (definido por la librería). En este caso, el objeto se llama RealTimeCore, pero se puede poner cualquier nombre

// Se deben definir los prototipos de las funciones de cada una de las tareas, en este caso se definen tres funciones. En inglés es lo que se llaman Callbacks. Se puede poner el nombre que quiera.
void Task_LeerPotX();
void Task_LeerPotSP();
void Task_M();
void Task_Action();
void PrintFun();

int M = 0;
int led = 8;
int X = 0.0;      // Variable que maneja el Potenciometro para control Manual
int sp = 0;    // Set Point
int sp_ant = 0;
int U_pwm = 0.0;

float Y = 0.0;    // Salida de la Planta
float Y_ant = 0.0;

// Parametros PID
const float kp = 0.4696 ;  
const float ti = 0.0291;
const float tt = 0.0146; // para antiwindup
const float td = 0.0;
const float ts = 0.01;     // Tiempo de Muestreo (s)
const float alpha = 0.0;
const float Beta = 1.0;   

float I_ant = 0.0; 
float D_ant = 0.0;
float error = 0.0;
float error_ant = 0.0;
float P = 0.0;
float I = 0.0;
float D = 0.0;
float U = 0.0;
float U_antiw = 0.0;
float antiw = 0.0;

// Acá se crean las tareas. Las tareas son objetos del tipo Task definidos por la librería:
//Task Task_LeerX(200, TASK_FOREVER, &Task_LeerPotX, &RealTimeCore); //Tarea que se repite cada 1000 milisegundos indefinidamente
//Task Task_LeerSP(200, TASK_FOREVER, &Task_LeerPotSP, &RealTimeCore); //Tarea que se repite cada 3000 milisegundos indefinidamente
Task Task_LeerM(500, TASK_FOREVER, &Task_M, &RealTimeCore); //Tarea que se repite sólo tres veces cada 5000 milisegundos
Task Action_PID(100, TASK_FOREVER, &Task_Action, &RealTimeCore); // Tarea para accion del PID 
Task Print_Datos(1000, TASK_FOREVER, &PrintFun, &RealTimeCore); 
// Ahora se deben definir explícitamente las funciones

// Esta función se encarga de leer el valor del potenciómetro Manual
void Task_LeerPotX(){
  X = analogRead(A4);
  X = map(X, 0, 1023, 0, 100);
}

void Task_LeerPotSP(){
  sp = analogRead(A5);
  sp = map(sp, 0, 1023, 0, 100);
}

void Task_M(){
  M = analogRead(A3);
  if (M >= 1000) {
    digitalWrite(led, HIGH);
  } else{
    digitalWrite(led, LOW);
  } 
}

void Task_Action(){ 
  Task_LeerPotSP();
  Y = analogRead(A1);
  Y = map(Y, 0, 1023, 0, 100);
  U = 0;
  U_pwm = 0;
  if(M <= 200){
    Task_LeerPotX();
    U_pwm = map(X, 0, 100, 0, 255);
    analogWrite(9, U_pwm);
    U = 0;
    I = 0;
    I_ant = 0;
    P = 0;
  }
  else if(M >= 950){
    error = Beta*sp - Y;      // 0< Sp y Y< 100 
    P = kp*(error);
    I = I_ant + ((kp*ts)/ti)*(error) + (antiw/tt);
    U = P + I;
    U_antiw = U;
    if(U >= 1023){
      //Sature a 1023 
      U = 1023;
      antiw = U - U_antiw;
      U_pwm = map(U, 0, 1023, 0, 255);
      analogWrite(9, U_pwm);
    }else if(U <= 0){
      //Sature a 0 
      U = 0;
      antiw = U - U_antiw;
      U_pwm = map(U, 0, 1023, 0, 255);
      analogWrite(9, U_pwm);
    }else{
      //Solo escriba
      U_pwm = map(U, 0, 1023, 0, 255);
      antiw = U - U_antiw;
      analogWrite(9, U_pwm);  
    }
  
    I_ant = I;
    Y_ant = Y;
    sp_ant = sp;
    error_ant = error;
  }

}

void PrintFun(){
  Serial.print("------------------------------------------\n");
  if(M >= 950){ 
    Serial.print("Modo Aumatico: \n"); 
  } else {
    Serial.print("Modo Manual: \n"); 
 }

  Serial.print("\nSeñal Manual: ");
  Serial.print(X);
  Serial.print("\nSet Point: ");
  Serial.print(sp);
  Serial.print("\nSalida de Planta: ");
  Serial.print(Y);
  Serial.print("\nSeñal de Control: ");
  Serial.print(U_pwm);
  Serial.print("\n");
}

void setup() {
  pinMode(13, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(A3, INPUT);
  pinMode(0, INPUT);
  pinMode(led, OUTPUT);
  
  // El código que se ponga acá se ejecuta una única vez al inicio:
  Serial.begin(9600); //se inicia la comunicación serial a 9600 bauds
  RealTimeCore.init(); //Inicializa el scheduler
  
  Serial.println("Se inicializo el Scheduler");
  
//  RealTimeCore.addTask(Task_LeerX); //Se agrega la tarea 01 al scheduler
//  RealTimeCore.addTask(Task_LeerSP); //Se agrega la tarea 02 al scheduler
  RealTimeCore.addTask(Task_LeerM); //Se agrega la tarea 03 al scheduler
  RealTimeCore.addTask(Action_PID); // Se agrega la tarea de Accion al scheduler
  RealTimeCore.addTask(Print_Datos); // Se agrega la tarea de Accion al scheduler
 
  Serial.println("Se agregaron las tareas al Scheduler");
//  Task_LeerX.enable(); // Se pone el flag de enable para la tarea 01. Por default, las tareas están desabilitadas
//  Task_LeerSP.enable(); // Se pone el flag de enable para la tarea 02. Por default, las tareas están desabilitadas
  Task_LeerM.enable();// 
  Action_PID.enable(); // Activo 
  Print_Datos.enable();
}

void loop() {
  RealTimeCore.execute(); // Cuando se usa un scheduler, esta instrucción es la única que debería estar en el loop
}
