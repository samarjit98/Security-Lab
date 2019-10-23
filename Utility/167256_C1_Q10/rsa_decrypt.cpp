#include <gmp.h>
#include <bits/stdc++.h>
using namespace std;

void decrypt(){
	mpz_t e, d, n; mpz_inits(e, d, n, NULL);

	ifstream fin; fin.open("key.txt", ios::in);
	fin>>e; fin>>n; fin>>d;
	fin.close();

	mpz_t c; mpz_init(c);

	fin.open("ciphertext.txt", ios::in);
	fin >> c;
	fin.close();

	mpz_t p; mpz_init(p);

	mpz_powm(p, c, d, n);

	cout<<p<<"\n";
}

int main(){
	decrypt();
	return 0;
}