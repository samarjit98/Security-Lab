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

void generate_key(point_t g){
	mpz_t n_a; mpz_init(n_a);
	cin>>n_a;
	mpz_t tmp1; mpz_init(tmp1);
	mpz_set(tmp1, n_a);

	point_t p_a; mpz_inits(p_a.x, p_a.y, NULL);

	point_mult(tmp1, g, p_a);

	ofstream fout("key.txt", ios::out);
	fout<<n_a<<"\n";
	fout<<p_a.x<<"\n";
	fout<<p_a.y<<"\n";
	fout<<g.x<<"\n";
	fout<<g.y<<"\n";
	fout.close();
}

int main(){
	mpz_inits(a, b, q, NULL);
	ifstream fin("elliptic_parameters.txt", ios::in);
	fin>>q;
	fin>>a;
	fin>>b;
	cout<<q<<" "<<a<<" "<<b<<"\n";
	fin.close();
	point_t g; mpz_inits(g.x, g.y, NULL);
	cin>>g.x; cin>>g.y;
	generate_key(g);
	return 0;
}