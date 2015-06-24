/*
*   http://oeis.org/wiki/Sum_of_divisors_function
*   The GNU Multiple Precision Arithmetic Library
*   S(N) = ∑1≤i≤N ∑1≤j≤N d(i·j)
*   S(10^11) 
*/


#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include<malloc.h>
//#include "gmp.h"
#include<pthread.h>
#include<time.h>


// 76576500 576
// 56274542353549676442063519359122792968697378
// 1000000
#define TOTAL_PRIMES 16384
#define TOTAL_THREADS 4


//primes[TOTAL_PRIMES]={0};

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

double pow_by_squaring(double x, double n){

	if(n==0)return 1;
	if(n==1)return x;
	if((int)n%2==0)return pow_by_squaring(x*x, n/2);
	return x * pow_by_squaring(x*x, (n-1)/2);


}

double fast_fmod(double x,double y){ 
	double a, b; 
	
	b=x/y;
	
	return (b-(int)b)*y; 

}

int count_factors(unsigned long long num, int prime){

	int cnt = 0;
	//if(num==0)return 0;
	while(num%prime==0){
		num=num/prime;
		cnt++;
	}
	
	return cnt;
}


int find_divisors(unsigned long long input, int primes[], int divisor_cnt[], bool output){

	int factor_cnt = 0;
	int prime_cnt = 0;
	//if(input>1000000000)return 0;
	int index_last_found_prime = 0;

	for(int i=0;i<TOTAL_PRIMES;i++){
		factor_cnt = count_factors(input, primes[i] );
		prime_cnt++;
		if(factor_cnt>0){
			index_last_found_prime = i+1;
			divisor_cnt[i]=factor_cnt;
			//prime_cnt++;
			//printf("%d => %d %d\n", input, primes[i], factor_cnt);
			
				
			//if(primes[i]>10)printf("LP = %d %d\n", i, primes[i]);
		
		}
		if(primes[i]>input)break;
	}

	return index_last_found_prime;
		
	int prod=1;
	int dcnt=0;
	for(int i=0;i<TOTAL_PRIMES;i++){
		dcnt=divisor_cnt[i];
		if(dcnt>0){
			prod = (dcnt+1)*prod;
			if(output==true)printf("%lld => %d %d\n", input, primes[i], divisor_cnt[i]);
				
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

double sum_divisors2(unsigned long long num, int primes[], int divisor_cnt[]){
	
	double sum=0;
	int cnt = 0;        
	int index_last_found_prime = 0;
	/*
	if(num==1){
		sum+=1;
		//sum+=num;
		return sum;
	}
	*/

			
	index_last_found_prime  = find_divisors(num, primes, divisor_cnt, false);
	//cnt  = find_divisors(num, primes, divisor_cnt, false);
	//dump_arrays(primes, divisor_cnt);

	for(int i = 0;i<index_last_found_prime;i++){ // was TOTAL_PRIMES
		//if(i>num)break;
		if(divisor_cnt[i]>0){
			//o(p^a) = (p^(a+1)-1)/(p-1)
						
			int p = primes[i];
			int a = divisor_cnt[i];
			
			//double y = pow(p, (a+1)); // 1.13s for 10^3
			//double y = exp(log(p)*(a+1)); // 1.29s for 10^3 
			
			double y = pow_by_squaring(p, (a+1)); // 1.10s for 10^3
			y-=1;
			y=y/(p-1);

			if(sum!=0)
				sum=sum*y;
			else
				sum=y;
		}
		//if(sum>1000000)break;	
		//if(i>cnt)break;
	}
	
	
	if(num==1){
		sum+=1;
		//sum+=num;
	}
	
	
	return sum;
}


#define NUM_THREADS 4

typedef struct tdata{
	int tid;
	double start;
	double finish;
	int divisor_cnt[TOTAL_PRIMES];
	int *primes;
	double result;
} thread_data;

thread_data data[NUM_THREADS];


void* worker(void *input){
	
	thread_data *data = (thread_data*) input;
	int tid = data->tid;
	int primes[TOTAL_PRIMES] = {0};
	
	int divisor_cnt[TOTAL_PRIMES]={0};
	
	double  total=0;
	double  sum1=0;

	double max = data->finish;
	double start = data->start;//pow(10, 4)+1;
	
	int pcnt = 0;
	int icnt = 0;
	while(pcnt<TOTAL_PRIMES){
		if(prime_check(icnt)){
			primes[pcnt]=icnt;
			//printf("%d %d\n", pcnt, primes[pcnt]);
			pcnt++;
		}
		icnt++;
	}

	printf("%d %.f %.f\n", tid, start, max);

	/*
	for(int i = 0; i<TOTAL_PRIMES;i++){
		printf("%d\n", primes[i]);
	}
	*/

	
	int complete = max*.01;
	for(int i = start;i<max+1;i++){
		
		//if(i>100)break;
		for(int j=1;j<max+1;j++){
			//printf("%d %d \n", i, j);	
			unsigned long long t = i*j;	
				
			if(j==i){
				
				sum1 = sum_divisors2(t, primes, divisor_cnt);		
				total+=sum1;
	        		memset(divisor_cnt, 0, sizeof(int)*TOTAL_PRIMES);
				break;
			}
			
			
			
			sum1 = sum_divisors2(t, primes, divisor_cnt);		
				
			//sum1 = sum_divisors2(i, primes, divisor_cnt);		
			sum1*=2;
			total+=sum1;
			//total=fast_fmod(total, 100000000)+sum1;
			
			/*
			if(total>10000000000){
				// 10^9 42*881
				// 10^10 133*986
				printf("%d %d %.f\n", i, j, total);
				exit(1);
			}
			*/
			//printf("%d %f\n\n", i*j, sum1 );
	        	memset(divisor_cnt, 0, sizeof(int)*TOTAL_PRIMES);
			//total+=1;
			
		}
		       	
		//if((int)i%complete==0){
		//	printf("%.0f%c complete\n", i/max*100, '%');
			/*
			psedtime = clock();
			double etime = elapsedtime - starttime;
			double msecs = (etime/CLOCKS_PER_SEC);
			double esecs = etime/CLOCKS_PER_SEC;
			double min = (esecs/60);
			double hrs = (esecs/3600);
			printf("%02.f:%02.f:%05.2f \n", hrs, min, msecs);
			*/
			//printf("%.f:%.f:%.f\n ", hrs, min, sec);
		//}
		
	}		

		
	printf("%d total %.0f %.0f\n", tid, total, fmod(total, 1000000000));

	data->result = total;

	pthread_exit((void*)input);
	
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
			//printf("%d %d\n", pcnt, primes[pcnt]);
			pcnt++;
		}
		icnt++;
	}
	//return 0;	
	clock_t starttime;
	clock_t elapsedtime;
	
	starttime = clock();
		

	printf("Primes Generated %d\n",TOTAL_PRIMES);

	/*
	double x = pow(10, 9);
	double y = 563576517282;
	double z = pow(x, y);	
	printf("%f\n", z);

	return 0;
	*/
	double  total=0;

	double max = pow(10, 3);
	double start = 1;//pow(10, 4)+1;
	
	pthread_t threads[NUM_THREADS];
	void *thread_status;
	pthread_attr_t attr;
		
	pthread_attr_init(&attr);
   	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	//divide work
	struct work_piece{
		double begin;
		double end;
	};

	struct work_piece pieces[NUM_THREADS];
	double work_total = max;
	double work_single_size = max / NUM_THREADS;

		
	pieces[0].begin = start;
	pieces[0].end = work_single_size;
	
	pieces[1].begin = work_single_size+1;
	pieces[1].end = work_single_size*2;
	
	pieces[2].begin = work_single_size*2+1;
	pieces[2].end = work_single_size*3;
	
	pieces[3].begin = work_single_size*3+1;
	pieces[3].end = work_single_size*4;
	
	/*
	pieces[0].begin = 50001;
	pieces[0].end = 58000;
	
	pieces[1].begin = 58001;
	pieces[1].end = 66000;
	
	pieces[2].begin = 66001;
	pieces[2].end = 72000;
	
	pieces[3].begin = 72001;
	pieces[3].end = 75000;
	*/
	// create threads	
	
	for(int i=0;i<NUM_THREADS;i++){
		
		data[i].start = pieces[i].begin;
		data[i].finish = pieces[i].end;
		data[i].tid = i;
		data[i].primes = primes;
		if(pthread_create(&threads[i], NULL, worker, (void*) &data[i])){
			printf("Error Creating thread %d \n", i);
		}
	}
	
	// join threads

	pthread_attr_destroy(&attr);

	for(int i = 0; i<NUM_THREADS;i++){
		if(pthread_join(threads[i], &thread_status)){
			printf("Error joing thread %d\n", i);
		}

	}
	
	total = 0;
	for(int i = 0;i<NUM_THREADS;i++){
		total +=data[i].result;
	}

	printf("Total %.f\n", total);

	pthread_exit(thread_status);
	return 0;
}

