function [y, t]=mysine(freq, ts, T)

t=0:ts:T;
y=sin(2*pi*freq*t);

end