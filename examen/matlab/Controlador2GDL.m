clc;
clear all;

%% Se definen parámetros de la planta
k = 9.22;
T = 34.31;
L = 3.17;

%% Se definen parámetros del controlador de 2GDL
tc = 0.2;
to = L/T;
a = (tc*(2-tc))+to;
Kp = (a)/(k*(tc+to).^2);
Ti = T*(a/(1+to));
B = (T*tc)/(Ti);

%% Se grafica Bode con márgenes de ganancia
s = tf('s');
P = k*(exp(-L*s))/(T*s+1);
Cr= Kp*(B+(1/s*Ti));
Cy= Kp*(1+(1/s*Ti));
C = Cr + Cy;
L1 = C*P;
figure;
margin(L1);
grid on;