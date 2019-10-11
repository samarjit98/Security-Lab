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

int mul_field(int a, int b){
	
}

void mix_columns(int state_array[4][4]){
	int field_mat[4][4] = {
							{2, 3, 1, 1},
							{1, 2, 3, 1},
							{1, 1, 2, 3},
							{3, 1, 1, 2}
						};
	int state_cpy[4][4];
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++){
			int val = 0;
			for(int k=0; k<4; k++){
				val = val ^ mul_field(field_mat[i][k], state_array[k][j]);
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
		substitute_bytes(state_array);
		shift_rows(state_array);
		if(i<9)mix_columns(state_array);
		add_round_key(state_array, Key[i+1]);
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

	printf("Plaintext state array: ");
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

	for(int i=1; i<=10; i++){

	}
}

int main(){
	FILE* f;

	f = fopen("S_box.txt", "r");
	for(int i=0; i<16; i++)
		for(int j=0; i<16; j++)
			fscanf(f, "%x", &S_box[i][j]);
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