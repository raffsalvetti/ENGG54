% esse script gera um arquivo cabecalho para ser adicionado ao projeto do
% dsp

% lendo amostra de audio
[q,Fs] = audioread('./sample2.wav'); 

% mostra estatisticas da amostra de audio
fprintf('Freq. Amostragem: %fHz; T. Amostragem: %fs; Amostras: %i;\n', Fs, 1/Fs, length(q));

% tocando o audio
sound(q, 44100); 

% a potencia representa a resolucao da parte flutuante que se quer, 
% ex K=0,0123456789 (10 bits depois da virgula) assim 
% K*2^10 = K * 1024 = 12,641975194; trucando o resultado temos 12 (que 
% representa K em pnto fixo);
X = 2^15; 

% selecionando apenas um canal (transforma a amostra em mono
q = q(:,1); 

% converte o array de amostras para ponto fixo
q = q.*X;   

% abre arquivo para escrita
fp = fopen('samples.h','w','n','UTF-8');

% tamanho da pagina de memoria do dsp
PAGE_SIZE = 32*1024;

% escrevendo arquivo head
fprintf(fp, '#ifndef __SAMPLES_H__\n');
fprintf(fp, '#define __SAMPLES_H__\n');
fprintf(fp, '#define FREQUENCY_SAMPLE %d\n', floor(Fs));
fprintf(fp, '#define N_SAMPLES %d\n', PAGE_SIZE);
arrays = floor(length(q) / PAGE_SIZE);
fprintf(fp, '#define NN_SAMPLES %d\n\n', arrays);
idx = 1;
var_name = {};
for c = 1:arrays
    var_name{c} = strcat('samples_', int2str(c - 1));
    qq = q(idx:idx + PAGE_SIZE - 2);
    fprintf(fp, 'const Int16 samples_%d[N_SAMPLES] = {0', c - 1);
    %fprintf(fp, ', 0x%04X', typecast(int16(round(q)), 'uint16'));
    fprintf(fp, ', %d', round(qq));
    fprintf(fp, '};\n\n');
    idx = idx + PAGE_SIZE;
end
fprintf(fp, 'const Int16 *samples[NN_SAMPLES] = {%s};\n', strjoin(var_name, ', '));
fprintf(fp, '#endif\n');
