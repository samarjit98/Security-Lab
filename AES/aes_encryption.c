#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int S_box[16][16];

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