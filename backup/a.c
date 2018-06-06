#include <stdio.h>
typedef struct{
	char nome[30];
	float altura;
	int idade;
} pessoa;

int main () {
	int n = 3;
	pessoa gnt[n];
	int i;
	for(i=0; i<3; i++){
		printf("o nome da %d pessoa é: ", i+1);
		scanf("%s", gnt[i].nome);
		printf("a altura da %d pessoa é: ", i+1);
		scanf("%f", &gnt[i].altura);
		printf("a idade da %d pessoa é: ", i+1);
		scanf("%d", &gnt[i].idade);
		printf("\n"); 
	}
	for(i=0; i<3; i++){
		printf("\n");
		printf("\nRegistro feito.\n");
		printf("a altura do(a) %s é: %f\n", gnt[i].nome, gnt[i].altura);
		printf("a idade do(a) %s é: %d\n", gnt[i].nome, gnt[i].idade);
	}
return 0;
}