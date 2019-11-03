[x,Fs] = audioread('./sample2.wav'); 
a = 10;
m = 0.15;

y = x(:,1);  
x = y;

for i=1:length(x)  
   q = x(i)/abs(x(i));
   y(i) = q*(1-exp(a*(q*x(i))));
   y(i) = m * y(i) + (1 - m) * x(i);
end

sound(y,Fs);