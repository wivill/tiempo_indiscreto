clear;
clc;

Ts = 0.01;

s = tf('s');
z = tf('z', Ts);

S1 = (3)/(s + 2);
Z1 = c2d(S1, Ts);
ssZ1 = ss(Z1);
ssS1 = ss(S1);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
A2 = [1 -1;-3 5];
B2 = [-3;-4];
C2 = [1 -2];
D2 = 0;

ssS2 = ss(A2, B2, C2, D2);
ssZ2 = c2d(ssS2, Ts);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
A3 = [1 -4 5;-1 7 -1;1 3 4];
B3 = [1;2;1];
C3 = [1 -1 1];
D3 = 0;

ssS3 = ss(A3, B3, C3, D3);
ssZ3 = c2d(ssS3, Ts);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
S4 = (5)/(s + 25);

ssS4 = ss(S4);
ssZ4 = c2d(ssS4, Ts);

% parte 2

sys1 = (ssZ1 + ssZ2)*(ssZ3);
sys_total = feedback(sys1,ssZ4);

% parte 3

ctrb(sys_total.A, sys_total.B); 