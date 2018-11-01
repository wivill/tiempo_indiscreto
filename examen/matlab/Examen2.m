clc;
clear all;

% Tiempo de muestreo y simulación
Ts = 0.1;
Tf = 120;

s = tf('s');
z = tf('z', Ts);

k = 9.22;
T = 34.31;
L = 3.17;
t = 0.15;

sim('PI1GDL',[0,Tf]);

planta = k/(T*s + 1);

planta_dis = c2d(planta, Ts, 'zoh');


