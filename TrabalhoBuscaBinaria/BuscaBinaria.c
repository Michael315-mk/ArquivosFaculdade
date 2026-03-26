#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2]; // Ao Espaço no final da linha + quebra de linha
};


int main(int argc, char** argv){
    Endereco lugar;
    FILE* arquivo;
    int total_bytes, total_elementos;
    int valor_strncmp;
    int inicio, fim, meio;
    int contador_de_acessos = 0;

    inicio = 0;


    // Vendo o número de parâmetros passados
    if(argc != 2){
        fprintf(stderr, "argumentos inválidos");
        return 0;
    }

    // Conferindo o arquivo aberto
    if ((arquivo = fopen("cep_ordenado.dat", "rb")) == NULL){
        fprintf(stderr, "Arquivo não foi aberto");
        return 0;
    }

    fseek(arquivo, 0, SEEK_END);
    total_bytes = ftell(arquivo);
    total_elementos = total_bytes / sizeof(Endereco);

    fim = total_elementos - 1;
    while(inicio <= fim){
        
        meio = (inicio + fim)/2;
        fseek(arquivo, meio * sizeof(Endereco), SEEK_SET);

        fread(&lugar, sizeof(Endereco), 1, arquivo);
        contador_de_acessos++;

        valor_strncmp = strncmp(argv[1],lugar.cep,8);

        if(valor_strncmp == 0)
		{
			printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",lugar.logradouro,lugar.bairro,lugar.cidade,lugar.uf,lugar.sigla,lugar.cep);
			break;
		}
        else if (valor_strncmp > 0){
            inicio = meio + 1; 
        }
        else{
            fim = meio - 1;
        }
    }
    printf("Total de acessos: %d", contador_de_acessos);
    

    fclose(arquivo);
    return 0;
}