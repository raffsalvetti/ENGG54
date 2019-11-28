% Obter fft
[dados, fs] = audioread('1kHz_gravado_alto.wav');
L = 32768;  %largura que sera analisada
dados2 = dados(1+L:L+L);    %pegar uma parte mais para frente para evitar algum ruido ou transiente do inicio da gravacao
y = dados2;  %sinal que vai passar na FFT
L = length(y);  %largura do sinal
NFFT = 2^nextpow2(L); %calcula a proxima potencia de 2 pra caso precise preencher para ficar num valor potencia de 2
Y = fft(y,NFFT)/L;  %fft
f = fs/2*linspace(0,1,NFFT/2+1);    %eixo da frequencia
fft_final = 2*abs(Y(1:NFFT/2+1));

% Desenhar espectro de amplitude com apenas um lado da fft.
fg = 1000;  %frequencia gravada
grafico = stem(f, fft_final);    %grafico
set(grafico, 'Marker', 'none');   %remover marcadores
xticks((0:(L/fg))*fg);  %graduacao a cada 1000Hz
title('Espectro de amplitude de y(t)')
xlabel('Frequencia (Hz)')
ylabel('|Y(f)|')

% Calcular distorcao harmonica total
bin_freq = f(NFFT/2+1)/(NFFT/2+1);  %incremento de frequencia a cada "bin"
n_harmonicos = (1:(fs/fg)/2);   %numero de cada harmonico incluindo a freq fundamental
f_harmonicos = n_harmonicos*1000;   %frequencia de cada harmonico
bin_harmonicos = round(f_harmonicos/bin_freq); %"bin" de cada harmonico (teorico ja que as frequencias se espalham)
int_res = 5; %como as frequencias se espalham nos bins em vez de ficar exatamente em um so, e necessario integrar, esse parametro da a largura que sera integrada
considerados = 20;  %numero de harmonicos considerados, incluindo a freq principal
%pre alocacao
soma_harmonicos = n_harmonicos*0;   %harmonicos integrados
sqr_harmonicos = n_harmonicos*0;    %harmonicos quadrados
numerador = 0;  %termo de cima da equacao
%integrar todos os harmonicos
for i = (1:considerados)
   
   %integracao do pulso
   for j = ((bin_harmonicos(i)-int_res):(bin_harmonicos(i)+int_res))
       soma_harmonicos(i) = soma_harmonicos(i) + fft_final(j);
   end
   
   sqr_harmonicos(i) = soma_harmonicos(i)*soma_harmonicos(i);   %quadrado dos valores
   
   numerador = numerador + sqr_harmonicos(i);
end
numerador = numerador - sqr_harmonicos(1);   %tirar a frequencia principal do numerador pois ela foi incluida no loop por simplicidade
numerador = sqrt(numerador);    %raiz
%RESULTADO
distorcao = numerador/soma_harmonicos(1);   %divide o numerador pela frequencia principal
distorcao_percentual = distorcao*100;