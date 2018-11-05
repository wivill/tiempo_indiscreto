clc;
clear all;

%% Parámetros generales de simulación
Ts = 0.1;
Tf = 150;

%% Se definen parámetros del PI 1GDL
k = 9.22;
T = 34.31;
L = 3.17;
t = 0.15;
H = 1/(k*(t+(L/T)));
I = 1/(T);

sim('PI1GDL', Tf);
sim('PI1GDL_Z', Tf);

figure(1)
grid on
hold on
plot(Y1, 'black')
plot(Y1_Z, '--')
plot(D1)
plot(SP1)
legend('Continuo', 'Discreto')
title('Salida del sistema continuo y discreto, PI 1GDL')
hold off

figure(2)
grid on
hold on
plot(U1, 'black')
plot(U1_Z, '--')
plot(D1)
legend('Continuo', 'Discreto')
title('Esfuerzo de control del sistema continuo y discreto, PI 1GDL')
hold off

%% Se definen par�metros del controlador de 2GDL
tc = 0.2;
to = L/T;
a = (tc*(2-tc))+to;
Kp = (a)/(k*(tc+to).^2);
Ti = T*(a/(1+to));
B = (T*tc)/(Ti);

sim('PI2GDL', Tf);
sim('PI2GDL_Z', Tf);

figure(3)
grid on
hold on
plot(Y2, 'black')
plot(Y2_Z, '--')
plot(D2)
plot(SP2)
legend('Continuo', 'Discreto')
title('Salida del sistema continuo y discreto, PI 2GDL')
hold off

figure(4)
grid on
hold on
plot(U2, 'black')
plot(U2_Z, '--')
plot(D2)
legend('Continuo', 'Discreto')
title('Esfuerzo de control del sistema continuo y discreto, PI 2GDL')
hold off


%% Se grafica Bode con m�rgenes de ganancia
% s = tf('s');
% C = H*(1+(I/s));
% P = k*(exp(-L*s))/(T*s+1);
% L1 = C*P;
% figure;
% margin(L1);
% grid on;
