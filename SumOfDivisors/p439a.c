/*
*   http://oeis.org/wiki/Sum_of_divisors_function
*   
*   S(N) = ∑1≤i≤N ∑1≤j≤N d(i·j)
*   S(10^11) 
*/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include<malloc.h>
#include "gmp.h"
// p12 
// 76576500 576

#define TOTAL_PRIMES 16384 
//1000000 

int prime_check(int p){
	double space = sqrt(p);
			
	int j;       
	
	if((space*space)==p)return 0;
	
	//printf("space %f\n", space);

	if(p==2||p==5||p==3||p==7||p==11)return 1;
	
	if(p%2==0)return 0;
	if(p%3==0)return 0;
	if(p%5==0)return 0;
	if(p%7==0)return 0;
	if(p%11==0)return 0;
	
	for(j=2;j<=(int)space;j++){
		if(p%j==0){
			return 0;
		}
	}
			
	if(j==p)return 1;
	
}


int count_factors(int num, int prime){

	int cnt = 0;
	while(num%prime==0){
		num=num/prime;
		cnt++;
	}
	
	return cnt;
}


int find_divisors(int input, int primes[], int divisor_cnt[], bool output){

	int factor_cnt = 0;
	for(int i=0;i<TOTAL_PRIMES;i++){
		factor_cnt = count_factors(input, primes[i] );
		if(factor_cnt>0){
			divisor_cnt[i]=factor_cnt;
			//printf("%d => %d %d\n", input, primes[i], factor_cnt);
		
		}
		if(primes[i]>input)break;
	}
	
	int prod=1;
	int dcnt=0;
	for(int i=0;i<TOTAL_PRIMES;i++){
		dcnt=divisor_cnt[i];
		if(dcnt>0){
			prod = (dcnt+1)*prod;
			if(output==true)printf("%d => %d %d\n", input, primes[i], divisor_cnt[i]);
		
		}
		if(primes[i]>input)break;
	}

	return prod;


}

int dump_arrays(int primes[], int divisor_cnt[]){

	for(int i=0;i<TOTAL_PRIMES;i++){
		if(divisor_cnt[i]>0)
			printf("%d %d\n", primes[i], divisor_cnt[i]);
	}

	return 0;
}


double  sum_divisors(int num, int primes[], int divisor_cnt[]){
	
	double sum=0;

	
	find_divisors(num, primes, divisor_cnt, false);
	
	for(int i = 0;i<TOTAL_PRIMES;i++){
		sum+=divisor_cnt[i]*primes[i];
	}
	
	
	if(num>3){
		sum+=1;
		sum+=num;
	}
	
	return sum;
}

double sum_divisors2(int num, int primes[], int divisor_cnt[]){
	
	double sum=0;
	
	find_divisors(num, primes, divisor_cnt, false);
	dump_arrays(primes, divisor_cnt);
	
	for(int i = 0;i<TOTAL_PRIMES;i++){
		
		if(divisor_cnt[i]>0){
			// o(p^a) = (p^(a+1)-1)/(p-1)
			int p = primes[i];
			int a = divisor_cnt[i];
			
			double y = pow(p, (a+1));
			y-=1;
			y=y/(p-1);

			if(sum!=0)
				sum=sum*y;
			else
				sum=y;
		}
	}
	
/*	
	if(num>3){
		sum+=1;
		sum+=num;
	}
*/	
	return sum;
}

int main(int argc, char* argv[])
{

	
	// build array of primes
	int primes[TOTAL_PRIMES] = {2, 3, 5, 7, 9, 11, 13, 17, 19};
	int divisor_cnt[TOTAL_PRIMES]={0};


	
	printf("Generating Primes\n");
	int pcnt = 0;
	int icnt = 0;
	while(pcnt<TOTAL_PRIMES){
		if(prime_check(icnt)){
			primes[pcnt]=icnt;
			//printf("%d\n", primes[pcnt]);
			pcnt++;
		}
		icnt++;
	}
	
	printf("Primes Generated %d\n",TOTAL_PRIMES);
	//return 0;
	mpz_t total;
	mpz_t sum1;
	
	mpz_init(sum1);
	mpz_init(total);

	double max = pow(10, 3);
	//max = 100000;
	//max++;
	//max=4;	
	for(int i=1;i<max;i++){
		for(int j=1;j<max;j++){
			mpz_set_d(sum1, sum_divisors2(i*j, primes, divisor_cnt));		
			mpz_add(total, total, sum1);
			//printf("%d %d\n\n", i*j, sum );
	        	memset(divisor_cnt, 0, sizeof(int)*TOTAL_PRIMES);
		}
	}
	
	printf("total %s\n", mpz_get_str(NULL, 10, total));
	return 0;
	
	printf("%d %d %d\n", sizeof(long long), sizeof(long double), sizeof(long int));
	if(argc==2){
		int input=0;
		input = atoi(argv[1]);
			
		printf("total %d\n", find_divisors(input, primes, divisor_cnt, true));
		printf("sum %.0f\n", sum_divisors2(input, primes, divisor_cnt));
		return 0;
	}

	
	int sum = 0;
	for(int i=1; i<100000;i++){ //100000
		sum = sum+i;
	        memset(divisor_cnt, 0, sizeof(int)*TOTAL_PRIMES);
		//for(int j=0;j<TOTAL_PRIMES;j++)divisor_cnt[j]=0;
		
		int total = find_divisors(sum, primes, divisor_cnt, false);
		
		//printf("%d %d\n", sum, total);
		if(total>500){
			printf("%d %d\n", sum, total);
			break;
		}
	}
	
	
	
	return 0;
}

