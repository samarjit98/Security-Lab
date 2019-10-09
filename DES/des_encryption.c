#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int S_box[8][4][16], Init_P[8][8], Final_P[8][8], ED[8][6], SD[4][8], CD[6][8], PD[7][8];

void binarize(int *data, int n, int val){
	for(int i=n-1; i>=0; i--){
		data[i] = val%2;
		val /= 2;
	}
}

void binarize2(int *data, int n, unsigned long long val){
	for(int i=n-1; i>=0; i--){
		data[i] = val%2;
		val /= 2;
	}
}

void ubinarize(int *data, int n, unsigned long long *val){
	*val = 0;
	unsigned long long mul = 1;
	for(int i=n-1; i>=0; i--){
		*val += data[i]*mul;
		mul *= 2;
	}
}

void init_permutation(int data[64]){
	int data_cpy[64];
	for(int i=0; i<64; i++)data_cpy[i]=data[i];

	for(int i=0; i<8; i++)
		for(int j=0; j<8; j++)
			data[8*i + j] = data_cpy[Init_P[i][j] - 1];
}

void final_permutation(int data[64]){
	int data_cpy[64];
	for(int i=0; i<64; i++)data_cpy[i]=data[i];

	for(int i=0; i<8; i++)
		for(int j=0; j<8; j++)
			data[8*i + j] = data_cpy[Final_P[i][j] - 1];
}

void permutation_box(int per_data[6], int i, int per_out[4]){
	int row = 2*per_data[0] + per_data[5];
	int col = 8*per_data[1] + 4*per_data[2] + 2*per_data[3] + per_data[4];

	binarize(per_out, 4, S_box[i][row][col]);
}

void round_function(int data[32], int key[48]){
	int exp_data[48];

	unsigned long long val;
	ubinarize(key, 48, &val);
	printf("Key: %llx\n", val);

	for(int i=0; i<8; i++)
		for(int j=0; j<6; j++)
			exp_data[6*i + j] = data[ED[i][j] - 1];

	ubinarize(exp_data, 48, &val);
	printf("After expansion: %llx\n", val);

	for(int i=0; i<48; i++)exp_data[i] = exp_data[i]^key[i];

	ubinarize(exp_data, 48, &val);
	printf("After xor: %llx\n", val);

	for(int i=0; i<8; i++){
		int per_data[6];
		for(int j=0; j<6; j++)per_data[j]=exp_data[6*i + j];

		int per_out[4];
		permutation_box(per_data, i, per_out);

		for(int j=0; j<4; j++)data[4*i + j] = per_out[j];
	}
	
	int data_cpy[32];
	for(int i=0; i<32; i++)data_cpy[i]=data[i];

	ubinarize(data_cpy, 32, &val);
	printf("After sbox: %llx\n", val);

	for(int i=0; i<4; i++)
		for(int j=0; j<8; j++)
			data[8*i + j] = data_cpy[SD[i][j] - 1];
	ubinarize(data, 32, &val);
	printf("After str d box: %llx\n", val);

}

void left_shift(int *data, int n, int times){
	for(int j=0; j<times; j++){
		int tmp=data[0];
		for(int i=0; i<n-1; i++)data[i] = data[i+1];
		data[n-1]=tmp;
	}
}

void generate_key(int in_key[64], int key[8][48]){
	int par_drop[56];

	printf("Key Generation--\n");

	for(int i=0; i<7; i++)
		for(int j=0; j<8; j++)
			par_drop[8*i + j] = in_key[PD[i][j] - 1];

	unsigned long long val;
	ubinarize(par_drop, 56, &val);
	printf("After par drop: %llx\n", val);
	
	int shifts[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
	for(int i=0; i<16; i++){
		left_shift(par_drop, 28, shifts[i]);
		left_shift(par_drop + 28, 28, shifts[i]);

		ubinarize(par_drop, 56, &val);
		printf("After shifting: %llx\n", val);

		for(int j=0; j<6; j++)
			for(int k=0; k<8; k++){
				key[i][j*8 + k] = par_drop[CD[j][k] - 1];
			}
	}
}

void encrypt(int data[64], int key[8][48]){
	int L[32], R[32];
	init_permutation(data);

	unsigned long long val;
	ubinarize(data, 64, &val);
	printf("After init permutation: %llx\n", val);

	for(int i=0; i<64; i++)
		if(i<32)L[i] = data[i];
		else R[i - 32] = data[i];

	for(int i=0; i<16; i++){
		printf("Round %d\n", i+1);
		int R_cpy[32];

		for(int ii=0; ii<32; ii++)R_cpy[ii] = R[ii];

		round_function(R, key[i]);

		for(int ii=0; ii<32; ii++)L[ii] = L[ii]^R[ii];

		ubinarize(L, 32, &val);
		printf("After xoring: %llx\n", val);

		if(i<15){
			for(int ii=0; ii<32; ii++)R[ii] = L[ii];
			for(int ii=0; ii<32; ii++)L[ii] = R_cpy[ii];
		}
		else{
			for(int ii=0; ii<32; ii++)R[ii] = R_cpy[ii];
			for(int ii=0; ii<32; ii++)L[ii] = L[ii];
		}
		
		ubinarize(L, 32, &val);
		printf("After round: %llx ", val);
		ubinarize(R, 32, &val);
		printf("%llx ", val);
		printf("\n\n");
	}

	for(int i=0; i<64; i++)
		if(i<32)data[i] = L[i];	
		else data[i] = R[i - 32];

	final_permutation(data);
}

int main(int argc, char* argv[]){
	/*
	Init S and P boxes
	*/
	FILE* f;
	
	for(int i=1; i<=8; i++){
		char filename[10];
		sprintf(filename, "S%d.txt", i);
		f = fopen(filename, "r");

		for(int j=0; j<4; j++)
			for(int k=0; k<16; k++){
				fscanf(f, "%d", &S_box[i-1][j][k]);
			}

		fclose(f);
	}

	f = fopen("SD.txt", "r");
	for(int i=0; i<4; i++)
		for(int j=0; j<8; j++){
			fscanf(f, "%d", &SD[i][j]);
		}
	fclose(f);

	f = fopen("ED.txt", "r");
	for(int i=0; i<8; i++)
		for(int j=0; j<6; j++){
			fscanf(f, "%d", &ED[i][j]);
		}
	fclose(f);

	f = fopen("Init_P.txt", "r");
	for(int i=0; i<8; i++)
		for(int j=0; j<8; j++){
			fscanf(f, "%d", &Init_P[i][j]);
		}
	fclose(f);

	f = fopen("Final_P.txt", "r");
	for(int i=0; i<8; i++)
		for(int j=0; j<8; j++){
			fscanf(f, "%d", &Final_P[i][j]);
		}
	fclose(f);
	
	f = fopen("CD.txt", "r");
	for(int i=0; i<6; i++)
		for(int j=0; j<8; j++){
			fscanf(f, "%d", &CD[i][j]);
		}
	fclose(f);

	f = fopen("PD.txt", "r");
	for(int i=0; i<7; i++)
		for(int j=0; j<8; j++){
			fscanf(f, "%d", &PD[i][j]);
		}
	fclose(f);

	/*
	Encryption algorithm
	*/
	
	unsigned long long hex_in, hex_key;
	printf("Enter key: ");
	scanf("%llx", &hex_key);
	printf("Enter input: ");
	scanf("%llx", &hex_in);



	int in_key[64], key[8][48], data[64];

	binarize2(data, 64, hex_in);
	binarize2(in_key, 64, hex_key);

	generate_key(in_key, key);
	encrypt(data, key);

	ubinarize(data, 64, &hex_in);
	printf("Ciphertext: %llx \n", hex_in);

	f = fopen("ciphertext.txt", "w");
	for(int i=0; i<64; i++)fprintf(f, "%d\n", data[i]);
	fclose(f);

	return 0;
}