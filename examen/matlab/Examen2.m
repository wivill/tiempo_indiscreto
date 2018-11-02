clc;
clear all;
%% Se definen parámetros
Ts = 0.1;
Tf = 60;
k = 9.22;
T = 34.31;
L = 3.17;
t = 0.15;
P = 1/(k*(t+(L/T)));
I = 1/(T);

sim('PI1GDL', Tf);
sim('PI1GDL_Z', Tf);

%% Se grafica Bode con márgenes de ganancia
s = tf('s');
C = P*(1+(I/s));
P = k*(exp(-L*s))/(T*s+1);
L1 = C*P;
figure;
margin(L1);
grid on;
