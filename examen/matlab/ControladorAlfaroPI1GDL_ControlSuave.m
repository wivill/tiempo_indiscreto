clc;
clear all;

%% Se definen parámetros de la planta
k = 9.22;
T = 34.31;
L = 3.17;

%% Se definen parámetros del controlador de 1GDL Sintonización Alfaro
Kp = (0.40*T)/(k*L);
Ti = 1/T;

%% Se grafica Bode con márgenes de ganancia
s = tf('s');
C = Kp*(1+(Ti/s));
P = k*(exp(-L*s))/(T*s+1);
L1 = C*P;
figure;
margin(L1);
grid on;
