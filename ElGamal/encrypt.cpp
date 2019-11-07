#include <bits/stdc++.h>
#include <gmp.h>
using namespace std;

void encrypt(mpz_t msg){
	ifstream fin("key.txt", ios::in);
	mpz_t q, g, key, h;
	mpz_inits(q, g, key, h, NULL);

	fin>>q;
	fin>>g;
	fin>>key;
	fin>>h;

	fin.close();

	mpz_t q_lower; mpz_inits(q_lower);
	mpz_ui_pow_ui(q_lower, 10, 20);

	mpz_t k, s, p; mpz_inits(k, s, p, NULL);

	mpz_t gcd_val; mpz_init(gcd_val);
	do{
		gmp_randstate_t st;
		gmp_randinit_mt(st);
		gmp_randseed_ui(st, 214567);
		mpz_urandomm(k, st, q);

		mpz_gcd(gcd_val, q, k);
	}while(mpz_cmp(key, q_lower) < 0 && mpz_cmp_ui(gcd_val, 1) != 0);

	mpz_powm(s, h, k, q);
	mpz_powm(p, g, k, q);

	mpz_mul(msg, s, msg);

	ofstream fout("ciphertext.txt", ios::out);
	fout<<msg<<"\n";
	fout<<p<<"\n";
	fout.close();
}

int main(){
	mpz_t msg; mpz_init(msg);
	cin>>msg;
	encrypt(msg);
	return 0;
}