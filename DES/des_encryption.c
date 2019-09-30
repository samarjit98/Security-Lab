#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	int S_box[8][4][16], Init_P[8][8], Final_P[8][8], ED[8][6], SD[4][8];
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
	Encryption algorithm
	*/

	return 0;
}