#include <bits/stdc++.h>
#include <gmp.h>
using namespace std;

int main(){
	mpz_t q_upper, q_lower, q, g, key, h;
	mpz_inits(q_upper, q_lower, q, g, h, NULL);

	mpz_ui_pow_ui(q_upper, 10, 50);
	mpz_ui_pow_ui(q_lower, 10, 20);

	do{
		gmp_randstate_t st;
		gmp_randinit_mt(st);
		gmp_randseed_ui(st, 123756);
		mpz_urandomm(q, st, q_upper);
	}while(mpz_cmp(q_lower, q) > 0);

	do{
		gmp_randstate_t st;
		gmp_randinit_mt(st);
		gmp_randseed_ui(st, 123579);
		mpz_urandomm(g, st, q);
	}while(mpz_cmp_ui(g, 2) < 0);

	mpz_t gcd_val; mpz_init(gcd_val);
	do{
		gmp_randstate_t st;
		gmp_randinit_mt(st);
		gmp_randseed_ui(st, 214567);
		mpz_urandomm(key, st, q);

		mpz_gcd(gcd_val, q, key);
	}while(mpz_cmp(key, q_lower) < 0 && mpz_cmp_ui(gcd_val, 1) != 0);

	mpz_powm(h, g, key, q);

	ofstream fout("key.txt", ios::out);
	fout<<q<<"\n";
	fout<<g<<"\n";
	fout<<key<<"\n";
	fout<<h<<"\n";
	fout.close();
	return 0;
}