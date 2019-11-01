#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>

#define COLOR_LIMIT 40 //Valor decimal limite das cores (entre 0 e 255 -> 40~45 são bons valores)
using namespace std;

/********** ATENÇÃO: UTILIZE ARQUIVOS .BMP EM MODO RGB! (GRAYSCALE AINDA NÃO FUNCIONA) ***************/

/************* Esta versão ignora os 32 bytes iniciais de offset da tela, ou seja, não inclui for para dummy bytes ************************/
unsigned int bytesExtract(char posicao_inicial[], int size){ //Converter bytes little-endian em big-endian e os extrai (no máximo 4 bytes)
	
	unsigned int swap = 0;
	
	//cout << "\\\\\\\\\\\\\\\\\\\\\\ " << sizeof(unsigned int) << endl;
	for(int i = size-1 ;i >= 0  ; i--){ 
			
			swap = (swap << 8) ;
			swap = swap + (posicao_inicial[i] & 0xFF);
	}	
	return swap;
}


int main(){

	char buffer[256];
	unsigned int posicao_pixels = 0;
	unsigned int tam_DIB = 0, tam_file = 0;
	fstream file, saida;
	
	saida.open("saida.txt",ios_base::in | ios_base::out);
	file.open("bittest.bmp",ios_base::in | ios_base::out);
	
	
if(!file.is_open()){
	cout << "Erro ao abrir arquivo bitmap. Finalizando..." << endl;
	system("pause");
}else{
	
	if(!saida.is_open()){
		saida.open("saida.txt",ios_base::out);
		saida.close();
		saida.open("saida.txt",ios_base::in | ios_base::out);
		
		if(!saida.is_open(){
			cout << "Erro ao gerar arquivo de saida. Execute o programa novamente, certificando-se de que o arquivo de saida(\"saida.txt\") anterior tenha sido apagado\n" << endl;
		}
	}
	//cout << "ok \n\n" << endl;
	
	
	file.sync();
	file.seekg(0);
	
	file.read((char*)buffer,14); //Lê os 14 primeiros bytes
	file.sync();
	file.flush();
	
	//Extrai posicao inicial dos pixels
	tam_file = bytesExtract(buffer+2, 4);
	cout << "Tamanho do arquivo: " << tam_file << " bytes" <<  endl;
	//Extrai posicao inicial dos pixels
	
	posicao_pixels = bytesExtract(buffer+10,4);
	cout << "Offset de pixels começa em: " << posicao_pixels << endl;
	file.read((char*)buffer,4); //Lê os 4 bytes seguintes (tam.cabeçalho DIB)
	file.sync();
	
	tam_DIB = posicao_pixels - 14;
	
	cout << "DIB size " << tam_DIB << endl;

	file.seekg(posicao_pixels);
	cout << "POSICAO " << file.tellg() << endl << endl;
	
	
/*************** TRATAMENTO DA IMAGEM *****************************/
	char img_alpha[16][384]={0} ;  //Armazena o bitmap com o canal Alpha
								
								//Armazena o bitmap sem o canal Alpha
	
	char out_img[16][96]= {0}; //bitmap de saída;
	int h,w;
	file.read((char*)img_alpha,6144); //6144 = 96_colunas*4_bytes*16_linhas
	file.sync();
	file.seekg(0);
	
	//leitura de bits
	for(int i=0; i<16; i++){
		for(int j=0; j< 384; j=j+4){
			if(	(((img_alpha[i][j+1] >> 1)&0xFF) > COLOR_LIMIT) || (((img_alpha[i][j+2] >> 1)&0xFF ) > COLOR_LIMIT) || (((img_alpha[i][j+3] >> 1)&0xFF ) > COLOR_LIMIT)){
				//cout << "1";
				out_img[i][j/4] = 1;
			}else{
				//cout << "0";
				out_img[i][j/4] = 0;
			}
		}	
	}
	
	
	
	
	//INVERTER OS BITS (ESTÃO COMENÇANDO DA ESQ. PARA DIR., DE BAIXO P/ CIMA)
	char support[96] = {0};
	
	for(int i = 0; i < 8; i++){
		//Cópia para vetor auxiliar
		for(int j = 0; j < 96; j++){
			support[j] = out_img[i][j];  //Swap
			out_img[i][j] = out_img[15-i][j];
			out_img[15-i][j] = support[j];
		}
	}
	//cout << endl << endl;

	
	char img_final[2][96]={0};
/**************************** PREENCHIMENTO DAS PÁGINAS 0 E 1 *****************************/	
	//preenchimento da página 0;
	
	unsigned int accumulator=0;
	
	for(h = 0; h < 96; h++){
		for(w = 7; w >= 0; w--){
			accumulator = (accumulator << 1) ;
			accumulator = accumulator + (out_img[w][h]);
		}
		img_final[0][h] = accumulator;
		accumulator = 0; //reset no valor;
	}
	
	//preenchimento da página 1;
	accumulator = 0;
	for(h = 0; h < 96; h++){
		for(w = 15; w >= 8; w--){		
			accumulator = (accumulator << 1) ;
			accumulator = accumulator + (out_img[w][h]);

		}		
		img_final[1][h] = accumulator;
		accumulator = 0; //reset no valor;
	}
/**********************************************************/

	
	//SAIDA FINAL
	
	
	saida.seekp(0);
////////////Preenche com modo matricial (maior economia de RAM)
	
	saida << "/******* Modo matricial (compacto) **********/" << endl << endl;
	saida.flush();
	
	saida << "char comandos[192] = {";
	saida.flush();
	
	for(w = 0; w < 2; w++){

		
		for(h = 0; h < 96; h++){
			saida <<"0x" << hex <<(unsigned int)(img_final[w][h] &0xFF);
			saida.flush(); 
			if((w != 1) || (h != 95) ){
				saida <<  ", " ;
				saida.flush();
			}
		}
	}
	saida << "};" << endl << endl;
	saida.flush();
	saida << "int x;" << endl << "for (x = 0; x < 192; x++){" << endl;
	saida.flush();
	saida << "    osd9616_send(0x40,comandos[x]);" << endl << "}" << endl;
	saida.flush();
	
	saida << endl << endl;
	saida.flush();
	



	
	////////////////Preenche com modo estendido (opcional)
   		
	//saida << "/******** Modo estendido ***********/" << endl << endl;
	//saida.flush();
	/********************
	for(w = 0; w < 2; w++){
		// NÃO UTILIZE ESTE BLOCO DE OFFSET
		saida << "for(i = 0; i < 32; i++){" << endl;
		saida.flush();
		saida << "osd9616_send(0x40,0x00); //dummy bytes para os 32 pixels de offset;" << endl;
		saida.flush();
		saida << "} " << endl;
		saida.flush();
		*/
		/***
		for(h = 0; h < 96; h++){
		
		saida << "osd9616_send(0x40,0x" << hex << (int)(img_final[w][h]&0xFF) << ");"<<endl;
		saida.flush();		
		}
		cout << endl;
	}
	*/
	
	saida.close();
	file.close();		
	
	cout << "FIM" << endl;
	}
		
	
return 0;
}
