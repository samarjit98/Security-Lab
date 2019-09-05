#include <gmp.h>
#include <bits/stdc++.h>
using namespace std;

void print_group_tables(mpz_t z){
	mpz_t i, j; mpz_inits(i, j, NULL);
	mpz_set_ui(i, 0);
	mpz_set_ui(j, 0);
	
	while(mpz_cmp(i, z) < 0){
		mpz_set_ui(j, 0);
		while(mpz_cmp(j, z) < 0){	
			mpz_t mod; mpz_init(mod);
			mpz_add(mod, i, j);
			mpz_mod(mod, mod, z);
			cout<<"("<<i<<","<<j<<"): "<<mod<<"  "; 
			if(mpz_cmp_ui(mod, 0) == 0){
				cout << "<--- Additive inverses **  ";
			}
			mpz_add_ui(j, j, 1);
		}
		cout << "\n\n";
		mpz_add_ui(i, i, 1);
	}

	cout << "\n\n\n\n";	

	mpz_set_ui(i, 1);
	mpz_set_ui(j, 1);

	while(mpz_cmp(i, z) < 0){
		mpz_set_ui(j, 1);
		while(mpz_cmp(j, z) < 0){	
			mpz_t mod; mpz_init(mod);
			mpz_mul(mod, i, j);
			mpz_mod(mod, mod, z);
			cout<<"("<<i<<","<<j<<"): "<<mod<<"  "; 
			if(mpz_cmp_ui(mod, 1) == 0){
				cout << "<--- Multiplicative inverses **  ";
			}
			mpz_add_ui(j, j, 1);
		}
		cout<<"\n\n";
		mpz_add_ui(i, i, 1);
	}
}

int main(){
	mpz_t z; mpz_init(z);
	cin>>z;
	print_group_tables(z);
	
	return 0;
}
		
/*

c167256@selab168:~/Security-Lab$ ./a.out
10
(0,0): 0  <--- Additive inverses **  (0,1): 1  (0,2): 2  (0,3): 3  (0,4): 4  (0,5): 5  (0,6): 6  (0,7): 7  (0,8): 8  (0,9): 9  

(1,0): 1  (1,1): 2  (1,2): 3  (1,3): 4  (1,4): 5  (1,5): 6  (1,6): 7  (1,7): 8  (1,8): 9  (1,9): 0  <--- Additive inverses **  

(2,0): 2  (2,1): 3  (2,2): 4  (2,3): 5  (2,4): 6  (2,5): 7  (2,6): 8  (2,7): 9  (2,8): 0  <--- Additive inverses **  (2,9): 1  

(3,0): 3  (3,1): 4  (3,2): 5  (3,3): 6  (3,4): 7  (3,5): 8  (3,6): 9  (3,7): 0  <--- Additive inverses **  (3,8): 1  (3,9): 2  

(4,0): 4  (4,1): 5  (4,2): 6  (4,3): 7  (4,4): 8  (4,5): 9  (4,6): 0  <--- Additive inverses **  (4,7): 1  (4,8): 2  (4,9): 3  

(5,0): 5  (5,1): 6  (5,2): 7  (5,3): 8  (5,4): 9  (5,5): 0  <--- Additive inverses **  (5,6): 1  (5,7): 2  (5,8): 3  (5,9): 4  

(6,0): 6  (6,1): 7  (6,2): 8  (6,3): 9  (6,4): 0  <--- Additive inverses **  (6,5): 1  (6,6): 2  (6,7): 3  (6,8): 4  (6,9): 5  

(7,0): 7  (7,1): 8  (7,2): 9  (7,3): 0  <--- Additive inverses **  (7,4): 1  (7,5): 2  (7,6): 3  (7,7): 4  (7,8): 5  (7,9): 6  

(8,0): 8  (8,1): 9  (8,2): 0  <--- Additive inverses **  (8,3): 1  (8,4): 2  (8,5): 3  (8,6): 4  (8,7): 5  (8,8): 6  (8,9): 7  

(9,0): 9  (9,1): 0  <--- Additive inverses **  (9,2): 1  (9,3): 2  (9,4): 3  (9,5): 4  (9,6): 5  (9,7): 6  (9,8): 7  (9,9): 8  





(1,1): 1  <--- Multiplicative inverses **  (1,2): 2  (1,3): 3  (1,4): 4  (1,5): 5  (1,6): 6  (1,7): 7  (1,8): 8  (1,9): 9  

(2,1): 2  (2,2): 4  (2,3): 6  (2,4): 8  (2,5): 0  (2,6): 2  (2,7): 4  (2,8): 6  (2,9): 8  

(3,1): 3  (3,2): 6  (3,3): 9  (3,4): 2  (3,5): 5  (3,6): 8  (3,7): 1  <--- Multiplicative inverses **  (3,8): 4  (3,9): 7  

(4,1): 4  (4,2): 8  (4,3): 2  (4,4): 6  (4,5): 0  (4,6): 4  (4,7): 8  (4,8): 2  (4,9): 6  

(5,1): 5  (5,2): 0  (5,3): 5  (5,4): 0  (5,5): 5  (5,6): 0  (5,7): 5  (5,8): 0  (5,9): 5  

(6,1): 6  (6,2): 2  (6,3): 8  (6,4): 4  (6,5): 0  (6,6): 6  (6,7): 2  (6,8): 8  (6,9): 4  

(7,1): 7  (7,2): 4  (7,3): 1  <--- Multiplicative inverses **  (7,4): 8  (7,5): 5  (7,6): 2  (7,7): 9  (7,8): 6  (7,9): 3  

(8,1): 8  (8,2): 6  (8,3): 4  (8,4): 2  (8,5): 0  (8,6): 8  (8,7): 6  (8,8): 4  (8,9): 2  

(9,1): 9  (9,2): 8  (9,3): 7  (9,4): 6  (9,5): 5  (9,6): 4  (9,7): 3  (9,8): 2  (9,9): 1  <--- Multiplicative inverses **  


*/


