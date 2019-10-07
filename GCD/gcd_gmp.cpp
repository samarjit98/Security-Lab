#include <gmp.h>
#include <bits/stdc++.h>
using namespace std;

void normal_gcd(mpz_t a, mpz_t b, mpz_t gcd){
	if(mpz_cmp_ui(a, 0)==0){
		mpz_set(gcd, b);
	}	
	else{
		mpz_t rem;
		mpz_init(rem);
		mpz_mod(rem, b, a);
		normal_gcd(rem, a, gcd);
	}
}



int main(){
	mpz_t a, b, gcd;
	mpz_inits(a, b, gcd, NULL);
	cin>>a>>b;
	normal_gcd(a, b, gcd);
	cout << "GCD: " << gcd << endl;
	return 0;
}
		
/*


*/


