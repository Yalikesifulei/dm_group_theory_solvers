#include<stdio.h>
#include<stdlib.h>

int gcd(int m, int n){
	if (n==0) return m;
	int r=m%n;
	while (r!=0) {
		m=n; n=r; r=m%n;
	}
	return n;
}

int main(){
	int n=0, m=0, *z_n=NULL, *z_m=NULL;
	printf("Enter n and m (Z+(n)->Z+(m)): ");
	scanf("%i %i", &n, &m);
	
	// generate and print arrays for groups
	z_n = (int *)calloc(n, sizeof(int));
	z_m = (int *)calloc(m, sizeof(int));
	int i=0, j=0;
	printf("Z+(%i) = {", n);
	for (i=0; i<n; i++){
		z_n[i]=i;
		if (i!=n-1) printf("%i,",z_n[i]); else printf("%i",z_n[i]);
	} printf("}\n"); 
	printf("Z+(%i) = {", m);
	for (i=0; i<m; i++){
		z_m[i]=i;
		if (i!=m-1) printf("%i,",z_m[i]); else printf("%i",z_m[i]);
	} printf("}\n"); 
	
	// find homos and fill homos humbers array
	int homos_count=0, *homos=NULL, **all_homos=NULL;
	for (i=0; i<m; i++){
		if ((n*i)%m==0) homos_count++;
	}
	homos = (int *)calloc(homos_count, sizeof(int));
	homos_count=0;
	for (i=0; i<m; i++){
		if ((n*i)%m==0) {
			homos[homos_count]=i;
			homos_count++;
		}
	}
	printf("\nThere are %i homomorphisms Z+(%i)->Z+(%i)\n", homos_count, n, m);
	// generate homos table array
	all_homos = (int **)malloc(n*sizeof(int *));
	for (i=0; i<n; i++){
		all_homos[i] = (int *)calloc(homos_count, sizeof(int));
	}
	
	printf(" j |");
	for (i=0; i<homos_count; i++){
		if (i!=(homos_count-1)) printf("f%i(j)|", homos[i]); else printf("f%i(j)", homos[i]);
	}
	printf("\n---|");
	for (i=0; i<homos_count; i++){
		if (i!=(homos_count-1)) printf("-----|", homos[i]); else printf("-----", homos[i]);
	}
	printf("\n");
	for (i=0; i<n; i++){
		printf("%3i|", z_n[i]);
    	for (j=0; j<homos_count; j++) {
    		all_homos[i][j] = (z_n[i]*homos[j])%m;
     		if (j!=(homos_count-1)) printf("%5i|", all_homos[i][j]); else printf("%5i ", all_homos[i][j]);
    	}
    	printf("\n");
  	}
  	
	system("pause");
	
	// free memory
	free(z_n); free(z_m); free(homos);
	for (i=0; i<n; i++){
		free(all_homos[i]);
	}
	free(all_homos);
	
	return 0;
}

// Made by Oleksii Galganov, IASA KA-81, 8.02.2019
