# Processador multiefeitos para áudio

## Introdução:
  Os módulos embarcados DSPs podem ser utilizados no tratamento de áudio seja de voz ou na alteração de sinais para gerar efeitos não inerentes do Sinal. Tal trabalho tem como objetivo reproduzir na arquitetura TMS320C55x efeitos utilizados em guitarras elétricas.  O principal objeto de estudo nesse processo de desenvolvimento se fundamenta no processamento digital de audio e à utilização de componentes presentes no kit de desenvolvimento que ajudam neste processo. 
 
  O estudo levou em consideração a implementação inicial em matlab ou python dos modelos dos efeitos como filtros digitais aplicados à amostras de áudio no formato “.wav” com a finalidade de se verificar a funcionalidade dos mesmos. Tais amostras geralmentes possuem taxa de amostragem de 44 kHz e com resolução de 16 bits.
  
Classificação de Efeitos de Áudio:
* Os efeitos aplicados em sinais de áudio podem ser classificados como:
* Filtros Básicos: Passa Baixas, Passa Altas, Equalizadores
* Filtros de tempo Variável: Wah-Wah, Phaser.
* Atrasadores: Vibrato, Flanger, Chorus, Echo
* Moduladores: modulador Ring, Tremolo, Vibrato
* Não-lineares: Compressão, Limitadores, Distorções, Excitações
* Efeitos Espaciais: Panning, Reverb, Surround Sound
