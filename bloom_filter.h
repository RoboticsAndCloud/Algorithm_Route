#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H
#include "std_bf.h"
#include "Fib.h"
#include <string>
#include <iostream>
#include <bitset>

using namespace std;


int trans_strip_to_intip(char *file_str_ip);

void test_bf_query(StandardBF *sBF1);


void bf_query(char *filename, CFib *MergedFib,StandardBF **a_BloomFilter, EXSBF *extraBF);
void bf_query2(char *filename, CFib *MergedFib, StandardBF **a_BloomFilter, EXSBF *extraBF);
void bf_query24(char *filename, CFib *MergedFib, StandardBF **a_BloomFilter, EXSBF *extraBF);

// extrabf k=8, bf,k= 8-16
int construct_bloom_filter(int kk,char *filename,  StandardBF **a_BloomFilter, EXSBF *extraBF,const int merged_level_node_num[], int bf_num);


// bf = 8,extrak = 4,8,16
int construct_bloom_filter2(int case_index,int kk, char *filename, StandardBF **a_BloomFilter, EXSBF *extraBF, const int merged_level_node_num[], int bf_num);

int construct_bloom_filter24(int case_index, int kk, char *filename, StandardBF **a_BloomFilter, EXSBF *extraBF, const int merged_level_node_num[], int bf_num);



int bloom_filter(char  *filename);
void bloom_filter_false_positive_ratio();
void false_positive_std_qw_bf(char *filename);
void test_false_positive_qwstdbf(char *filename);

void q_test_false_positive_qwstdbf(char *filename); // qw test ,create file "q w ratio"
void test_false_positive_stdbf(char *filename); 

void false_positive_std_bf(char *filename); // std bf 
/*
#include<stdio.h>
#include<time.h>
int main()
{
clock_t start,end;

start = clock(); //开始时，取得开始时间。

//你自己的代码

end = clock();   //结束时，取得结束时间

printf("Run time: %lf S",(double)(end-start)/CLOCKS_PER_SEC);
return 0;

}
*/

#endif