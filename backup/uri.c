#include <stdio.h>
int main (){
	double sal, imposto, x;
	scanf("%lf", &sal);
	if(sal<=2000)
		printf("Isento\n");
	else if(sal>2000 && sal<=3000){
		x = sal-2000;
		imposto = x*8/100;
		printf("R$ %.2lf\n", imposto);
	}
	else if(sal>3000 && sal<=4500){
		x = sal-3000;
		imposto += x*18/100;
		printf("R$ %.2lf\n", imposto+80);
	}
	else if(sal>4500){
		x = sal-4500;
		imposto = x*28/100;
		printf("R$ %.2lf\n", imposto+350);
	}
return 0;
}
