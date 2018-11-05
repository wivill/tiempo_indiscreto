clc;
clear;

%% Parámetros generales de simulación
Ts = 0.1;
Tf = 150;
s = tf('s');
z = tf('z', Ts);
options = bodeoptions; options.MagUnits = 'abs';

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
ylabel('Amplitud')
xlabel('Tiempo T/(s)')
legend('Continuo', 'Discreto', 'Perturbación', 'Referencia')
title('Salida del sistema continuo y discreto, PI 1GDL')
hold off

figure(2)
grid on
hold on
plot(U1, 'black')
plot(U1_Z, '--')
plot(D1)
ylabel('Amplitud')
xlabel('Tiempo T/(s)')
legend('Continuo', 'Discreto', 'Perturbación')
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
ylabel('Amplitud')
xlabel('Tiempo T/(s)')
legend('Continuo', 'Discreto', 'Perturbación', 'Referencia')
title('Salida del sistema continuo y discreto, PI 2GDL')
hold off

figure(4)
grid on
hold on
plot(U2, 'black')
plot(U2_Z, '--')
plot(D2)
ylabel('Amplitud')
xlabel('Tiempo T/(s)')
legend('Continuo', 'Discreto', 'Perturbación')
title('Esfuerzo de control del sistema continuo y discreto, PI 2GDL')
hold off

%% Se grafica Bode con m�rgenes de ganancia
% PI 1GDL
C = H*(1+(I/s));
P = k*(exp(-L*s))/(T*s+1);
L1 = C*P;
Ms1 = 1/(1+L1);
[mGanancia1,mFase1,wmGanancia1,wmFase1] = margin(L1);
figure(5);
margin(L1);
title('Márgenes de fase y ganancia para el sistema con PI 1GDL')
grid on;

figure(6)
bodeplot(Ms1,options)
grid on
title('Medición de función de sensibilidad Ms para sistema con PI 1GDL')

% 2GDL
Cr= Kp*(B+(1/(s*Ti)));
Cy= Kp*(1+(1/(s*Ti)));
L2 = (Cy+Cr)*P;
Ms2 = 1/(1+L2);
[mGanancia2,mFase2,wmGanancia2,wmFase2] = margin(L2);

figure(7);
margin(L2);
title('Márgenes de fase y ganancia para el sistema con PI 2GDL')
grid on;

figure(8)
bodeplot(Ms2,options)
grid on
title('Medición de función de sensibilidad Ms para sistema con PI 2GDL')