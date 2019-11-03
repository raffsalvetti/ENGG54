# Programa de teste Ethernet

O kit de desenvolvimento possui um interface de rede RJ45 e um controlador ethernet embutido que pode ser configurado para
operar nos modos de auto-negociação (garantindo funcionamento com a maioria dos dispositivos de rede com
qualquer tipo de hub ou switch). É possível ainda usar um cabo do tipo *cross-over* para ligar o kit diretamente a um comutador (demora um pouco para configurar a rede nesse modo de operação).   

Os possíveis tipos de configuração são:
 - FDX_100 (Full-duplex 100Mbits)
 - FDX_10 (Full-duplex 10Mbits)
 - HDX_100 (Half-duplex 100Mbits)
 - HDX_10 (Half-duplex 10Mbits)
 - AUTO_NEG (Auto-negociação)

É possível usar as configurações de IP estático e dinâmico (DHCP).  

O processador se comunica com o controlador ethernet através de uma região de memoria mapeada, conforme explicado [aqui](https://github.com/raffsalvetti/ENGG54/wiki/Mapa-de-Mem%C3%B3ria), então a configuração do controlador bem como os dados que trafegam através do controlador são transmitidas por uma área de memória.

O exemplo a seguir foi extraído do pacote de exemplos encontrado [aqui](http://support.spectrumdigital.com/boards/ezdsp5502/revc/files/ezdsp5502_BSL_RevC.zip).
E um programa de teste codificado em C que demonstra a troca simples de frames ethernet.
O programa foi modificado para apresentar os status de teste com sucesso, falha e um estado intermediário (aguardando um link de dados) com os leds vermelho, verde e laranja respectivamente, pois a depuração não foi possível usando a função printf.

Um programa de teste auxiliar, que roda num computador, foi usado para escutar os pacotes enviados em broadcast pelo DSP e envia-los em boradcast novamente (echo server) na rede.
O programa auxiliar foi construído a partir de uma modificação com base nos programas [sendRawEth.c](https://gist.github.com/austinmarton/1922600) e [recvRawEth.c](https://gist.github.com/austinmarton/2862515).

Vale lembrar que:
 - O cabeçalho ezdsp5502_i2cgpio.h foi adicionado ao projeto do DSP para usar os LEDs indicadores.
 - a função EZDSP5502_I2CGPIO_writeLine(LEDX, NIVEL) é usada para ligar ou desligar os LEDs
 - NIVEL pode ser HIGH ou LOW
 - O nível de tensão HIGH significa DESLIGAR o LED
 - O nível de tensão LOW significa LIGAR o LED
 - LED0 = AZUL
 - LED1 = LARANJA
 - LED2 = VERMELHO
 - LED3 = VERDE
 - As chamadas da função printf foram comentadas no código

Abaixo são mostrados alguns dos blocos de código adicionados ou modificados.

	//em main.c foi acrescentado esse código
	//Configura os pinos dos leds como saida
	EZDSP5502_I2CGPIO_configLine(  LED0, OUT );
	EZDSP5502_I2CGPIO_configLine(  LED1, OUT );
	EZDSP5502_I2CGPIO_configLine(  LED2, OUT );
	EZDSP5502_I2CGPIO_configLine(  LED3, OUT );

	//desligar todos os leds
    EZDSP5502_I2CGPIO_writeLine(   LED0, HIGH );
    EZDSP5502_I2CGPIO_writeLine(   LED1, HIGH );
    EZDSP5502_I2CGPIO_writeLine(   LED2, HIGH );
    EZDSP5502_I2CGPIO_writeLine(   LED3, HIGH );

	// o Bloco abaixo foi modificado como segue
	/* Check for test fail */
	if ( TestFail != 0 ) {
	/* Print error message */
	//printf( "     FAIL... error code %d... quitting\n", TestFail );
		EZDSP5502_I2CGPIO_writeLine(   LED2, LOW );
	} else {
		/* Print pass message */
		//printf( "    PASS\n" );
		//printf( "\n***ALL Tests Passed***\n" );
		EZDSP5502_I2CGPIO_writeLine(   LED3, LOW );
	}

Para modificar os parâmetros do módulo Ethernet, é necessário usar a seguinte função: enet_regwrite(REGISTRADOR, VALOR).
Os registradores e suas funções são encontrados no manual do kit.
Para o programa de teste, a configuração foi a seguinte:

	enet_regwrite(0x70, 0x0000);  // Disable transmit
    enet_regwrite(0x70, 0x0010);  // Flush transmit queue
    enet_regwrite(0x70, 0x0007);  // TXCR - Configure auto-CRC on transmit, transmit padding, transmit enable
    enet_regwrite(0x84, 0x4000);  // TXFDPR - Enable auto increment of data pointer
    enet_regwrite(0x92, 0xffff);  // ISR - Clear all interrupts
    enet_regwrite(0x90, 0xffff);  // IER - Enable transmit/receive interrupts
    enet_regwrite(0x80, 0x0004);  // Transmit (0x04 for auto enqueue)
    enet_regwrite(0x74, 0x0013);  // RXCR1 receive enabled, promiscuous mode
    enet_regwrite(0x76, 0x0084);  // RXCR2 SPI receive single frame data burst
    enet_regwrite(0x9c, 0x0001);  // Interrupt after 1 frame
    enet_regwrite(0x82, 0x0030);  // RXQCR enable receive threshold, auto-dequeue

# Funcionamento do programa

Na função **main** é chamada a função **enet_test**, que configura o módulo ethernet com os parâmetros mencionados acima. Logo apos, envia para a rede um array de bytes chamado *testpacket*, visualizado na imagem seguinte, contendo um frame ethernet com os endereços mac de origem e destino (boradcast ff:ff:ff:ff:ff), através da função **enet_txpacket**, que recebe o array e seu tamanho como parâmetros.

![testpacket](dadsdas)  
Array contendo informações do frame ethernet.  
- Na cor verde, está a configuração do MAC destino, no caso boradcast.  
- Em laranja, está o MAC do kit de desenvolvimento.  
- Em roxo, está o Flag ethertype, que identifica o tipo de frame, para esse teste, foi usado um valor não padrão.

Depois de enviar o pacote, o programa entra num loop, *while((enet_regread(0x92) & 0x2000) == 0)*, e aguarda receber o mesmo pacote que enviou por broadcast. Se o pacote tem o mesmo tamanho, o LED verde ascende, caso contrario o led vermelho ascende e o teste é encerrado.

![tcpdump](dadsdas)  
Nessa imagem é mostrado os dois frames passando pelo log do tcpdump, um programa muito usado para depurar e verificar os pacotes que passam por uma interface de rede.  
O primeiro grupo, com os marcadores, é o frame de teste enviado pelo kit de desenvolvimento. O segundo grupo é um *echo* do frame enviado pelo programa de teste auxiliar que pode ser encontrado [aqui]().


# Dica

Quando o programa era carregado na placa pelo programador embutido no CCS, de forma intermitente, aparecia uma mensagem de erro no console como mostado na imagem.  

![Erro_Console_CCS](dadsdas)  

Se isso acontecer, execute o seguinte procedimento:
 - Clique com o botão direito no projeto e abra a janela de propriedade
 - Selecione a opção **General** no menu lateral esquerdo e marque as opções que aparecem em verde, mostrado na imagem que segue.
 - Clique em "Verify..." e espere o CCS se comunicar com o kit
 - Clique em "Apply and Close"  

![General_Config](dadsdas)  
Configurações

![Verify...](dadsdas)  
Verificando conexão com a placa
