#include <bits/stdc++.h>
#include <gmp.h>
using namespace std;

typedef struct{
	mpz_t x;
	mpz_t y;
} point_t;

mpz_t a, b, q;

void point_double(point_t point, point_t &out){
	mpz_t num, denom, tmp1, tmp2; mpz_inits(num, denom, tmp1, tmp2, NULL);
	mpz_set(tmp1, point.x);
	mpz_mul(tmp1, tmp1, tmp1);
	mpz_mul_ui(tmp1, tmp1, 3);
	mpz_add(tmp1, tmp1, a);
	mpz_set(tmp2, point.y);
	mpz_mul_ui(tmp2, tmp2, 2);
	mpz_invert(tmp2, tmp2, q);
	mpz_mul(tmp1, tmp1, tmp2);
	mpz_mod(tmp1, tmp1, q);

	mpz_set(out.x, tmp1);
	mpz_mul(out.x, out.x, out.x);
	mpz_sub(out.x, out.x, point.x);
	mpz_sub(out.x, out.x, point.x);
	mpz_mod(out.x, out.x, q);

	mpz_set(out.y, point.x);
	mpz_sub(out.y, out.y, out.x);
	mpz_mul(out.y, out.y, tmp1);
	mpz_sub(out.y, out.y, point.y);
	mpz_mod(out.y, out.y, q);
}

void point_add(point_t point1, point_t point2, point_t &out){
	mpz_t num, denom, tmp1, tmp2; mpz_inits(num, denom, tmp1, tmp2, NULL);
	mpz_set(tmp1, point2.y);
	mpz_sub(tmp1, tmp1, point1.y);
	mpz_set(tmp2, point2.x);
	mpz_sub(tmp2, tmp2, point1.x);
	mpz_invert(tmp2, tmp2, q);
	mpz_mul(tmp1, tmp1, tmp2);
	mpz_mod(tmp1, tmp1, q);

	mpz_set(out.x, tmp1);
	mpz_mul(out.x, out.x, out.x);
	mpz_sub(out.x, out.x, point1.x);
	mpz_sub(out.x, out.x, point2.x);
	mpz_mod(out.x, out.x, q);

	mpz_set(out.y, point1.x);
	mpz_sub(out.y, out.y, out.x);
	mpz_mul(out.y, out.y, tmp1);
	mpz_sub(out.y, out.y, point1.y);
	mpz_mod(out.y, out.y, q);
}

void point_mult(mpz_t num, point_t point, point_t &out){
	mpz_t tmpnum; mpz_init(tmpnum);
	mpz_set(tmpnum, num);

	point_t tmp1; mpz_inits(tmp1.x, tmp1.y, NULL);
	point_double(point, tmp1);
	mpz_sub_ui(num, num, 2);

	while(mpz_cmp_ui(num, 0)>0){
		point_t tmp2; mpz_inits(tmp2.x, tmp2.y, NULL);
		mpz_set(tmp2.x, tmp1.x);
		mpz_set(tmp2.y, tmp1.y);
		point_add(tmp2, point, tmp1);
		mpz_sub_ui(num, num, 1);
	}
	mpz_set(out.x, tmp1.x);
	mpz_set(out.y, tmp1.y);

	mpz_set(num, tmpnum);
}

void decrypt(){
	point_t c1, c2; mpz_inits(c1.x, c1.y , c2.x, c2.y, NULL);
	ifstream fin("key.txt", ios::in);

	point_t p_a, g; mpz_t n_a; mpz_inits(n_a, p_a.x, p_a.y, g.x, g.y, NULL);
	fin>>n_a;
	fin>>p_a.x;
	fin>>p_a.y;
	fin>>g.x;
	fin>>g.y;
	fin.close();

	fin.open("ciphertext.txt", ios::in);
	fin>>c1.x;
	fin>>c1.y;
	fin>>c2.x;
	fin>>c2.y;
	fin.close();

	point_t tmp; mpz_inits(tmp.x, tmp.y, NULL);
	mpz_set(tmp.x, c1.x);
	mpz_set(tmp.y, c1.y);

	point_mult(n_a, tmp, c1);
	mpz_sub(c1.y, q, c1.y);

	point_t p_m; mpz_inits(p_m.x, p_m.y, NULL);
	point_add(c2, c1, p_m);

	cout<<p_m.x<<"\n";
	cout<<p_m.y<<"\n";
}

int main(){
	mpz_inits(a, b, q, NULL);
	ifstream fin("elliptic_parameters.txt", ios::in);
	fin>>q;
	fin>>a;
	fin>>b;
	cout<<q<<" "<<a<<" "<<b<<"\n";
	fin.close();
	decrypt();
}

