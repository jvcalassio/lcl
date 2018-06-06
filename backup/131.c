#include <stdio.h>

/*le um vetor*/
void scanvetor (int* vet, int n){
	int i;
	for(i=0; i<n; i++)
		scanf("%d", &vet[i]);
}

/*busca x no vetor 'vet'*/
void busca (int* vet, int n, int x){
	int l=0, r=n-1, m, achou=0;
	while(l<=r && achou==0){
		m=(l+r)/2;
		if(x<vet[m]){
			r=m-1;
		}
		else
			if(x>vet[m]){
				l=m+1;
			}
			else
				achou=1;
	}
	if(achou==1)
		printf("s\n");
	else
		printf("n\n");
}

int main () {
	int n, q, x, i;
	scanf("%d %d", &n, &q);
	int vet[n];
	scanvetor(vet, n);
	for(i=0; i<q; i++){
		scanf("%d", &x);
		busca(vet, n, x);
	}
	return 0;
}