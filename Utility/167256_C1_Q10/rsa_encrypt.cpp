#include <gmp.h>
#include <bits/stdc++.h>
using namespace std;

void encrypt(mpz_t m){
	mpz_t e, d, n; mpz_inits(e, d, n, NULL);

	ifstream fin; fin.open("key.txt", ios::in);
	fin>>e; fin>>n; fin>>d;
	fin.close();

	mpz_t c; mpz_init(c);

	mpz_powm(c, m, e, n);

	cout<<"Ciphertext:"<<c<<"\n";

	ofstream fout; fout.open("ciphertext.txt", ios::out);
	fout << c;
	fout.close();
}

int main(){
	mpz_t m; mpz_init(m);
	cin>>m; encrypt(m);
	return 0;
}