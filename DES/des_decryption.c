#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int S_box[8][4][16], Init_P[8][8], Final_P[8][8], ED[8][6], SD[4][8];

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

	binarize(per_out, S_box[i][row][col]);
}

void round_function(int data[32], int key[48]){
	int exp_data[48];

	for(int i=0; i<8; i++)
		for(int j=0; j<6; j++)
			exp_data[6*i + j] = data[ED[i][j] - 1];

	for(int i=0; i<48; i++)exp_data[i] = exp_data[i]^key[i];

	for(int i=0; i<8; i++){
		int per_data[6];
		for(int j=0; j<6; j++)per_data[j]=exp_data[6*i + j];

		int per_out[4];
		permutation_box(per_data, i, per_out);

		for(int j=0; j<4; j++)data[4*i + j] = per_out[j];
	}

	int data_cpy[32];
	for(int i=0; i<32; i++)data_cpy[i]=data[i];

	for(int i=0; i<4; i++)
		for(int j=0; j<8; j++)
			data[8*i + j] = data_cpy[SD[i][j] - 1];
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
			fscanf(f, "%d", &SD[i][j])
		}
	fclose(f);

	f = fopen("ED.txt", "r");
	for(int i=0; i<8; i++)
		for(int j=0; j<6; j++){
			fscanf(f, "%d", &ED[i][j])
		}
	fclose(f);

	f = fopen("Init_P.txt", "r");
	for(int i=0; i<8; i++)
		for(int j=0; j<8; j++){
			fscanf(f, "%d", &Init_P[i][j])
		}
	fclose(f);

	f = fopen("Final_P.txt", "r");
	for(int i=0; i<8; i++)
		for(int j=0; j<8; j++){
			fscanf(f, "%d", &Final_P[i][j])
		}
	fclose(f);

	/*
	Decryption algorithm
	*/

	return 0;
}