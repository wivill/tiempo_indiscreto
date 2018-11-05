clc;
clear;

%% Parámetros generales de simulación
Ts = 0.1;
Tf = 150;
s = tf('s');
z = tf('z', Ts);
options = bodeoptions; options.MagUnits = 'abs';

% planta
k = 9.22;
T = 34.31;
L = 3.17;

%% Se definen parámetros de los controladores

% Analítico
t = 0.15;
H = 1/(k*(t+(L/T)));
I = 1/(T);

%Alfaro control suave
Kp_al = (0.40*T)/(k*L);
Ti_al = 1/T;
%Alfaro control estricto
Kp_es = (0.57*T)/(k*L);
Ti_es = 1/T;

% PI 2GDL analítico
tc = 0.2;
to = L/T;
a = (tc*(2-tc))+to;
Kp = (a)/(k*(tc+to).^2);
Ti = T*(a/(1+to));
B = (T*tc)/(Ti);

% Simulación
sim('PI1GDL', Tf);
sim('PI1GDL_Z', Tf);
sim('PI2GDL', Tf);
sim('PI2GDL_Z', Tf);

figure(1)
grid on
hold on
plot(Y_SA, 'black')
plot(Y_alfaro_est, '--')
plot(Y_alfaro_soft)
plot(Y2)
plot(D_SA)
plot(SP_SA)
ylabel('Amplitud')
xlabel('Tiempo T/(s)')
legend('Sintonización analítica', 'Alfaro estricto', 'Alfaro suave', '2GDL analítico', 'Perturbación', 'Referencia')
title('Comparación de salida entre controladores')
hold off

figure(2)
grid on
hold on
plot(U_SA, 'black')
plot(U_alfaro_est, '--')
plot(U_alfaro_soft)
plot(U2)
plot(D_SA)
% plot(SP_SA)
ylabel('Amplitud')
xlabel('Tiempo T/(s)')
legend('Sintonización analítica', 'Alfaro estricto', 'Alfaro suave', '2GDL analítico', 'Perturbación')
title('Esfuerzo de control entre controladores')
hold off

figure(3)
grid on
hold on
plot(Y_SA_Z, 'black')
plot(Y_alfaro_est_Z, '--')
plot(Y_alfaro_soft_Z)
plot(Y2_Z)
plot(D_SA_Z)
plot(SP_SA)
ylabel('Amplitud')
xlabel('Tiempo T/(s)')
legend('Sintonización analítica', 'Alfaro estricto', 'Alfaro suave', '2GDL analítico', 'Perturbación', 'referencia')
title('Comparación de salida entre controladores discretizados')
hold off

figure(4)
grid on
hold on
plot(U_SA_Z, 'black')
plot(U_alfaro_est_Z, '--')
plot(U_alfaro_soft_Z)
plot(U2_Z)
plot(D_SA)
% plot(SP_SA)
ylabel('Amplitud')
xlabel('Tiempo T/(s)')
legend('Sintonización analítica', 'Alfaro estricto', 'Alfaro suave', '2GDL analítico', 'Perturbación')
title('Esfuerzo de control entre controladores discretizados')
hold off

%% Se grafica Bode con m�rgenes de ganancia
% PI SA
C_SA = H*(1+(I/s));
P = k*(exp(-L*s))/(T*s+1);
L_SA = C_SA*P;
Ms_SA = 1/(1+L_SA);
[mGanancia_SA,mFase_SA,wmGanancia_SA,wmFase_SA] = margin(L_SA);
figure(5);
margin(L_SA);
title('Márgenes de fase y ganancia con PI Sintonización Analítica')
grid on;

figure(6)
bodeplot(Ms_SA,options)
grid on
title('Función de sensibilidad Ms con PI Sintonización Analítica')

figure(7)
fc_graf_polar_L(L_SA)
grid on
title('Diagrama de fase para Ms con PI Sintonización Analítica')



% alfaro estricto
C_est = Kp_es*(1+(I/s));
L_est = C_est*P;
Ms_est = 1/(1+L_est);
[mGanancia_est,mFase_est,wmGanancia_est,wmFase_est] = margin(L_est);
figure(8);
margin(L_est);
title('Márgenes de fase y ganancia con PI Alfaro Control Estricto')
grid on;

figure(9)
bodeplot(Ms_est,options)
grid on
title('Función de sensibilidad Ms con PI Alfaro Control Estricto')

figure(10)
fc_graf_polar_L(L_est)
grid on
title('Diagrama de fase para Ms con PI Alfaro Control Estricto')

% alfaro suave
C_al = Kp_al*(1+(I/s));
L_soft = C_al*P;
Ms_soft = 1/(1+L_soft);
[mGanancia_soft,mFase_soft,wmGanancia_soft,wmFase_soft] = margin(L_soft);
figure(11);
margin(L_soft);
title('Márgenes de fase y ganancia con PI Alfaro Control Suave')
grid on;

figure(12)
bodeplot(Ms_soft,options)
grid on
title('Función de sensibilidad Ms con PI Alfaro Control Suave')

figure(13)
fc_graf_polar_L(L_soft)
grid on
title('Diagrama de fase para Ms con PI Alfaro Control Suave')


% 2GDL
Cr= Kp*(B+(1/(s*Ti)));
Cy= Kp*(1+(1/(s*Ti)));
L2 = (Cy+Cr)*P;
Ms2 = 1/(1+L2);
[mGanancia2,mFase2,wmGanancia2,wmFase2] = margin(L2);

figure(14);
margin(L2);
title('Márgenes de fase y ganancia con PI 2GDL')
grid on;

figure(15)
bodeplot(Ms2,options)
grid on
title('Función de sensibilidad Ms con PI 2GDL')

figure(16)
fc_graf_polar_L(L2)
grid on
title('Diagrama de fase para Ms con PI 2GDL')

% figure(17)
% fc_fragility_rings(Kp_es,Ti_es,0,P)
% grid on
% title('Fragilidad con PI Alfaro Control Estricto')
% 
% figure(18)
% fc_fragility_rings(Kp_al,Ti_al,0,P)
% grid on
% title('Fragilidad con PI Alfaro Control Suave')
% 
% figure(19)
% fc_fragility_rings(H,I,0,P)
% grid on
% title('Fragilidad con PI Sintonización Analítica')
% 
% figure(20)
% fc_fragility_rings(Kp,Ti,0,P)
% grid on
% title('Fragilidad con PI 2GDL')

%% Medición de IAE

sim('IAE', Tf);

figure(21)
grid on
hold on
plot(Y_alfaro_est, 'black')
plot(Y_alfaro_est_Z, '--')
plot(D_SA)
plot(SP_SA)
ylabel('Amplitud')
xlabel('Tiempo T/(s)')
legend('Alfaro estricto', 'Alfaro estricto discreto', 'Perturbación', 'Referencia')
title('Comparación de salida entre controlador continuo y discreto, sintonización Control Estricto, Ts = 0.1 s')
hold off

IAE_Elegido