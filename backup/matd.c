#include <stdio.h>

int posl (int vet[], int n, int x){
	int l=0, r=n-1;
	while(l<=r){
		int mid = (l+r)/2;
		if(vet[mid]>x)
			r=mid-1;
		else if(vet[mid]<x)
			l=mid+1;
		else if(vet[mid]==x){
			if(mid==0 || vet[mid-1]!=x)
				return mid;
			else if(vet[mid-1]==x)
				r=mid-1;
		}
	}
	return 0;
}

int posr(int vet[], int n, int x){
	int l=0, r=n-1;
	while(l<=r){
		int mid = (l+r)/2;
		if(vet[mid]>x)
			r=mid-1;
		else if(vet[mid]<x)
			l=mid+1;
		else if(vet[mid]==x){
			if(mid==n-1 || vet[mid+1]!=x)
				return mid+1;
			else if(vet[mid+1]==x)
				l=mid+1;
		}
	}
	return 0;
}

int main (){
	int n, q;
	scanf("%d %d", &n, &q);
	int vet[n], i;
	for(i=0; i<n; i++)
		scanf("%d", &vet[i]);
	int x;
	for(i=0; i<q; i++){
		scanf("%d", &x);
		printf("%d\n", posr(vet, n, x) - posl(vet, n, x));
	}
return 0;
}