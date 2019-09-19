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
		mpz_t rem, x1, y1;
		mpz_inits(rem, x1, y1, NULL);
		mpz_mod(rem, b, a);
		mpz_t a1; mpz_init(a1);
		mpz_set(a1, a); 
		extended_gcd(rem, a1, gcd, x1, y1);
		mpz_t quot;
		mpz_init(quot);
		mpz_fdiv_q(quot, b, a);
		mpz_t tmp1, tmp2;
		mpz_inits(tmp1, tmp2, NULL);
		mpz_mul(tmp1, quot, x1);
		mpz_set(tmp2, x1);
		mpz_sub(x, y1, tmp1);
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


