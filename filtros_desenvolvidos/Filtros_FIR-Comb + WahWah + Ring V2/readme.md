Os filtros desenvolvidos estão dentro do arquivo aic3204_tone_headphone.c
Os demais arquivos foram upados por causa que esse projeto foi desenvolvido em cima do gabarito de testes da Spectrum Digital

Configs utilizadas:
Frequencia amostragem: 44,1 kHz
Obtenção do áudio: através do jack p2 de input
Volume de saída: - 5dB (0.0 dB já era bem alto pros ouvidos)

existem 4 filtros + saída normal no projeto, onde os filtros são:

- Filtro atrasador + atenuador (0.5 + 0.25*e^(-5jw) + 0.125*e^(-10jw) (filtro = 1) -> Led azul somente
- Saída normal (filtro = 1) -> Led Amarelo somente
- FIR Comb (Atrasador de 10 amostras -> 0.5 + 0.125*e^(-10jw)) (filtro = 2) -> Led vermelho somente
- Filtro Ring (filtro = 3) -> Leds azul e amarelo
- WahWah (filtro = 4) -> Leds azul e vermelho
