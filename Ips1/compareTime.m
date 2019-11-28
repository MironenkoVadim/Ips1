clc; close all; clear all;

filename1 = 'parallel.txt';
filename2 = 'serial.txt';

f1 = fopen(filename1);
f2 = fopen(filename2);

T1 = fscanf(f1,'%i %f\n');
T2 = fscanf(f2,'%i %f\n');

figure()
hold on; grid on;
xlabel('Point'); ylabel('Time, sec')
plot(T1(1:2:end), T1(2:2:end), 'r', 'LineWidth',2)
plot(T2(1:2:end), T2(2:2:end), 'black', 'LineWidth',2)
legend('Parallel', 'Serial', 'Location','best')