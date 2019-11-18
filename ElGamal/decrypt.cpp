#include <bits/stdc++.h>
#include <gmp.h>
using namespace std;

void decrypt(mpz_t msg, mpz_t p){
	ifstream fin("key.txt", ios::in);
	mpz_t q, g, key, h;
	mpz_inits(q, g, key, h, NULL);

	fin>>q;
	fin>>g;
	fin>>key;
	fin>>h;

	fin.close();

	mpz_powm(h, p, key, q);
	mpz_fdiv_q(msg, msg, h);

	cout<<msg<<endl;
}

int main(){
	mpz_t msg, p; mpz_inits(msg, p, NULL);
	ifstream fin("ciphertext.txt", ios::in);
	fin>>msg;
	fin>>p;
	fin.close();
	decrypt(msg, p);
	return 0;
}