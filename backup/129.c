#include <stdio.h>

long long int posl (long long int vet[], long long int n, long long int x){
	long long int l=0, r=n-1;
	while(l<=r){
		long long int mid = (l+r)/2;
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

long long int posr(long long int vet[], long long int n, long long int x){
	long long int l=0, r=n-1;
	while(l<=r){
		long long int mid = (l+r)/2;
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
	long long int n, q;
	scanf("%lld %lld", &n, &q);
	long long int vet[n], i;
	for(i=0; i<n; i++)
		scanf("%lld", &vet[i]);
	long long int x;
	for(i=0; i<q; i++){
		scanf("%lld", &x);
		printf("%lld\n", posr(vet, n, x) - posl(vet, n, x));
	}
return 0;
}