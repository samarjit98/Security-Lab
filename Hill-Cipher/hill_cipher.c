#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encrypt(int *block, int **key, int n){
	int *block_cpy;
	block_cpy = (int*)malloc(n*sizeof(int));

	for(int i=0; i<n; i++)block_cpy[i] = block[i];

	for(int i=0; i<n; i++){
		int elem = 0;
		for(int j=0; j<n; j++){
			elem += block_cpy[j] * key[i][j];
		}
		block[i] = elem % 26;
	}
}

int det(int **A, int n){
	if(n==1)return A[0][0];
	if(n==2){
		return A[0][0]*A[1][1] - A[0][1]*A[1][0];
	}

	int val = 0;

	int sgn = 1;
	for(int i=0; i<n; i++){
		int **C;
		C = (int**)malloc((n-1)*sizeof(int*));
		for(int ii=0; ii<n; ii++)C[ii] = (int*)malloc((n-1)*sizeof(int));

		int ai=1, aj=0, ci=0, cj=0;
		while(ci<(n-1)){
			aj = 0; cj = 0;
			while(cj<(n-1)){
				if(aj == i)aj++;
				C[ci][cj] = A[ai][aj];
				cj++; aj++;
			}
			ai++; ci++;
		}

		val += sgn * A[0][i] * det(C, n-1);
		sgn = -sgn;
	}

	return val;
}

void transpose(int **A, int **A_t, int n){
	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
			A_t[i][j] = A[j][i];
}

void adjoint(int **A, int **adj_A, int n){
	int sgn = 1;
	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++){
			int **C;
			C = (int**)malloc((n-1)*sizeof(int*));
			for(int ii=0; ii<n; ii++)C[ii] = (int*)malloc((n-1)*sizeof(int));

			int ai=0, aj=0, ci=0, cj=0; 
			while(ci<(n-1)){
				if(ai==i)ai++;
				aj = 0; cj = 0;
				while(cj<(n-1)){
					if(aj == j)aj++;
					C[ci][cj] = A[ai][aj];
					cj++; aj++;
				}
				ai++; ci++;
			}

			adj_A[i][j] = det(C, n-1) * sgn;
			sgn = -sgn;
		}

	int **adj_A_t;
	adj_A_t = (int**)malloc(n*sizeof(int*));
	for(int i=0; i<n; i++)adj_A_t[i] = (int*)malloc(n*sizeof(int));
	transpose(adj_A, adj_A_t, n);

	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
			adj_A[i][j] = adj_A_t[i][j];
}

int gcd_e(int a, int b, int *x, int *y){ 
    if (a == 0) { 
        *x = 0, *y = 1; 
        return b; 
    } 
  
    int x1, y1; 
    int gcd = gcd_e(b%a, a, &x1, &y1); 
 
    *x = y1 - (b/a) * x1; 
    *y = x1; 
  
    return gcd; 
} 

int mod_inverse(int a, int m){ 
    int x, y; 
    int g = gcd_e(a, m, &x, &y); 
    int res = (x%m + m) % m; 
    return res;
} 

void invert(int **key, int **key_inv, int n){
	int **adj_key;
	int det_key = det(key, n) % 26;
	det_key = mod_inverse(det_key, 26);

	adj_key = (int**)malloc(n*sizeof(int*));
	for(int i=0; i<n; i++)adj_key[i] = (int*)malloc(n*sizeof(int));
	adjoint(key, adj_key, n);

	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++){
			key_inv[i][j] = adj_key[i][j] % 26;
			if(key_inv[i][j] < 0)key_inv[i][j] += 26;
		}
	
	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++){
			key_inv[i][j] = (key_inv[i][j]*det_key)%26;	
			if(key_inv[i][j] < 0)key_inv[i][j] += 26;
		}
	
}

void decrypt(int *block, int **key, int n){
	int *block_cpy;
	block_cpy = (int*)malloc(n*sizeof(int));

	int **key_inv;
	key_inv = (int**)malloc(n*sizeof(int*));
	for(int i=0; i<n; i++)key_inv[i] = (int*)malloc(n*sizeof(int));

	invert(key, key_inv, n);
	/*
	printf("Inverse Key:\n");

	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++)printf("%d ", key_inv[i][j]);
		printf("\n");
	}
	*/

	for(int i=0; i<n; i++)block_cpy[i] = block[i];

	for(int i=0; i<n; i++){
		int elem = 0;
		for(int j=0; j<n; j++){
			elem += block_cpy[j] * key_inv[i][j];
		}
		block[i] = elem % 26;
	}
}

int main(int argc, char* argv[]){
	
	int **key;
	int n;

	printf("Enter key size:");
	scanf("%d", &n);

	key = (int**)malloc(n*sizeof(int*));
	for(int i=0; i<n; i++)key[i] = (int*)malloc(n*sizeof(int));

	printf("Enter key:");
	char key_buff[100];
	scanf("%s", key_buff);

	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
			key[i][j] = key_buff[i*n + j] - 'A';

	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++)
			printf("%d ", key[i][j]);
		printf("\n");
	}
			

	char ans = 'y';

	while(ans=='y'){
		char buff[100];
		printf("Enter text:");
		scanf("%s", buff);

		int **blocks;
		int k = strlen(buff);
		int num_blocks;
		if(k%n == 0)num_blocks = k/n;
		else num_blocks = k/n + 1;

		blocks = (int**)malloc(num_blocks*sizeof(int*));
		for(int i=0; i<num_blocks; i++)blocks[i] = (int*)malloc(n*sizeof(int));

		for(int i=0; i<num_blocks; i++)
			for(int j=0; j<n; j++)
				if(i*n + j < k)blocks[i][j] = buff[i*n + j] - 'A';
				else blocks[i][j] = 0;

		printf("\nEncrypted Text:\n");

		for(int i=0; i<num_blocks; i++){
			for(int j=0; j<n; j++)printf("%c", blocks[i][j] + 'A');
			printf(" ----> ");
			encrypt(blocks[i], key, n);
			for(int j=0; j<n; j++)printf("%c", blocks[i][j] + 'A');
			printf("\n");
		}

		printf("\nDecrypted Text:\n");

		for(int i=0; i<num_blocks; i++){
			for(int j=0; j<n; j++)printf("%c", blocks[i][j] + 'A');
			printf(" ----> ");
			decrypt(blocks[i], key, n);
			for(int j=0; j<n; j++)printf("%c", blocks[i][j] + 'A');
			printf("\n");
		}

		scanf("%c", &ans);
	}

	return 0;
}

/*

➜  Hill-Cipher git:(master) ✗ ./a.out          
Enter key size:3
Enter key:GYBNQKURP
6 24 1 
13 16 10 
20 17 15 
Enter text:ACT

Encrypted Text:
ACT ----> POH

Decrypted Text:
POH ----> Inverse Key:
8 5 10 
21 8 21 
21 12 8 
ACT

*/
