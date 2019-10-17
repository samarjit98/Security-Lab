#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int S_box[16][16];

void add_round_key(int state_array[4][4], int key[4][4]){
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			state_array[i][j] = state_array[i][j] ^ key[i][j];
}

void substitute_bytes(int state_array[4][4]){
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++){
			int col = state_array[i][j] & 15;
			int row = (state_array[i][j] >> 4) & 15;
			state_array[i][j] = S_box[row][col];
		}
}

void left_shift(int *data, int n, int times){
	for(int j=0; j<times; j++){
		int tmp=data[0];
		for(int i=0; i<n-1; i++)data[i] = data[i+1];
		data[n-1]=tmp;
	}
}

void shift_rows(int state_array[4][4]){
	for(int i=0; i<4; i++){
		left_shift(state_array[i], 4, i);
	}
}

int gal_mul(int a, int b){
	int res = 0;
	int min_poly = 0b11101;
	for (; b; b >>= 1){
		if (b & 1)res ^= a;
		if (a & 0x80)a = (a << 1) ^ min_poly;
		else a <<= 1;
	}
	return res;
}

void mix_columns(int state_array[4][4]){
	int field_mat[4][4] = { {2, 3, 1, 1}, {1, 2, 3, 1}, {1, 1, 2, 3}, {3, 1, 1, 2} };
	int state_cpy[4][4];
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++){
			int val = 0;
			for(int k=0; k<4; k++){
				val = val ^ gal_mul(field_mat[i][k], state_array[k][j]);
			}
			state_cpy[i][j] = val;
		}

	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			state_array[i][j] = state_cpy[i][j];
}

void encrypt(int state_array[4][4], int Key[11][4][4]){
	add_round_key(state_array, Key[0]);

	for(int i=0; i<10; i++){
		printf("Round: %d\n", i);
		printf("Input:\n");
		for(int j=0; j<4; j++){
			for(int k=0; k<4; k++)
				printf("%x ", state_array[j][k]);
			printf("\n");
		}
		substitute_bytes(state_array);
		shift_rows(state_array);
		if(i<9)mix_columns(state_array);
		add_round_key(state_array, Key[i+1]);
		printf("Output:\n");
		for(int j=0; j<4; j++){
			for(int k=0; k<4; k++)
				printf("%x ", state_array[j][k]);
			printf("\n");
		}
		printf("Key:\n");
		for(int j=0; j<4; j++){
			for(int k=0; k<4; k++)
				printf("%x ", Key[i+1][j][k]);
			printf("\n");
		}
		printf("\n");
	}
}

void convert_to_state_array(unsigned long long int ip1, unsigned long long int ip2, int state_array[4][4]){
	for(int j=3; j>=0; j--)
		for(int i=3; i>=0; i--){
			if(j>=2){
				state_array[i][j] = ip2 & 255;
				ip2 = ip2 >> 8;
			}
			else{
				state_array[i][j] = ip1 & 255;
				ip1 = ip1 >> 8;
			}
		}

	printf("Plaintext state array: \n");
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++)
			printf("%x ", state_array[i][j]);
		printf("\n");
	}
	printf("\n");
}

void generate_key(unsigned long long int key1, unsigned long long int key2, int Key[11][4][4]){
	for(int j=3; j>=0; j--)
		for(int i=3; i>=0; i--){
			if(j>=2){
				Key[0][i][j] = key2 & 255;
				key2 = key2 >> 8;
			}
			else{
				Key[0][i][j] = key1 & 255;
				key1 = key1 >> 8;
			}
		}

	int RCon[10][4]; RCon[0][0] = 1; RCon[0][1] = 0; RCon[0][2] = 0; RCon[0][3] = 0; 
	for(int i=1; i<10; i++){
		RCon[i][0] = gal_mul(RCon[i-1][0], 2);
		for(int j=1; j<4; j++)RCon[i][j] = 0;
	}

	for(int i=1; i<=10; i++){
		for(int j=0; j<4; j++){
			int tmp;
			if(j==0){
				tmp = Key[i-1][0][3];
				for(int k=0; k<3; k++)Key[i][k][j] = Key[i-1][k+1][j];
				Key[i][3][j] = tmp;
			}
			else{
				tmp = Key[i][0][j-1];
				for(int k=0; k<3; k++)Key[i][k][j] = Key[i][k+1][j-1];
				Key[i][3][j] = tmp;
			}
		}

		for(int j=0; j<4; j++)
			for(int k=0; k<4; k++){
				int col = Key[i][j][k] & 15;
				int row = (Key[i][j][k] >> 4) & 15;
				Key[i][j][k] = S_box[row][col];
		}

		for(int j=0; j<4; j++)Key[i][j][0] = Key[i][j][0] ^ RCon[i-1][j];

		for(int j=0; j<4; j++)
			for(int k=0; k<4; k++)
				Key[i][j][k] = Key[i][j][k] ^ Key[i-1][j][k];
	}
}

int main(){
	FILE* f;

	f = fopen("S_box.txt", "r");
	for(int i=0; i<16; i++){
		for(int j=0; j<16; j++){
			fscanf(f, "%x", &S_box[i][j]);
			printf("%x ", S_box[i][j]);
		}
		printf("\n");
	}
	fclose(f);

	unsigned long long int key1, key2;
	printf("Enter key (64 bits - 64 bits): ");
	scanf("%llx", &key1); scanf("%llx", &key2);

	unsigned long long int ip1, ip2;
	printf("Enter plaintext (64 bits - 64 bits): ");
	scanf("%llx", &ip1); scanf("%llx", &ip2);

	int state_array[4][4], Key[11][4][4];
	convert_to_state_array(ip1, ip2, state_array);
	generate_key(key1, key2, Key);
	encrypt(state_array, Key);

	printf("Ciphertext: ");
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++)
			printf("%x ", state_array[i][j]);
		printf("\n");
	}
	printf("\n");

	return 0;
}

/*
2475a2b334755688 31e2120013aa5487
0004121412041200 0c00131108231919
*/