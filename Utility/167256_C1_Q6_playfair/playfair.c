#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *in, *kp;
char input[100], key[50];
char matrix[5][5];
char arr[100];
int j = 0;
int count = 0;

void encrypt(char a, char b)
{
	static int i = 0;
	arr[i] = a;
	arr[i + 1] = b;
	char c;
	if (a == b) {
		c = b;
		arr[i + 1] = 'X';
		arr[i + 2] = c;
	}

	int p, q, r, s;
	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			if (arr[j] == matrix[x][y]) {
				p = x;
				q = y;
			}
		}
	}

	for (int u = 0; u < 5; u++) {
		for (int t = 0; t < 5; t++) {
			if (arr[j + 1] == matrix[u][t]) {
				r = u;
				s = t;
			}
		}
	}

	if (q == s)
		printf("%c %c -> %c %c", a, b, matrix[(p + 1) % 5][q], 
		matrix[(r + 1) % 5][q]);
	else if (p == r)
		printf("%c %c -> %c %c", a, b, matrix[p][(q + 1) % 5], 
		matrix[p][(s + 1) % 5]);
	else
		printf("%c %c -> %c %c", a, b, matrix[p][s], matrix[r][q]);

	if (a == b)
		i = i + 3;
	else
		i = i + 2;

	j = j + 2;
}

void readkey()
{
	int i = 0;
	char ch;

	while (!feof(kp)) {
		ch = getc(kp);
		key[i++] = ch;
	}

	printf("\n");
	for (int k = 0; k < i; k++) {
		printf("%c", key[k]);
	}
	printf("\n");

	int k = 0;
	char inp[26];
	int flag[26] = { 0 };
	int index = 0;
	int x = strlen(key) - 1;

	while (x) {
		if (key[k] != ' ') {
			if (flag[key[k] - 65] != 1) {
				inp[index] = key[k];
				index++;
				flag[key[k] - 65] = 1;
			}
		}
		k++;
		x--;
	}

	for (int t = 0; t < 26; t++) {
		if (flag[t] != 1) {
			flag[t] = 1;
			if ((t + 65) != 'J') {
				inp[index] = t + 65;
				index++;
			}
		}
	}

	k = 0;
	printf("\n");

	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			matrix[x][y] = inp[k++];
			printf("%c ", matrix[x][y]);
		}
		printf("\n");
	}
	printf("\n");
}

void readfile()
{
	char ch;
	int k, i, n;
	int size;
	size = ftell(in);
	rewind(in);
	char a, b;

	k = size / 2;
	n = k;
	int result;
	char* ptr;

	while (k)
	{
		ptr = (char*)malloc(sizeof(char) * 2);
		if (ptr == NULL) {
			fputs("Memory Error", stderr);
			exit(2);
		}

		result = fread(ptr, 1, 2, in);
		if (result != 2) {
			exit(3);
		}

		a = (*ptr);
		b = *(ptr + 1);

		while (a == ' ') {
			a = b;
			b = getc(in);
		}

		while (b == ' ') {
			b = getc(in);
		}

		encrypt(a, b);
		printf("\n");
		count = count + 2;
		free(ptr);
		k--;
	}
}

void padinput()
{
	fseek(in, 0, SEEK_END);
	int size = ftell(in);

	if (size % 2) {
		fputc('X', in);
	}
}

int main()
{
	// Use plain text "HIDE THE GOLD IN THE TREE STUMP"
	in = fopen("playfair.txt", "r+");
	if (in == NULL) {
		printf("Error while opening the file.\n");
		exit(1);
	}

	// Use "playfair example" as the key to encrypt the message
	kp = fopen("key.txt", "r");
	if (kp == NULL) {
		printf("Error while opening the file.\n");
		exit(1);
	}

	padinput();
	readkey();
	readfile();

	while (j != count) {
		if (arr[j + 1] != '\0')
			encrypt(arr[j], arr[j + 1]);
	}

	fclose(in);

	return 0;
}