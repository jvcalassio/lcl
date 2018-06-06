#include <stdio.h>

void LerVetor (long long vet[], long long n){
	long long i;
	for(i=0; i<n; i++){
		scanf("%lld", &vet[i]);
	}
}

long long Qtdx(long long vet[], long long n, long long x){
	long long l=0, r=n-1, m=0, aux=1;
	while(aux=1){
		m=(l+r)/2;
		if(vet[m]<x){
			l=m+1;
		}
		else if(vet[m]>x){
			r=m-1;
		}
		else if(vet[m]==x){
			if(vet[m]==vet[m-1]){
				l=(l+m)/2;
			}
			if(vet[m]==vet[m+1]){
				r=(r+m)/2;
			}
		}

	}
return r-l+1;
}

int main () {
	long long n, q, x, i;
	scanf("%lld %lld", &n, &q);
	long long a[n];
	LerVetor(a, n);
	for(i=0; i<q; i++){
		scanf("%lld", &x);
		printf("%lld\n", Qtdx(a, n, x));
	}
return 0;
}