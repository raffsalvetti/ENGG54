[x,Fs] = audioread('./sample2.wav'); 

delay = 0.5; % em segundos
alpha = 0.65; % echo strength  
D = delay*Fs;  
y = zeros(size(x));  
y(1:D) = x(1:D);  
   
for i=D+1:length(x)  
  y(i) = x(i) + alpha*x(i-D);  
end  

sound(y,Fs);