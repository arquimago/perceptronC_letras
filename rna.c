#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define LEARNING_RATE    1.0
#define MAX_ITERATION    10000
#define EXEMPLOS 21

float peso[7][9][7];
float bias[7];

float randomFloat(){
    //return (float)rand() / (float)RAND_MAX;
	return 0.0;
}

void calculaSaida(int entrada[9][7], int saida[7]){
    int i,j,k;
	float soma;
	
	for(i=0;i<7;i++){
		soma=0.0;
		for(j=0;j<9;j++){
			for(k=0;k<7;k++){
				soma+=entrada[j][k]*peso[i][j][k];
			}
		}
		soma+=bias[i];
		if(soma>=0) saida[i] = 1;
		else saida[i] = 0;
	}
}

void letraTarget(char letra, int *target){
	switch(letra){
		case 'a':
			target[0]=1;
			break;
		case 'b':
			target[1]=1;
			break;
		case 'c':
			target[2]=1;
			break;
		case 'd':
			target[3]=1;
			break;
		case 'e':
			target[4]=1;
			break;
		case 'j':
			target[5]=1;
			break;
		case 'k':
			target[6]=1;
			break;
		default:
			printf("ERRO!\n");
			break;
	}
}

void targetLetra(int *target){
	int i;
	for(i=0;i<7;i++){
		if(!target[i])
			printf(". ");
		else {
			switch(i){
				case 0:
					printf("a ");
					break;
				case 1:
					printf("b ");
					break;
				case 2:
					printf("c ");
					break;
				case 3:
					printf("d ");
					break;
				case 4:
					printf("e ");
					break;
				case 5:
					printf("j ");
					break;
				case 6:
					printf("k ");
					break;
				default:
					printf("ERRO!\n");
					break;
			}
		}
	}
	printf("\n");
}


int main(int argc, char* argv[]){
	srand(time(NULL));
	int entrada[EXEMPLOS][9][7];
	int target[EXEMPLOS][7];
	int teste[9][7];
	int saida[7];
	int i,j,k,l,epoca;
	char letra;	
	float erroLocal[7], erroGlobal;
	//captura entrada de treinamento
	FILE* arqTreino = fopen(argv[1], "r");
	FILE* arqTestes = fopen(argv[2], "r");
	if(arqTreino==NULL){
		printf("Falha no acesso ao arquivo de treinamento");
		exit(42);
	}
	printf("Arquivo de treinamento carregado!\n");
	memset(target,0,sizeof(target));
	for(i=0;i<EXEMPLOS;i++){
		fscanf(arqTreino," %c",&letra);
		letraTarget(letra,target[i]);
		printf("Target Carregado %d: ", i+1);
		targetLetra(target[i]);
		for(j=0;j<9;j++){
			for(k=0;k<7;k++){
				fscanf(arqTreino,"%d",&entrada[i][j][k]);
				//printf("%d ", entrada[i][j][k]);
			}
			//printf("\n");
		}
	}
	printf("Arquivo de treinamento copiado pra memoria!\n");
	system("pause");
	//inicia pesos aleatórios
	for(i=0;i<7;i++){
		for(j=0;j<9;j++){
			for(k=0;k<7;k++){
				peso[i][j][k] = randomFloat();
			}
		}
		bias[i] = randomFloat();
	}
	//treina a rede
	epoca=0;
	do{
		epoca++;
		erroGlobal=0;
		printf("Epoca %d\n", epoca);
		for(i=0;i<EXEMPLOS;i++){
			calculaSaida(entrada[i],saida);
			for(j=0;j<7;j++) {
				erroLocal[j] = target[i][j]-saida[j];
				erroGlobal += (erroLocal[j]*erroLocal[j]);
			}
			for(j=0;j<7;j++){
				for(k=0;k<9;k++){
					for(l=0;l<7;l++){
						peso[j][k][l] += (float)LEARNING_RATE * erroLocal[j] * entrada[i][k][l];
					}
				}
				bias[j] += (float)LEARNING_RATE * erroLocal[j];
			}
		}
		
	}while(erroGlobal != 0 && epoca <= MAX_ITERATION);
	printf("treinada!\n");
	//testa a rede
	fclose(arqTreino);
	if(arqTestes==NULL){
		printf("Falha no acesso ao arquivo de testes");
		exit(51);
	}
	
	for(i=0;i<EXEMPLOS;i++){
		fscanf(arqTestes," %c",&letra);
		//printf("%c\n", letra);
		for(j=0;j<9;j++){
			for(k=0;k<7;k++){
				fscanf(arqTestes,"%d",&teste[j][k]);
				//printf("%d ", teste[j][k]);
			}
			//printf("\n");
		}
		calculaSaida(teste,saida);
		printf("Para o teste %d resultado: ", i);
		targetLetra(saida);
	}
	fclose(arqTestes);
	return 0;
}