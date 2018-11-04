clc;
clear all;

%% Se definen par�metros de la planta
k = 9.22;
T = 34.31;
L = 3.17;

%% Se definen par�metros del controlador de 2GDL
tc = 0.2;
to = L/T;
a = (tc*(2-tc))+to;
Kp = (a)/(k*(tc+to).^2);
Ti = T*(a/(1+to));
B = (T*tc)/(Ti);