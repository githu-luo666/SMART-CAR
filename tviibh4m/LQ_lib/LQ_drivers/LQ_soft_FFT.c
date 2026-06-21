
/* matlab 代码  可以对比fft的结果
Fs = 1024;            % Sampling frequency
T = 1/Fs;             % Sampling period
L = 1024;             % Length of signal
t = (0:L-1)*T;        % Time vector
S = 70*sin(2*pi*50*t) + 100*sin(2*pi*120*t);
f = Fs*(0:(L/2))/L;
Y = fft(S);
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
hold off
plot(f,P1)
title('Single-Sided Amplitude Spectrum of S(t)')
xlabel('f (Hz)')
ylabel('|P1(f)|')
hold on
plot(1000*t(1:50),S(1:50))
title('Signal Corrupted with Zero-Mean Random Noise')
xlabel('t (milliseconds)')
ylabel('X(t)')
 */








