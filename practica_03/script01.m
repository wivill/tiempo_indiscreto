clear;
clc;

Ts = 0.01;

s = tf('s');
z = tf('z', Ts);

S1 = (3)/(s + 2);
Z1 = c2d(S1, Ts);
ssZ1 = ss(Z1);
ssS1 = ss(S1);