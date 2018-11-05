clc;
clear all;

%% Parámetros generales de simulación
Ts = 0.1;
Tf = 200;

%% Se definen parámetros del PI 1GDL
k = 9.22;
T = 34.31;
L = 3.17;
t = 0.15;
H = 1/(k*(t+(L/T)));
I = 1/(T);

sim('PI1GDL', Tf);
sim('PI1GDL_Z', Tf);

%% Se definen par�metros del controlador de 2GDL
tc = 0.2;
to = L/T;
a = (tc*(2-tc))+to;
Kp = (a)/(k*(tc+to).^2);
Ti = T*(a/(1+to));
B = (T*tc)/(Ti);

sim('PI2GDL', Tf);
sim('PI2GDL_Z', Tf);

%% Se grafica Bode con m�rgenes de ganancia
% s = tf('s');
% C = H*(1+(I/s));
% P = k*(exp(-L*s))/(T*s+1);
% L1 = C*P;
% figure;
% margin(L1);
% grid on;
