[x,Fs] = audioread('./sample2.wav'); 

delay = 0.12; % em segundos (valor bem pequeno por causa do feedback)
alpha = 0.8; % volume
beta = 0.00015; % ganho do feedback
D = delay*Fs;  
y = x(:,1);
x = y;
for i=1:length(x)  
    if (i > D)
        k = y(i-D);
    else
        k = 0;
    end
    y(i) = alpha * x(i) - (beta * D) * k;
end  

sound(y,Fs);