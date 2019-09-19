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

void invert(int **key, int **key_inv){

}

void decrypt(int *block, int **key, int n){
	int *block_cpy;
	block_cpy = (int*)malloc(n*sizeof(int));

	int **key_inv;
	key_inv = (int**)malloc(n*sizeof(int*));
	for(int i=0; i<n; i++)key_inv[i] = (int*)malloc(n*sizeof(int));

	invert(key, key_inv);

	for(int i=0; i<n; i++)block_cpy[i] = blocks[i];

	for(int i=0; i<n; i++){
		int elem = 0;
		for(int j=0; j<n; j++){
			elem += block_cpy[j] * key_inv[i][j];
		}
		block[i] = elem;
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
				blocks[i][j] = buff[i*n + j] - 'A';

		printf("\nEncrypted Text:\n");

		for(int i=0; i<num_blocks; i++){
			for(int j=0; j<n; j++)printf("%c", blocks[i][j] + 'A');
			printf(" ----> ");
			encrypt(blocks[i], key, n);
			for(int j=0; j<n; j++)printf("%c", blocks[i][j] + 'A');
			printf("\n");
		}

		scanf("%c", &ans);
	}

	return 0;
}
