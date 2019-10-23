#include <gmp.h>
#include <bits/stdc++.h>
using namespace std;

void generate_key(mpz_t n, mpz_t e){
	mpz_t p, q, n_tuot; mpz_inits(p, q, n_tuot, NULL);
	mpz_nextprime(p, n); mpz_nextprime(q, p);
	mpz_mul(n, p, q);

	mpz_t tmp1, tmp2; mpz_inits(tmp1, tmp2, NULL);
	mpz_set(tmp1, p);
	mpz_set(tmp2, q);
	mpz_sub_ui(tmp1, tmp1, 1);
	mpz_sub_ui(tmp2, tmp2, 1);
	mpz_mul(n_tuot, tmp1, tmp2);

	while(mpz_cmp(e, n_tuot) < 0){
		mpz_t gcd_val; mpz_init(gcd_val);
		mpz_gcd(gcd_val, e, n_tuot);
		if(mpz_cmp_ui(gcd_val, 1)==0)break;
		mpz_add_ui(e, e, 1);
	}

	mpz_t d; mpz_init(d);
	mpz_invert(d, e, n_tuot);

	ofstream fout;
	fout.open("key.txt", ios::out);

	fout << e << " " << n << "\n";
	fout << d << " " << n << "\n";

	fout.close();
}

int main(){
	mpz_t n, e; mpz_inits(n, e, NULL);
	cin>>n>>e;
	generate_key(n, e);
	return 0;
}