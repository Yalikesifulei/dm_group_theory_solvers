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

int euler(int n){
	int i,k=0;
	for (i=1; i<=n; i++){
		if (gcd(n,i)==1) k++;
	}
	return k;
}

int search(int x, int arr[], int size){
	int i;
	for (i=0; i<size; i++){
		if (arr[i]==x) return i;
	}
	return -1;
}

void sort(int Array[], int n) {
	int temp, f=1, k=n-2;
	int i;
	while (f) {
		f=0;
		for (i=0; i<=k; i++){
			if (Array[i]>Array[i+1]) {
				temp=Array[i];
				Array[i]=Array[i+1];
				Array[i+1]=temp;
				f=1;
			}
		}
	}
}

int is_equal(int arr_1[], int arr_2[], int n){
	while (--n>0)
    if (arr_1[n]!=arr_2[n]) return 0;
  return 1;
}

int main(){
	int n=0,i=0,j=0,k=0,temp=0;
	int *z_multi=NULL, z_order=0, **z_cycles=NULL, is_cyclic=0;
	printf("Enter n for Z*(n): ");
	scanf("%i", &n);
	printf("\n");
	
	z_order=euler(n);
	
	// generate arrays for Z*(n) and cyclic subgroups
	z_multi = (int *)malloc(z_order*sizeof(int));
	z_cycles = (int **)malloc(z_order*sizeof(int *));
	for (i=0; i<z_order; i++){
		z_cycles[i] = (int *)calloc(z_order, sizeof(int));
	}
	
	// print Z*(n) group
	printf("Z*(%i) = {", n);
	for (i=0, k=1; k<=n; k++){
		if (gcd(n,k)==1) {
			z_multi[i]=k; 
			if (i!=z_order-1) printf("%i,",z_multi[i]); else printf("%i",z_multi[i]); i++;
		}
	}
	printf("}\n"); 
	printf("|Z*(%i)| = %i\n", n, z_order);
	
	// calculate cyclic subgroups
	for (i=0; i<z_order; i++){
		temp=z_multi[i];
		k=0;
		while (temp!=1) {
			z_cycles[i][k] = temp;
			temp=(temp*z_multi[i])%n;
			k++;
		}
		z_cycles[i][k]=1;
	}
	
	// print table with order, inverse and cyclic subgroup for each element of Z*(n)
	printf("\n x | |x| | x^-1 | [x] ");
	printf("\n---|-----|------|---------------------------\n");
	for (i=0; i<z_order; i++){
		// x
		printf("%3i|", z_multi[i]);
		// |x|
		temp=0;
    	for (j=0; j<z_order; j++){
      		if (z_cycles[i][j]!=0) temp++; 
    	}
    	printf("%5i|", temp); if (temp==z_order) is_cyclic=1;
    	// x^-1
    	if (z_order==1 || z_order==2) printf("%6i|", z_cycles[i][0]);
    	for (j=1; j<z_order-1; j++){
    		if (z_cycles[i][z_order-1]!=0) {printf("%6i|", z_cycles[i][z_order-2]); break;} else {
      		if (z_cycles[i][j-1]!=0 && z_cycles[i][j+1]==0) {
      			printf("%6i|", z_cycles[i][j-1]); 
				break;
			}
		}
    	}
    	// [x]
    	printf(" {%i", z_cycles[i][0]);
    	for (j=1; j<z_order; j++){
      		if (z_cycles[i][j]) printf(", %i", z_cycles[i][j]);
    	}
    printf("}\n");
  	}
	if (is_cyclic) printf("\nZ*(%i) is cyclic\n\n", n); else printf("\nZ*(%i) is non-cyclic\n\n", n);
	
	// generator and its subgroup
	int gen=0, gen_index=0, gen_order=0, *subgroup=NULL;
	printf("Enter subgroup generator from Z*(%i): ", n);
	do {
		scanf("%i", &gen); if (search(gen, z_multi, z_order)==-1) printf("Error, generator must belong to Z*(%i): ", n); 
	} while(search(gen, z_multi, z_order)==-1);
	
	gen_index=search(gen, z_multi, z_order);
	
	gen_order=0;
	for (j=0; j<z_order; j++) {
		if (z_cycles[gen_index][j]!=0) gen_order++;
	}
	subgroup=(int *)calloc(gen_order, sizeof(int));
	for (j=0; j<gen_order; j++) {
		subgroup[j]=z_cycles[gen_index][j];
	}
	printf("[%i] = {%i", gen, subgroup[0]);
	for (j=1; j<gen_order; j++) {
		printf(", %i", subgroup[j]);
	}
	printf("}, and there are all cosets:\n");
	
	//generate array for cosets and sorted one
	int **cosets=NULL, **cosets_sorted=NULL;
	cosets = (int **)malloc(z_order*sizeof(int *));
	for (i=0; i<z_order; i++){
		cosets[i] = (int *)calloc(gen_order, sizeof(int));
	}
	cosets_sorted = (int **)malloc(z_order*sizeof(int *));
	for (i=0; i<z_order; i++){
		cosets_sorted[i] = (int *)calloc(gen_order, sizeof(int));
	}
	
	// print cosets table
	printf("\n x | (x) ");
	printf("\n---|"); for (temp=0; temp<4*gen_order; temp++) printf("-"); printf("\n");
	for (i=0; i<z_order; i++){
		cosets[i][0] = (z_multi[i] * subgroup[0])%n;
		cosets_sorted[i][0]=cosets[i][0];
		printf("%3i|{%i", z_multi[i], cosets[i][0]);
		for (j=1; j<gen_order; j++){
			cosets[i][j] = (z_multi[i] * subgroup[j])%n;
			cosets_sorted[i][j]=cosets[i][j];
			printf(", %i", cosets[i][j]);
		}
		printf("}\n");
	}
	printf("\n");
	
	// sort cosets
	for (i=0; i<z_order; i++){
		sort(cosets_sorted[i], gen_order);
	}

	// find uniq cosets
	int uniq_count=0, uniq_cosets=z_order/gen_order;
	for (i=0; i<z_order-1; i++){
		for (k=i+1; k<z_order; k++) {
			if (is_equal(cosets_sorted[i], cosets_sorted[k], gen_order)){
				for (j=0; j<gen_order; j++) cosets_sorted[k][j]=0;
			}
		}
	}
	for (i=0; i<z_order; i++){
		if (cosets_sorted[i][0]!=0) uniq_count++;
	}
	
	// print factor group
	int *cosets_uniq=(int *)malloc(uniq_count*sizeof(int));
	printf("We have %i unique cosets.\n", uniq_count);
	printf("Z*(%i)/[%i] = {", n, gen);
	for (i=0, temp=1; i<z_order; i++){
		if(!cosets_sorted[i][0]) continue;
		if (temp!=uniq_count) {
			printf("(%i), ", z_multi[i]); temp++;
		} else printf("(%i)", z_multi[i]);
	}
	printf("} = {");
	for (i=0, temp=0; i<z_order; i++){
		if(!cosets_sorted[i][0]) continue;
		printf("{%i", cosets_sorted[i][0]);
		cosets_uniq[temp]=z_multi[i];
		for (j=1; j<gen_order; j++){
			printf(", %i", cosets_sorted[i][j]);
		} temp++;
		if (temp!=uniq_count) printf("}, "); else printf("}}\n");
	}
	
	// generate array for Cayley table
	int **cayley_table=NULL;
	cayley_table = (int **)malloc((uniq_count+1)*sizeof(int *));
	for (i=0; i<(uniq_count+1); i++){
		cayley_table[i] = (int *)calloc((uniq_count+1), sizeof(int));
	}; cayley_table[0][0]=0;
	for (i=1; i<(uniq_count+1); i++){
		cayley_table[i][0]=cosets_uniq[i-1];
		cayley_table[0][i]=cosets_uniq[i-1];
	}
	
	for (i=1; i<(uniq_count+1); i++){
		for (j=1; j<(uniq_count+1); j++){
			temp = search((cayley_table[i][0] * cayley_table[0][j])%n, z_multi, z_order); //temp is number of element in Z*(n)
			sort(cosets[temp], gen_order);
			for(k=0; k<z_order; k++){
				if (is_equal(cosets[temp], cosets_sorted[k], gen_order)) cayley_table[i][j]=z_multi[k];
			}
		}
	}
	
	// print Cayley table
	printf("\nCayley table for Z*(%i)/[%i]:\n", n, gen);
	printf(" * "); 
	for (j=1; j<(uniq_count+1); j++){
			printf("%3i", cayley_table[0][j]);
	} printf("\n");
	for (i=1; i<(uniq_count+1); i++){
		for (j=0; j<(uniq_count+1); j++){
			printf("%3i", cayley_table[i][j]);
		}
		printf("\n");
	}
	
	int **cosets_cycles=NULL;
	cosets_cycles = (int **)malloc(uniq_count*sizeof(int *));
	for (i=0; i<uniq_count; i++){
		cosets_cycles[i] = (int *)calloc(uniq_count, sizeof(int));
	}
	
	for (i=0; i<uniq_count; i++){
		temp=cosets_uniq[i];
		k=0;
		while (temp!=1) {
			cosets_cycles[i][k] = temp;
			temp = cayley_table[i+1][search(temp, cosets_uniq, uniq_count)+1];
			k++;
		}
		cosets_cycles[i][k]=1;
	}
	
	// print uniq cosets and their cyclic subgroups
	is_cyclic=0;
	printf("\n (x) | [(x)] ");
	printf("\n-----|---------------\n");
	for (i=0; i<uniq_count; i++){
		// x
		printf("%3i  |", cosets_uniq[i]);
		// |x|
		temp=0;
    	for (j=0; j<uniq_count; j++){
      		if (cosets_cycles[i][j]!=0) temp++; 
    	}
    	if (temp==uniq_count) is_cyclic=1;
    	// [x]
    	printf(" {%i", cosets_cycles[i][0]);
    	for (j=1; j<uniq_count; j++){
      		if (cosets_cycles[i][j]) printf(", %i", cosets_cycles[i][j]);
    	}
    printf("}\n");
  	}
	if (is_cyclic) printf("\nZ*(%i)/[%i] is cyclic\n\n", n, gen); else printf("\nZ*(%i)/[%i] is non-cyclic\n\n", n, gen);
	
	system("pause");
	
	// free memory
	free(z_multi); free(subgroup); free(cosets_uniq);
	for (i=0; i<z_order; i++){
		free(z_cycles[i]);
		free(cosets[i]);
		free(cosets_sorted[i]);
	}
	free(z_cycles); free(cosets); free(cosets_sorted);
	for (i=0; i<uniq_count; i++){
		free(cosets_cycles[i]);
	}	
	free(cosets_cycles);
	for (i=0; i<(uniq_count+1); i++){
		free(cayley_table[i]);
	}	
	free(cayley_table);
	
	return 0;
}

// Made by Oleksii Galganov, IASA KA-81, 7-8.02.2019
