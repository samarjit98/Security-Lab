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

void crt(mpz_t num[], mpz_t rem[], mpz_t crt_x, int n){
	mpz_t prod; mpz_init(prod);
	mpz_set_ui(prod, 1);

	for(int i=0; i<n; i++)mpz_mul(prod, prod, num[i]);

	mpz_t res; mpz_init(res);
	mpz_set_ui(res, 0);

	for(int i=0; i<n; i++){
		mpz_t pp; mpz_init(pp);
		mpz_cdiv_q(pp, prod, num[i]);
		mpz_t gcd, x, y; mpz_inits(gcd, x, y, NULL);
		extended_gcd(pp, num[i], gcd, x, y); 
		if(mpz_sgn(x)==-1)mpz_add(x, x, num[i]);
		mpz_t tmp; mpz_init(tmp);
		mpz_mul(tmp, rem[i], x);
		mpz_mul(tmp, tmp, pp);
		mpz_add(res, res, tmp);
	}

	mpz_mod(res, res, prod);

	mpz_set(crt_x, res);
}

int main(){
	int n; cin>>n;
	mpz_t num[n], rem[n];

	for(int i=0; i<n; i++)mpz_inits(num[i], rem[i], NULL);
	for(int i=0; i<n; i++)cin>>num[i];
	for(int i=0; i<n; i++)cin>>rem[i];

	mpz_t x; mpz_init(x);
	crt(num, rem, x, n);
	
	cout<<x<<endl;

	return 0;
}

/*

c167256@uselab170:~/Security-Lab$ ./a.out
3
3 4 5
2 3 1
11

c167256@uselab170:~/Security-Lab$ ./a.out
2
732498723894 743647832
6743 7436
286354154922119220296

*/
