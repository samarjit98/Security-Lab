#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Inv_S_box[16][16], S_box[16][16];

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
			state_array[i][j] = Inv_S_box[row][col];
		}
}

void right_shift(int *data, int n, int times){
	for(int j=0; j<times; j++){
		int tmp=data[n-1];
		for(int i=n-1; i>0; i--)data[i] = data[i-1];
		data[0]=tmp;
	}
}

void shift_rows(int state_array[4][4]){
	for(int i=0; i<4; i++){
		right_shift(state_array[i], 4, i);
	}
}

int gal_mul(int a, int b){
	int res = 0;
	int min_poly = 0b11011;
	for (; b; b >>= 1){
		if (b & 1)res ^= a;
		if (a & 0x80)a = (a << 1) ^ min_poly;
		else a <<= 1;
		a = a & (0b11111111);
	}
	return res;
}

void mix_columns(int state_array[4][4]){
	int field_mat[4][4] = { {0x0e, 0x0b, 0x0d, 0x09}, {0x09, 0x0e, 0x0b, 0x0d}, {0x0d, 0x09, 0x0e, 0x0b}, {0x0b, 0x0d, 0x09, 0x0e} };
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

void decrypt(int state_array[4][4], int Key[11][4][4]){
	add_round_key(state_array, Key[10]);

	for(int i=0; i<10; i++){
		printf("Round: %d\n", i);
		printf("Input:\n");
		for(int j=0; j<4; j++){
			for(int k=0; k<4; k++)
				printf("%x ", state_array[j][k]);
			printf("\n");
		}
		shift_rows(state_array);
		printf("Shift Rows:\n");
		for(int j=0; j<4; j++){
			for(int k=0; k<4; k++)
				printf("%x ", state_array[j][k]);
			printf("\n");
		}
		substitute_bytes(state_array);
		printf("Sub bytes:\n");
		for(int j=0; j<4; j++){
			for(int k=0; k<4; k++)
				printf("%x ", state_array[j][k]);
			printf("\n");
		}
		add_round_key(state_array, Key[10 - (i+1)]);
		if(i<9){
			mix_columns(state_array);
			printf("Mix columns:\n");
			for(int j=0; j<4; j++){
				for(int k=0; k<4; k++)
					printf("%x ", state_array[j][k]);
				printf("\n");
			}
		}
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
	int init_val[10] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };
	int RCon[10][4]; RCon[0][0] = 1; RCon[0][1] = 0; RCon[0][2] = 0; RCon[0][3] = 0; 
	for(int i=0; i<10; i++){
		RCon[i][0] = init_val[i]; 
		for(int j=1; j<4; j++)RCon[i][j] = 0;
	}

	for(int i=1; i<=10; i++){
		for(int j=0; j<4; j++){
			int tmp;
			if(j==0){
				tmp = Key[i-1][0][3];
				for(int k=0; k<3; k++)Key[i][k][j] = Key[i-1][k+1][3];
				Key[i][3][j] = tmp;

				for(int k=0; k<4; k++){
					int col = Key[i][k][j] & 15;
					int row = (Key[i][k][j] >> 4) & 15;
					Key[i][k][j] = S_box[row][col];
				}

				for(int k=0; k<4; k++)Key[i][k][j] = Key[i][k][j] ^ RCon[i-1][k];

				for(int k=0; k<4; k++)
					Key[i][k][j] = Key[i][k][j] ^ Key[i-1][k][j];
			}
			else{
				for(int k=0; k<4; k++)Key[i][k][j] = Key[i][k][j-1];

				for(int k=0; k<4; k++)
					Key[i][k][j] = Key[i][k][j] ^ Key[i-1][k][j];
			}
		}
		/*
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
		*/
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

	f = fopen("Inv_S_box.txt", "r");
	for(int i=0; i<16; i++){
		for(int j=0; j<16; j++){
			fscanf(f, "%x", &Inv_S_box[i][j]);
			printf("%x ", Inv_S_box[i][j]);
		}
		printf("\n");
	}
	fclose(f);

	unsigned long long int key1, key2;
	printf("Enter key (64 bits - 64 bits): ");
	scanf("%llx", &key1); scanf("%llx", &key2);

	int state_array[4][4], Key[11][4][4];
	f = fopen("ciphertext.txt", "r");
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			fscanf(f, "%x", &state_array[i][j]);
			printf("%x ", state_array[i][j]);
		}
		printf("\n");
	}
	fclose(f);

	generate_key(key1, key2, Key);
	decrypt(state_array, Key);

	printf("Plaintext: ");
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++)
			printf("%x ", state_array[i][j]);
		printf("\n");
	}
	printf("\n");

	return 0;
}

/*
2b7e151628aed2a6 abf7158809cf4f3c
3243f6a8885a308d 313198a2e0370734
*/