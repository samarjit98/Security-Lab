#include <gmp.h>
#include <bits/stdc++.h>
using namespace std;

void extended_gcd(mpz_t a, mpz_t b, mpz_t gcd, mpz_t x, mpz_t y){
	if(mpz_cmp_ui(a, 0)==0){
		mpz_set(gcd, b);
		mpz_set_ui(x, 0);
		mpz_set_ui(y, 1);
	}	
	else{
		mpz_t rem;
		mpz_init(rem);
		mpz_cdiv_r(rem, b, a);
		mpz_t a1; mpz_init(a1);
		mpz_set(a1, a); 
		extended_gcd(rem, a1, gcd, x, y);
		mpz_t quot;
		mpz_init(quot);
		mpz_cdiv_q(quot, b, a);
		mpz_t tmp1, tmp2;
		mpz_inits(tmp1, tmp2, NULL);
		mpz_mul(tmp1, quot, x);
		mpz_set(tmp2, x);
		mpz_sub(x, y, tmp1);
		mpz_set(y, tmp2);
	}
}


int main(){
	mpz_t a, b, gcd, x, y;
	mpz_inits(a, b, gcd, x, y, NULL);
	cin>>a>>b;
	extended_gcd(a, b, gcd, x, y);
	cout << gcd << endl;
	cout << x << endl;
	cout << y << endl;	

	return 0;
}
		
/*


*/


