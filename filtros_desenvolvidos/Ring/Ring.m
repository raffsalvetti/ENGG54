%modulação em Ring:

[x,Fs] = audioread('input.wav');
info = audioinfo('input.wav')
disp(Fs);
indice = 1:length(x);
Fc = 440;
carrier =  sin(2*pi*indice*(Fc/Fs));

for c=1 :length(x)
    y(c) = x(c) * carrier(c);
end
  
sound(y,Fs);

figure(1);
hold on
plot(y,'r');
plot(x,'b');
title('Efeito Ring');
