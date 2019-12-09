Neste código, foi feito um port do script de MATLAB, exceto pelo sinal usado (áudio ia ocupar muita memória). Ao invés disso,
utilizou-se amostras geradas pelo MATLAB de dois sinais:
-> Sinal sem distorção: 0.03*cos(2 * pi * fg * Ts  *n);
-> Sinal com distorção: 0.03*cos(2 * pi * fg * Ts  *n) + 0.03*cos(2 * pi * 2 * fg * Ts  *n);

onde fg = 100 Hz (harmonica fundamental);Fs = 512 Hz; Ts = 1/512 ; comprimento L = 512 amostras (para ter 1 hertz/amostra);

Mais detalhes se encontram no Relatório Geral.
