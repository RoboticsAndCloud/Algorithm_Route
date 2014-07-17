#include <stdio.h>
//#include <stdlib.h>
#include "std_bf.h"
#include <fstream>
#include <windows.h>
#include <math.h>
#include <string>
#include <time.h>
#include <iostream>
#include <set>
#include <iomanip>
#include <cstring>
//using namespace std;

#include "bloom_filter.h"
#include "Fib.h"

extern bitset<65536> bit_array;

#define PREFIX_NUM	20
#define PREFIX_LEN 5
#define TEST_PREFIX_LEN 4
#define LINE_LEN 100
#define BF_NUM 16

#define TEST_N 1000
#define TEST_K 4
#define Test_M 400
#define MEMORY_LEN 64 // zichang
unsigned int k_array_1[] = { 5, 5, 6, 6, 7, 7, 8, 9, 9, 10, 10, 10, 11, 11, 12, 12 }; // case 1
unsigned int k_array_2[] = { 6, 6, 6, 6, 6, 6, 6, 11, 6, 6, 6, 6, 6, 6, 6, 6 };     // case 2
void bloom_filter_false_positive_ratio()
{
	double k = 1;
	//double m_n = k / ln(2);
	double m_n = 2;
	double false_positive_ratio = pow((1-exp(-k/m_n)),k);
	for (m_n = 2; m_n <= 20; m_n++)
	{
		cout << "m/n k_pre false_positive k =1 k =2 k=3 k=4 k=5 k=6 k=7 k=8 k=9 k=10 k=11 k=12 k=13 k=14 k=15 k=16" << endl;
		cout << m_n<<" ";
		cout << pow(0.5,m_n*log(2)) << " ";
		false_positive_ratio = pow((1 - exp(-k / m_n)), k);
		cout << false_positive_ratio << " ";

		for (k = 1; k<=m_n && k <= 16; k++)
		{
				
			false_positive_ratio = pow((1 - exp(-k / m_n)), k);
			cout << false_positive_ratio << " ";
		
		}
		cout << endl;
	
	}

	cout << endl;

	

}

void test_qwbf_query(QWStandardBF *sBF1,char *filename)
{
	ifstream fin;
	fin.open(filename);
	int ip;
	int j=0;
	unsigned int total_num = 0;
	unsigned int find_num = 0;
	unsigned char prefix[TEST_PREFIX_LEN];
	if (!fin.is_open())
	{
		cout << "fail" << endl;
		return;
	}

	while (!fin.eof())
	{

		memset(prefix, 0, sizeof(prefix));

		fin >> ip;
		total_num++;
		prefix[0] = ip >> 24;
		prefix[1] = (ip << 8) >> 24;
		prefix[2] = (ip << 16) >> 24;
		prefix[3] = (ip << 24) >> 24;

		if (sBF1->query(prefix, TEST_PREFIX_LEN))
		{
			find_num++;
			//cout <<find_num<<" " <<ip<< endl;
		}
		else
		{
			
		}
	}
	fin.close();
	cout << "total num: " << total_num << " find num : " << find_num << endl;
}

void  test_false_positive_qwstdbf(char *filename)
{

	//	clock_t start, end;
	//	start = clock(); //开始时，取得开始时间。
	time_t t_s, t_e;
	//time_t s_cbf, s_search;
	time(&t_s);
	int n = TEST_N;
	unsigned char prefix[TEST_PREFIX_LEN];
	unsigned int k = TEST_K;
	unsigned int m = (int)(k * n / log(2));
	//unsigned int m = Test_M;
	QWStandardBF *sBF1 = new QWStandardBF();

	double fp_test;
	double fp_theor;
	double ratio;

	unsigned int total_num;
	unsigned int true_num;
	unsigned int false_positive_num;
	

	//ofstream fout1("result_qfrom1to_n_k6_1000k_nomal");
	//ofstream fout("result_qfrom1to_n_k6_1000k_ratio_final");
	ofstream fout1("a.txt", ios::app);
	ofstream fout("b.txt", ios::app);

	int testn[] = { 1000, 2000, 5000, 10000, 20000 };

	for (int tmpn = 0; tmpn < 1; tmpn++)
	{
		cout << "###################################################################" << endl;
		cout << "test n =" << testn[tmpn] << endl;
		fout1 << "###################################################################" << endl;
		fout1 << "test n =" << testn[tmpn] << endl;
		fout << "###################################################################" << endl;
		fout << "test n =" << testn[tmpn] << endl;
		
		n = testn[tmpn];
		
		for (int kk = 4; kk <= 4; kk++)
		{

			k = kk;

			
			m = (int)((k * n / log(2)) + 0.5);
		
			

			total_num = 0;
			false_positive_num = 0;
			true_num = 0;


			fout1 << "-----------------------------------------------" << endl;
			fout1 << "######### test k= " << k << " #############" << endl;

			fout << "-----------------------------------------------" << endl;
			fout << "######### test k= " << k << " #############" << endl;
			fout << setw(7) << setiosflags(ios::left) << "q";
			fout << setw(14) << "fp_test";
			fout << setw(14) << "theor";
			fout << setw(14) << "ratio" << endl;
			for (int q = 2800; q < 2864; q++)
			{
				sBF1->initial(m, k, n, q);


				int ip;
				int j = 0;
				ifstream fin(filename);
				while (!fin.eof() && j < n)
				{
					memset(prefix, 0, sizeof(prefix));
					fin >> ip;

					prefix[0] = ip >> 24;
					prefix[1] = (ip << 8) >> 24;
					prefix[2] = (ip << 16) >> 24;
					prefix[3] = (ip << 24) >> 24;

					sBF1->insert(prefix, TEST_PREFIX_LEN);
					j++;


				}//while

				fin.seekg(0, ios::beg); //set read pointer at the file begin
				j = 0;
				fp_test = 0;
				ratio = 0;
				total_num = 0;
				true_num = 0;
				false_positive_num = 0;
				while (!fin.eof())
				{

					memset(prefix, 0, sizeof(prefix));
					fin >> ip;


					prefix[0] = ip >> 24;
					prefix[1] = (ip << 8) >> 24;
					prefix[2] = (ip << 16) >> 24;
					prefix[3] = (ip << 24) >> 24;


					if (total_num < unsigned(n))
					{
						sBF1->q_w_create(prefix, TEST_PREFIX_LEN);
					}
					else  //search
					{
						if (sBF1->query(prefix, TEST_PREFIX_LEN))
						{
							false_positive_num++;
						}
						else
						{
							true_num++;
						}

					}

					total_num++;

				}//while

				fin.close();


				time(&t_e);

				fp_test = double(false_positive_num) / double(total_num);
				fp_theor = pow((1 - pow(1 - 1 / double(m), double(k*n))), (double)k);
				ratio = abs(1 - fp_test / fp_theor);
				/*cout << "The time of constructing Bloom Filter " << t_s << " " << t_e << "  " << t_e - t_s << endl;


				cout << "k = " << sBF1->Get_bf_k() << endl;
				cout << "m = " << sBF1->Get_bf_m() << endl;
				cout << "n = " << sBF1->Get_bf_n() << endl;
				cout << "q = " << sBF1->Get_bf_q() << endl;
				cout << "bit_1_num = " << sBF1->Get_bf_base_num_of_bit_1() << endl;
				cout << "ratio of m bit " <<  (double)sBF1->Get_bf_base_num_of_bit_1()/ (double)(m/8+1)*8 << endl;
				*/
				fout1 << "k = " << sBF1->Get_bf_k() << endl;
				fout1 << "m = " << sBF1->Get_bf_m() << endl;
				fout1 << "n = " << sBF1->Get_bf_n() << endl;
				fout1 << "q = " << sBF1->Get_bf_q() << endl;
				fout1 << "bit_1_num = " << sBF1->Get_bf_base_num_of_bit_1() << endl;
				fout1 << "ratio of m bit " <<  (double)sBF1->Get_bf_base_num_of_bit_1()/ (double)(m/8+1)*8 << endl;

				//sBF1->show_bf_bit();
				//sBF1->showwindow();

				//test_qwbf_query(sBF1, "rrctest.txt");
				//sBF1->show_query();
				unsigned char *bfbase = sBF1->Get_bf_base();
				fout1 << "bf_bit: ";
				char bf_bit[9];
				for (int i = m / 8; i >= 0; i--)
				{
					//cout << bf_base[i] << " int "<<int(bf_base[i]) << endl;
					_itoa(bfbase[i], bf_bit, 2);
					fout1 << bf_bit << " ";
				}
				fout1 << endl;

				fout1 << "qw_work " << sBF1->Get_bf_qw_work() << " same w: " << sBF1->Get_bf_samew() << " less w: " << sBF1->Get_bf_lessw() << endl;

			//	cout << "qw_work " << sBF1->Get_bf_qw_work() << " same w: " << sBF1->Get_bf_samew() << " less w: " << sBF1->Get_bf_lessw() << endl;
				unsigned int *bfw = sBF1->Get_bf_w();

				fout1 << "w size: ";
			//	cout << "w size: ";
				for (unsigned int i = 0; i < k; i++)
				{

				//	cout << "w" << i << " " << bfw[i] << ends;
					fout1 << "w" << i << " " << bfw[i] << " ";
				}

				fout1 << endl;
				fout1 << "total_num: " << total_num << " bloom_filter_num: " << n << " true_num: " << true_num << " false_positive_num: " << false_positive_num << endl; //" approximation " << pow((1 - exp(-double(k*n) / double(m))), k)
				fout1 << " false_positive_ratio " << double(false_positive_num) / double(total_num) << " theoretical value: " << pow((1 - pow(1 - 1 / double(m), double(k*n))), (double)k) << " ratio " << ratio << endl;
				fout1 << "-------------------------------------------------------------------------------------------------------" << endl;



				/*cout << endl;
				cout << "total_num: " << total_num << " bloom_filter_num: " << n << " true_num: " << true_num << " false_positive_num: " << false_positive_num << endl; //" approximation " << pow((1 - exp(-double(k*n) / double(m))), k)
				cout << " false_positive_ratio " << double(false_positive_num) / double(total_num) << " theoretical value: " << pow((1 - pow(1 - 1 / double(m), double(k*n))), (double)k) << " ratio " << ratio << endl;
				*/
				cout << q << endl;
			//	cout << "-------------------------------------------------------------------------------------------------------" << endl;
				fout << setw(7) << setiosflags(ios::fixed) << setprecision(7) << setiosflags(ios::left) << q;
				fout << setw(14) << fp_test;
				fout << setw(14) << fp_theor;
				fout << setw(14) << ratio << endl;


			}// for q
		}// fro kkk

	}//for tmpn
	delete sBF1;
	fout.close();
	fout1.close();
	
}


void  q_test_false_positive_qwstdbf(char *filename)
{

	//	clock_t start, end;
	//	start = clock(); //开始时，取得开始时间。
	time_t t_s, t_e;
	//time_t s_cbf, s_search;
	time(&t_s);
	int n = TEST_N;
	unsigned char prefix[TEST_PREFIX_LEN];
	unsigned int k = TEST_K;
	unsigned int m = (int)(k * n / log(2));
	//unsigned int m = Test_M;
	QWStandardBF *sBF1 = new QWStandardBF();

	double fp_test;
	double fp_theor;
	double ratio;

	unsigned int total_num;
	unsigned int true_num;
	unsigned int false_positive_num;

	set<unsigned int> v;

	ifstream fin(filename);

	int ip;
	while (!fin.eof())
	{
		fin >> ip;
		//cout << ip << " ip ";
		v.insert(ip);
	}

	fin.close();
	set<unsigned int>::iterator begin = v.begin();
	set<unsigned int>::iterator end = v.end();


	//ofstream fout1("result_qfrom1to_n_k6_1000k_nomal");
	//ofstream fout("result_qfrom1to_n_k6_1000k_ratio_final");
	//ofstream fout1("a.txt", ios::app);
	ofstream fout("qw_100000.txt",ios::app);

	int testn[] = { 1000, 2000, 5000, 10000, 20000 };

	for (int tmpn = 0; tmpn < 1; tmpn++)
	{
		cout << "###################################################################" << endl;
		cout << "test n =" << testn[tmpn] << endl;
	//	fout1 << "###################################################################" << endl;
	//	fout1 << "test n =" << testn[tmpn] << endl;
		fout << "###################################################################" << endl;
		fout << "test n =" << testn[tmpn] << endl;

		fout << "-----------------------------------------------" << endl;
	//	fout << "######### test k= " << k << " #############" << endl;
		fout << setw(14) << setiosflags(ios::left) << setiosflags(ios::fixed) << setprecision(7) << "qw";
		fout << setw(14) << "4";
		fout << setw(14) << "5";
		fout << setw(14) << "6";
		fout << setw(14) << "7";
		fout << setw(14) << "8";
		fout << setw(14) << "9";
		fout << setw(14) << "10";
		fout << setw(14) << "11";
		fout << setw(14) << "12";
		fout << setw(14) << "13";
		fout << setw(14) << "14";
		fout << setw(14) << "15";
		fout << setw(14) << "16" << endl;
		fout << setw(14) << "therofp";
		for (int i = 4; i <= 16; i++)
		{
			k = i;
			m = (int)((k * n / log(2)) + 0.5);
			fp_theor = pow((1 - pow(1 - 1 / double(m), double(k*n))), (double)k);
			fout << setw(14) << setiosflags(ios::left) << setiosflags(ios::fixed) << setprecision(7);
			fout << setw(14) << fp_theor;

		}
		fout << endl;

		n = testn[tmpn];

		//for (int kk = 4; kk <= 4; kk++)
		for (int q = 1; q <= MEMORY_LEN ;q++)
		{

			
			total_num = 0;
			false_positive_num = 0;
			true_num = 0;


		//	fout1 << "-----------------------------------------------" << endl;
		//	fout1 << "######### test k= " << k << " #############" << endl;

			fout << setw(14) << q;

			for (int kk = 4; kk < 17; kk++)
			{

				k = kk;
				m = (int)((k * n / log(2)) + 0.5);
				sBF1->initial(m, k, n, q);


				int ip;
				int j = 0;
				//ifstream fin(filename);
				//while (!fin.eof() && j < n)

				for (begin = v.begin(); begin != end; begin++)
				{
					memset(prefix, 0, sizeof(prefix));
					//fin >> ip;
					ip = *begin;

					prefix[0] = ip >> 24;
					prefix[1] = (ip << 8) >> 24;
					prefix[2] = (ip << 16) >> 24;
					prefix[3] = (ip << 24) >> 24;

					sBF1->insert(prefix, TEST_PREFIX_LEN);
					j++;
					if (j >= n)
						break;


				}//while

			//	fin.seekg(0, ios::beg);
				j = 0;
				fp_test = 0;
				ratio = 0;
				total_num = 0;
				true_num = 0;
				false_positive_num = 0;
				//while (!fin.eof())
				for (begin = v.begin(); begin != end; begin++)
				{

					memset(prefix, 0, sizeof(prefix));
				//	fin >> ip;
					ip = *begin;

					prefix[0] = ip >> 24;
					prefix[1] = (ip << 8) >> 24;
					prefix[2] = (ip << 16) >> 24;
					prefix[3] = (ip << 24) >> 24;


					if (total_num < unsigned(n))
					{
						sBF1->q_w_create(prefix, TEST_PREFIX_LEN);
					}
					else  //search
					{
						if (sBF1->query(prefix, TEST_PREFIX_LEN))
						{
							false_positive_num++;
						}
						else
						{
							true_num++;
						}

					}

					total_num++;

				}//while

				fin.close();


				time(&t_e);

				fp_test = double(false_positive_num) / double(total_num);
			//	fp_theor = pow((1 - pow(1 - 1 / double(m), double(k*n))), (double)k);
			//	ratio = abs(1 - fp_test / fp_theor);
				/*cout << "The time of constructing Bloom Filter " << t_s << " " << t_e << "  " << t_e - t_s << endl;


				cout << "k = " << sBF1->Get_bf_k() << endl;
				cout << "m = " << sBF1->Get_bf_m() << endl;
				cout << "n = " << sBF1->Get_bf_n() << endl;
				cout << "q = " << sBF1->Get_bf_q() << endl;
				cout << "bit_1_num = " << sBF1->Get_bf_base_num_of_bit_1() << endl;
				cout << "ratio of m bit " <<  (double)sBF1->Get_bf_base_num_of_bit_1()/ (double)(m/8+1)*8 << endl;
				*/
				/*
				fout1 << "k = " << sBF1->Get_bf_k() << endl;
				fout1 << "m = " << sBF1->Get_bf_m() << endl;
				fout1 << "n = " << sBF1->Get_bf_n() << endl;
				fout1 << "q = " << sBF1->Get_bf_q() << endl;
				fout1 << "bit_1_num = " << sBF1->Get_bf_base_num_of_bit_1() << endl;
				fout1 << "ratio of m bit " << (double)sBF1->Get_bf_base_num_of_bit_1() / (double)(m / 8 + 1) * 8 << endl;
				
				//sBF1->show_bf_bit();
				//sBF1->showwindow();

				//test_qwbf_query(sBF1, "rrctest.txt");
				//sBF1->show_query();
				unsigned char *bfbase = sBF1->Get_bf_base();
				fout1 << "bf_bit: ";
				char bf_bit[9];
				for (int i = m / 8; i >= 0; i--)
				{
					//cout << bf_base[i] << " int "<<int(bf_base[i]) << endl;
					_itoa(bfbase[i], bf_bit, 2);
					fout1 << bf_bit << " ";
				}
				fout1 << endl;

				fout1 << "qw_work " << sBF1->Get_bf_qw_work() << " same w: " << sBF1->Get_bf_samew() << " less w: " << sBF1->Get_bf_lessw() << endl;

				//	cout << "qw_work " << sBF1->Get_bf_qw_work() << " same w: " << sBF1->Get_bf_samew() << " less w: " << sBF1->Get_bf_lessw() << endl;
				unsigned int *bfw = sBF1->Get_bf_w();

				fout1 << "w size: ";
				//	cout << "w size: ";
				for (unsigned int i = 0; i < k; i++)
				{

					//	cout << "w" << i << " " << bfw[i] << ends;
					fout1 << "w" << i << " " << bfw[i] << " ";
				}

				fout1 << endl;
				fout1 << "total_num: " << total_num << " bloom_filter_num: " << n << " true_num: " << true_num << " false_positive_num: " << false_positive_num << endl; //" approximation " << pow((1 - exp(-double(k*n) / double(m))), k)
				fout1 << " false_positive_ratio " << double(false_positive_num) / double(total_num) << " theoretical value: " << pow((1 - pow(1 - 1 / double(m), double(k*n))), (double)k) << " ratio " << ratio << endl;
				fout1 << "-------------------------------------------------------------------------------------------------------" << endl;


				*/
				/*cout << endl;
				cout << "total_num: " << total_num << " bloom_filter_num: " << n << " true_num: " << true_num << " false_positive_num: " << false_positive_num << endl; //" approximation " << pow((1 - exp(-double(k*n) / double(m))), k)
				cout << " false_positive_ratio " << double(false_positive_num) / double(total_num) << " theoretical value: " << pow((1 - pow(1 - 1 / double(m), double(k*n))), (double)k) << " ratio " << ratio << endl;
				*/
				cout <<"q "<< q <<" k " <<k<< endl;
				//	cout << "-------------------------------------------------------------------------------------------------------" << endl;
			//	fout << setw(12) << setiosflags(ios::fixed) << setprecision(7) << setiosflags(ios::left) << q;
				fout << setiosflags(ios::left) << setiosflags(ios::fixed) << setprecision(7);
				fout << setw(14) << fp_test;
			//	fout << setw(12) << fp_theor;
			//	fout << setw(12) << ratio << endl;


			}// for kk
			fout << endl;
		}// fro q

	}//for tmpn
	delete sBF1;
	fout.close();
	//fout1.close();

}













void test_stdbf_query(StandardBF *sBF1, char *filename)
{
	ifstream fin;
	fin.open(filename);
	int ip;
	int j = 0;
	unsigned int total_num = 0;
	unsigned int find_num = 0;
	unsigned char prefix[TEST_PREFIX_LEN];
	if (!fin.is_open())
	{
		cout << "fail" << endl;
		return;
	}

	while (!fin.eof())
	{

		memset(prefix, 0, sizeof(prefix));

		fin >> ip;
		total_num++;
		prefix[0] = ip >> 24;
		prefix[1] = (ip << 8) >> 24;
		prefix[2] = (ip << 16) >> 24;
		prefix[3] = (ip << 24) >> 24;

		
		if (sBF1->query(prefix, TEST_PREFIX_LEN))
		{
			find_num++;
			//cout <<find_num<<" " <<ip<< endl;
		}
		else
		{

		}
	}
	fin.close();
	cout << "total num: " << total_num << " find num : " << find_num << endl;
}

void test_false_positive_stdbf(char *filename)
{

	//	clock_t start, end;
	//	start = clock(); //开始时，取得开始时间。
	time_t t_s, t_e;
	time(&t_s);
	int n = TEST_N;
	unsigned char prefix[TEST_PREFIX_LEN];
	unsigned int k = TEST_K;
	unsigned int m = (int)(k * n / log(2));
	//unsigned int m = Test_M;
	StandardBF *sBF1 = new StandardBF(m, k);
	
	int ip;
	int j = 0;
	ifstream fin(filename);

	unsigned int total_num;
	int true_num;
	int false_positive_num;
	double fp_test;
	double ratio;
	double fp_theor;
	fp_test = 0;
	ratio = 0;
	total_num = 0;
	true_num = 0;
	false_positive_num = 0;

	while (!fin.eof() )
	{

		memset(prefix, 0, sizeof(prefix));
		fin >> ip;

		prefix[0] = ip >> 24;
		prefix[1] = (ip << 8) >> 24;
		prefix[2] = (ip << 16) >> 24;
		prefix[3] = (ip << 24) >> 24;

		
		
	
		if (total_num < unsigned(n))
		{
			sBF1->insert(prefix, TEST_PREFIX_LEN);
		}
		else  //search
		{
			if (sBF1->query(prefix, TEST_PREFIX_LEN))
			{
				false_positive_num++;
			}
			else
			{
				true_num++;
			}
		}

		total_num++;

	}//while
	fin.close();


	time(&t_e);
	fp_test = double(false_positive_num) / double(total_num);
	fp_theor = pow((1 - pow(1 - 1 / double(m), double(k*n))), (double)k);
	ratio = abs(1 - fp_test / fp_theor);
	cout << "The time of constructing Bloom Filter " << t_s << " " << t_e << "  " << t_e - t_s << endl;


	cout << "k = " << sBF1->Get_bf_k() << endl;
	cout << "m = " << sBF1->Get_bf_m() << endl;
	cout << "n = " << sBF1->Get_bf_n() << endl;
	
	cout << "total_num: " << total_num << " bloom_filter_num: " << n << " true_num: " << true_num << " false_positive_num: " << false_positive_num << endl; //" approximation " << pow((1 - exp(-double(k*n) / double(m))), k)
	cout << " false_positive_ratio " << double(false_positive_num) / double(total_num) << " theoretical value: " << pow((1 - pow(1 - 1 / double(m), double(k*n))), (double)k) << " ratio " << ratio << endl;

	

}




/*
  false positive qw
  ip type: long,int  
*/
void false_positive_std_qw_bf(char *filename)
{		
	cout << "starting test false positive..." << endl;
	if (filename == NULL)
		return;
	time_t t_s, t_e;
	time(&t_s);
    
	unsigned char prefix[TEST_PREFIX_LEN];

	 //k 6-10
	int k = 6;
	int n =10000;
	int m = (int)((k * n / log(2))+0.5);
	//StandardBF *sBF1 = new StandardBF(m, k);
	QWStandardBF *sBF1 = new QWStandardBF(m, k,n,1);
	//ifstream fin("ipprefix.4bytes");
	//ifstream fin(filename);
	unsigned int ip;
	
	//char sPrefix[19];

	unsigned int total_num=0;
	unsigned int false_positive_num=0;
	unsigned int true_num = 0;

	double fp_test;
	double fp_theor;
	double ratio;

	set<unsigned int> v;

	ifstream fin(filename);
	
	while (!fin.eof())
	{
		fin >> ip;
		//cout << ip << " ip ";
		v.insert(ip);
	}
	
	fin.close();
	

	
	
	set<unsigned int>::iterator begin = v.begin();
	set<unsigned int>::iterator end = v.end();

	
	ofstream fout1("rrctest100000k.txt"); 
	
	for (; begin != end; begin++)
	{
		fout1 << *begin << endl;
	}
	fout1.close();
	return;
	
	ofstream fout("result_qwstdbf_q_from1to20final.txt"); ///just ratio
	ofstream fout2("result_qwstdbf_q_from1to20.txt"); // all 

	time(&t_e);
	cout << " construct bloom filter time : " << t_e - t_s << endl;
	
	fout << " construct bloom filter time : " << t_e - t_s << endl;
	fout2 << " construct bloom filter time : " << t_e - t_s << endl;

	int testn[] = {1000,2000,5000,10000,20000};
	for (int j = 0; j < 5; j++)
	{
		fout <<"-----------------------------------------------"<< endl;
		fout << "######### test n= " << testn[j] << " #############" << endl;
		fout2 << "-----------------------------------------------" << endl;
		fout2 << "######### test n= " << testn[j] << " #############" << endl;
		fout << setw(7) << setiosflags(ios::left) << "k";
		fout << setw(14) << "fp_test";
		fout << setw(14) << "theor";
		fout << setw(14) << "ratio" << endl;




		for (int i = 4; i <= 16; i++)
		{
			time(&t_s);
			k = i;

			n = testn[j];
			m = (int)((k * n / log(2)) + 0.5);
			//sBF1->initial(m, k);
			sBF1->initial(m, k,n,1);
			
			total_num = 0;
			false_positive_num = 0;
			true_num = 0;



			//ifstream fin("rrctest_out.txt");

			for (begin = v.begin(); begin != end; begin++)
			{

				//while (!fin.eof())
				//{
				memset(prefix, 0, sizeof(prefix));
				//fin >> ip;
				ip = *begin;
				//cout << ip << "  ";
				//	sprintf(sPrefix,"%u.%u.%u.%u", (ip >> 24), (ip << 8) >> 24, (ip << 16) >> 24, (ip << 24) >> 24);
				//	cout << sPrefix << endl;
				prefix[0] = ip >> 24;
				prefix[1] = (ip << 8) >> 24;
				prefix[2] = (ip << 16) >> 24;
				prefix[3] = (ip << 24) >> 24;
				//	cout << "perfix " << (int)prefix[0] << " " << (int)prefix[1] << " " << (int)prefix[2] << " " << (int)prefix[3] << endl;
				if (total_num < unsigned(n))
				{
					sBF1->insert(prefix, TEST_PREFIX_LEN);
				}
				else
				{
					break;
				}
				total_num++;

			}//insert
			
			
			total_num = 0;
			false_positive_num = 0;
			true_num = 0;
			for (begin = v.begin(); begin != end; begin++)
			{
				memset(prefix, 0, sizeof(prefix));
				//fin >> ip;
				ip = *begin;
				//cout << ip << "  ";
				//	sprintf(sPrefix,"%u.%u.%u.%u", (ip >> 24), (ip << 8) >> 24, (ip << 16) >> 24, (ip << 24) >> 24);
				//	cout << sPrefix << endl;
				prefix[0] = ip >> 24;
				prefix[1] = (ip << 8) >> 24;
				prefix[2] = (ip << 16) >> 24;
				prefix[3] = (ip << 24) >> 24;
				//	cout << "perfix " << (int)prefix[0] << " " << (int)prefix[1] << " " << (int)prefix[2] << " " << (int)prefix[3] << endl;
				if (total_num < unsigned(n))
				{
					sBF1->q_w_create(prefix, TEST_PREFIX_LEN);
				}
				else  //search
				{					
					if (sBF1->query(prefix, TEST_PREFIX_LEN))
					{
						false_positive_num++;
					}
					else
					{
						true_num++;
					}

				}

				total_num++;
			}// for set iterator
			//	}//while
			//	fin.close();


			
			time(&t_e);
			fp_test = double(false_positive_num) / double(total_num);
			fp_theor = pow((1 - pow(1 - 1 / double(m), double(k*n))), (double)k);
			ratio = abs(1-fp_test/fp_theor);
			
			cout << "-----------------------------------------------------------------------------------" << endl << endl;
			cout << "The time of test  Bloom Filter start: " << t_s << " end: " << t_e << " total: " << t_e - t_s << endl;
			cout << "k = " << sBF1->Get_bf_k() << endl;
			cout << "m = " << sBF1->Get_bf_m() << endl;
			cout << "n = " << sBF1->Get_bf_n() << endl;
			cout << "q = " << sBF1->Get_bf_q() << endl;
			cout << "total_num: " << total_num << " bloom_filter_num: " << n << " true_num: " << true_num << " false_positive_num: " << false_positive_num << endl; //" approximation " << pow((1 - exp(-double(k*n) / double(m))), k)
			cout << " false_positive_ratio " << double(false_positive_num) / double(total_num) << " theoretical value: " << pow((1 - pow(1 - 1 / double(m), double(k*n))), (double)k) << " ratio "<<ratio<< endl;
			
			
			/*cout.width(8);
			cout.setf(ios::left);
			cout.precision(7);
			*/
			cout << setw(7) << setiosflags(ios::fixed)<<setprecision(7) << setiosflags(ios::left) << k << fp_test <<"     "<< fp_theor <<"     "<< ratio << endl;

			fout << setw(7) << setiosflags(ios::fixed) << setprecision(7) << setiosflags(ios::left) << k;
			fout << setw(14) << fp_test;
			fout << setw(14) << fp_theor;
			fout << setw(14) << ratio << endl;

			
			fout2 << "-----------------------------------------------------------------------------------" << endl << endl;
			fout2 << "The time of test  Bloom Filter start: " << t_s << " end: " << t_e << " total: " << t_e - t_s << endl;
			fout2 << "k = " << sBF1->Get_bf_k() << endl;
			fout2 << "m = " << sBF1->Get_bf_m() << endl;
			fout2 << "n = " << sBF1->Get_bf_n() << endl;
			fout2 << "q = " << sBF1->Get_bf_q() << endl;
			fout2 << "total_num: " << total_num << " bloom_filter_num: " << n << " true_num: " << true_num << " false_positive_num: " << false_positive_num << endl;
			fout2 << " false_positive_ratio " << double(false_positive_num) / double(total_num) << " theoretical value: " << pow((1 - pow(1 - 1 / double(m), double(k*n))), (double)k) << " ratio " << ratio << endl;
            	
	}// end for

	}// end for testn[]

	delete sBF1;
	fout.close();
	fout2.close();
}
/*
  false_positive_std,
  read int ip,then duplicate removal,insert n elements,at last,query..
*/
void false_positive_std_bf(char *filename)
{
	cout << "starting test false positive..." << endl;
	if (filename == NULL)
		return;
	time_t t_s, t_e;
	time(&t_s);

	unsigned char prefix[TEST_PREFIX_LEN];

	//k 6-10
	int k = 6;
	int n = 10000;
	int m = (int)((k * n / log(2)) + 0.5);
	StandardBF *sBF1 = new StandardBF(m, k);
	EXSBF *exSbf = new EXSBF(m, k);
	
	unsigned int ip;

	
	unsigned int total_num = 0;
	unsigned int false_positive_num = 0;
	unsigned int true_num = 0;

	double fp_test;
	double fp_theor;
	double ratio;

	set<unsigned int> v;

	ifstream fin(filename);

	while (!fin.eof())
	{
		fin >> ip;
		//cout << ip << " ip ";
		v.insert(ip);
	}

	fin.close();


	ofstream fout("result_exbf_test.txt",ios::app); ///just ratio
	ofstream fout2("result_exbf_test_nomal.txt",ios::app); // all 

	set<unsigned int>::iterator begin = v.begin();
	set<unsigned int>::iterator end = v.end();

	/*
	ofstream fout("rrctest1000000.txt");

	for (; begin != end; begin++)
	{
	fout << *begin << endl;
	}
	fout.close();
	return;
	*/
	time(&t_e);
	cout << " construct bloom filter time : " << t_e - t_s << endl;

	fout << " construct bloom filter time : " << t_e - t_s << endl;
	fout2 << " construct bloom filter time : " << t_e - t_s << endl;
	int testn[] = { 1000, 2000, 5000, 10000, 20000 };
	for (int j = 0; j < 1; j++)
	{
		fout << "-----------------------------------------------" << endl;
		fout << "######### test n= " << testn[j] << " #############" << endl;
		fout2 << "-----------------------------------------------" << endl;
		fout2 << "######### test n= " << testn[j] << " #############" << endl;
		fout << setw(7) << setiosflags(ios::left) << "k";
		fout << setw(14) << "fp_test";
		fout << setw(14) << "theor";
		fout << setw(14) << "ratio" << endl;




		for (int i = 4; i <= 7; i++)
		{
			time(&t_s);
			k = i;

			n = testn[j];
			m = (int)((k * n / log(2)) + 0.5);
			sBF1->initial(m, k);
			exSbf->initial(m, k);

			total_num = 0;
			false_positive_num = 0;
			true_num = 0;

			
			for (begin = v.begin(); begin != end; begin++)
			{
				memset(prefix, 0, sizeof(prefix));
				//fin >> ip;
				ip = *begin;
				//cout << ip << "  ";
				//	sprintf(sPrefix,"%u.%u.%u.%u", (ip >> 24), (ip << 8) >> 24, (ip << 16) >> 24, (ip << 24) >> 24);
				//	cout << sPrefix << endl;
				prefix[0] = ip >> 24;
				prefix[1] = (ip << 8) >> 24;
				prefix[2] = (ip << 16) >> 24;
				prefix[3] = (ip << 24) >> 24;
				//	cout << "perfix " << (int)prefix[0] << " " << (int)prefix[1] << " " << (int)prefix[2] << " " << (int)prefix[3] << endl;
				if (total_num < unsigned(n))
				{
					sBF1->insert(prefix, TEST_PREFIX_LEN);
					exSbf->insert(prefix, TEST_PREFIX_LEN);
				}
				else  //search
				{
					if (sBF1->query(prefix, TEST_PREFIX_LEN) && exSbf->query(prefix, TEST_PREFIX_LEN))
					//if (exSbf->query(prefix, TEST_PREFIX_LEN))
					{
						false_positive_num++;
					}
					else
					{
						true_num++;
					}

				}

				total_num++;
			}// for set iterator
			//	}//while
			//	fin.close();



			time(&t_e);
			fp_test = double(false_positive_num) / double(total_num);
			fp_theor = pow((1 - pow(1 - 1 / double(m), double(k*n))), (double)k);
			ratio = abs(1 - fp_test / fp_theor);

			cout << "-----------------------------------------------------------------------------------" << endl << endl;
			cout << "The time of test  Bloom Filter start: " << t_s << " end: " << t_e << " total: " << t_e - t_s << endl;
			cout << "k = " << sBF1->Get_bf_k() << endl;
			cout << "m = " << sBF1->Get_bf_m() << endl;
			cout << "n = " << sBF1->Get_bf_n() << endl;
		
			cout << "total_num: " << total_num << " bloom_filter_num: " << n << " true_num: " << true_num << " false_positive_num: " << false_positive_num << endl; //" approximation " << pow((1 - exp(-double(k*n) / double(m))), k)
			cout << " false_positive_ratio " << double(false_positive_num) / double(total_num) << " theoretical value: " << pow((1 - pow(1 - 1 / double(m), double(k*n))), (double)k) << " ratio " << ratio << endl;


			/*cout.width(8);
			cout.setf(ios::left);
			cout.precision(7);
			*/
			cout << setw(7) << setiosflags(ios::fixed) << setprecision(7) << setiosflags(ios::left) << k << fp_test << "     " << fp_theor << "     " << ratio << endl;

			fout << setw(7) << setiosflags(ios::fixed) << setprecision(7) << setiosflags(ios::left) << k;
			fout << setw(14) << fp_test;
			fout << setw(14) << fp_theor;
			fout << setw(14) << ratio << endl;


			fout2 << "-----------------------------------------------------------------------------------" << endl << endl;
			fout2 << "The time of test  Bloom Filter start: " << t_s << " end: " << t_e << " total: " << t_e - t_s << endl;
			fout2 << "k = " << sBF1->Get_bf_k() << endl;
			fout2 << "m = " << sBF1->Get_bf_m() << endl;
			fout2 << "n = " << sBF1->Get_bf_n() << endl;

			fout2 << "total_num: " << total_num << " bloom_filter_num: " << n << " true_num: " << true_num << " false_positive_num: " << false_positive_num << endl;
			fout2 << " false_positive_ratio " << double(false_positive_num) / double(total_num) << " theoretical value: " << pow((1 - pow(1 - 1 / double(m), double(k*n))), (double)k) << " ratio " << ratio << endl;

		}// end for

	}// end for testn[]

	delete sBF1;
	delete exSbf;
	fout.close();
	fout2.close();
}





/*
   just for test 
*/
void test_bf_query(StandardBF *sBF1)
{

	ifstream fin;
	fin.open("test_ipprefix.4bytes");
	int j = 0;
	unsigned int x;
	unsigned char prefix[PREFIX_NUM][PREFIX_LEN];
	if (!fin.is_open())
	{
		cout << "fail" << endl;
		return;
	}

	while (!fin.eof() && j<PREFIX_NUM)
	{

		for (int i = 0; i<PREFIX_LEN; i++)
		{
			fin >> x;

			//	cout<<x<<" ";
			prefix[j][i] = (char)x;
		}
		j++;
		//cout<<endl;
	}
	fin.close();

	for (int i = 0; i<PREFIX_NUM; i++)
	{
		for (j = 0; j<PREFIX_LEN; j++)
		{
			cout << (int)prefix[i][j] << ends;
		}
		if (sBF1->query(prefix[i], PREFIX_LEN))
			cout << "the prefix is found.\n";
		else
			cout << "the prefix is not found.\n";
		cout << endl;
	}




}




int trans_strip_to_intip(char *file_str_ip)
{
	char file_name[30];
	strcpy(file_name,file_str_ip);
	strcat(file_name,"_intip");

	ofstream fout;	
	fout.open(file_name);
	

	if (fout.fail())
	{
		cout << "open outputfile failed.. " << endl;
		return -1;
	}


	ifstream fin(file_str_ip);//filename=MergedTrie
	if (fin.fail())
	{
		cout << "open outputfile failed.. " << endl;
		return -1;
	}

	char str[LINE_LEN];
	char sPrefix[20];
	int level;

	unsigned int total_num = 0;
	unsigned int bitarray_num = 0;
	int insertcount = 0;
	unsigned int ip;
	char *str_p;
	while (!fin.eof())
	{
		total_num++;
		memset(sPrefix, 0, sizeof(sPrefix));
		

		fin >> sPrefix ;

		
	
		int iLen = strlen(sPrefix);
	    
		ip = 0;
		str_p=strtok(sPrefix,".");
		ip += atoi(str_p) << 24;

		str_p = strtok(NULL, ".");
		ip += atoi(str_p) << 16;

		str_p = strtok(NULL, ".");
		ip += atoi(str_p) << 8;

		str_p = strtok(NULL, ".");
		ip += atoi(str_p);

		/*
		while (str_p != NULL)
		{
			ip += atoi(str_p);
			cout << str_p << "." ;
			str_p=strtok(NULL,".");

		}
		*/
		//cout << endl;
		fout << ip << endl;

		

	}//while
	fin.close();
	fout.close();
	return 0;

}


void bf_query24(char *filename, CFib *MergedFib, StandardBF **a_BloomFilter, EXSBF *extraBF)
{
	static int k = 8;
	static int k_extra = 8;
	//char *filename = "rrc00_2013.6.8.8.txt";
	cout << "querying " << filename << " from bloom filter... " << endl;

	ifstream fin(filename);
	if (fin.fail())
	{
		cout << "open failed :" << filename << endl;
		return;
	}

	//	char str[LINE_LEN];
	//	char sPrefix[20];
	unsigned char prefix[PREFIX_LEN];
	//	int level;
	int j = 0;
	ofstream fout;
	char res_file[30] = "result_bfquery_";
	char tmp[3];

	if (extraBF == NULL)
	{
		sprintf(tmp, "%d", k);
		strcat(res_file, tmp);
		k++;
		//fout.open("result_bf_qurey.txt",ios::app);
		fout.open(res_file, ios::app);
	}
	else
	{
		sprintf(tmp, "%d", k_extra);
		strcat(res_file, tmp);
		k_extra++;
		//fout.open("result_extrabf_qurey.txt",ios::app);
		strcat(res_file, "extra");
		fout.open(res_file, ios::app);
	}

	if (fout.fail())
	{
		cout << "open outputfile failed.. " << endl;
		return;
	}

	ofstream foutR("result_ratio.txt", ios::app);
	if (extraBF == NULL)
	{
		fout << "##################################################################" << endl;
		fout << "#########################    NEW    TEST   #######################" << endl;
		fout << "######################     sdf  k = " << k - 1 << "    #################" << endl;
		fout << "##################################################################" << endl;


		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "######################       sbf ratio       ####################" << endl;
		foutR << "######################       k = " << k - 1 << "     ####################" << endl;
		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "---------------------------------------------------------------------" << endl;

	}
	else
	{
		fout << "##################################################################" << endl;
		fout << "#########################    NEW    TEST   #######################" << endl;
		fout << "######################       k = " << k_extra - 1 << "    #################" << endl;
		fout << "##################################################################" << endl;


		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "######################       extraBf   ratio         #################" << endl;
		foutR << "######################    extra   k = " << k_extra - 1 << "     ####################" << endl;
		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "---------------------------------------------------------------------" << endl;

	}

	set<unsigned int> v;

	unsigned int ip;
	int  count = 0;
	int total_num = 0;



	//	cout << "v.size = " << v.size() << endl;
	//fout << "v.size = " << v.size() << endl;


	//set<unsigned int>::iterator begin = v.begin();
	//set<unsigned int>::iterator end = v.end();	

	unsigned int access_num[16] = { 0 };

	unsigned int bitarray_work_num = 0; //bitarray work
	unsigned int bf_query_once = 0;// 16个bf只返回一个结果
	unsigned int bf_query_more_than_twice = 0;//16个bf查询返回多于1个结果
	unsigned int extra_bf_work = 0; //extraBF 过滤起作用的ip个数（即通过extra_bf可去掉fp的结果）

	unsigned short bf_mapping_num = 0; //ip映射成功的bf的个数
	//unsigned short int mapping_bf_level[16];
	bitset<16> mapping_bf_level(0); //record the level	 that mapped the ip 

	unsigned int error_not_find_ip = 0;
	unsigned int error_outofi = 0;
	int error_ip_extra_mappingnoce = 0;
	int extra_mapping_times = 0;
	int extra_not_mapping_times = 0;


	int findIplevel;
	int bf_nomapping = 0;

	//for (begin = v.begin(); begin != end; begin++)
	unsigned int begin;
	while (!fin.eof())
	{
		fin >> ip;

		begin = ip;
		total_num++;
		//if (ip < 33554432)
		//	continue;

		//cout << ip << " ip ";
		//	v.insert(ip);
		///	cout << ip << endl;
		count++;
		if (count == 1000000)
		{
			count = 0;
			cout << "count = 1M, running waiting..." << endl;

			//Sleep(10000);
			//break;
		}


		memset(prefix, 0, sizeof(prefix));
		mapping_bf_level.reset();
		//cout << "----------------------------------------------" << endl;
		//fout << "----------------------------------------------" << endl;
		//fin >> ip;
		ip = begin;
		//cout << ip << "  ";
		//	sprintf(sPrefix,"%u.%u.%u.%u", (ip >> 24), (ip << 8) >> 24, (ip << 16) >> 24, (ip << 24) >> 24);
		//	cout << sPrefix << endl;

		//	cout << "perfix " << (int)prefix[0] << " " << (int)prefix[1] << " " << (int)prefix[2] << " " << (int)prefix[3] << endl;

		ip = ip >> 16;
		//cout << "ip >> 16 " <<ip<< " ip int "<<begin<<endl;
		//fout << "ip >> 16 " << ip << " ip int " << begin << endl;
		if (bit_array[ip] == 1) // 前缀为16,15,14,....,1
		{
			if (bit_array[ip] == 1)
			{
				ip = ip << 16;
				/*		int prefixLen = 0;
				for (int t = 0; t < 16; t++)
				{
				if (((ip << t)&HIGHTBIT)==HIGHTBIT)  //HIGHTBIT, 2^31
				{
				prefixLen++;
				}
				}
				*/

				bitarray_work_num++;
				access_num[0]++;// 1次访hash的ip个数

				/*
				if ((findIplevel=MergedFib->FindIp(ip, 16))!=0)  /// bit_array ,prefix = 16???
				{
				ip = begin;
				prefix[0] = ip >> 24;
				prefix[1] = (ip << 8) >> 24;
				prefix[2] = (ip << 16) >> 24;
				prefix[3] = (ip << 24) >> 24;
				prefix[4] = 16;
				//	cout << " int ip :" << begin << endl;

				//	cout <<"level:"<<prefixLen<< " find ip in bit_array,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << 16 << endl;

				//	fout << " int ip :" << begin << endl;
				//	fout << "level:" << 16 << " find ip in bit_array,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << 16 << endl;

				//	Sleep(10000);
				}
				else
				{
				error_not_find_ip++; //should find the ip ,but MergedTrie->FindIp return 0;
				}
				*/
			}
		}
		else  //search
		{
			bf_mapping_num = 0;
			for (int i = 0; i < BF_NUM; i++)
			{
				ip = begin;
				ip = ip >> (32 - (17 + i));
				ip = ip << (32 - (17 + i));
				prefix[0] = ip >> 24;
				prefix[1] = (ip << 8) >> 24;
				prefix[2] = (ip << 16) >> 24;
				prefix[3] = (ip << 24) >> 24;
				prefix[4] = (17 + i);
				if (a_BloomFilter[i]->query(prefix, PREFIX_LEN))
				{
					bf_mapping_num++;
					mapping_bf_level[i] = 1;

#ifdef DEBUG
					cout << "ip int " << begin << endl;
					cout << "level = " << 17 + i << " find ip in bf->query,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

#endif // DEBUG



					//	fout << "ip int " << begin << endl;
					//	fout << "level = " << 17 + i << " find ip in bf->query,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

				}

			}//for 

			if (bf_mapping_num == 0)
			{
#ifdef DEBUG
				cout << "so what...." << endl;
#endif // DEBUG


				ip = begin;
				prefix[0] = ip >> 24;
				prefix[1] = (ip << 8) >> 24;
				prefix[2] = (ip << 16) >> 24;
				prefix[3] = (ip << 24) >> 24;
				prefix[4] = 0;
#ifdef DEBUG
				cout << "ip int " << begin << endl;
				cout << " not find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << endl;

#endif // DEBUG


				//	fout << "ip int " << begin << endl;
				fout << " not find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << endl;


				bf_nomapping++;

			}
			else if (bf_mapping_num == 1)
			{
#ifdef DEBUG

				cout << "bf_mapping_num==1" << endl;
#endif // DEBUG

				bf_query_once++;

				access_num[0]++;//只查询一次hash的ip个数


				string strip = "";
				//				char tmp[4];
				int i;
				for (i = 0; i < 16; i++)
				{
					if (mapping_bf_level[i] == 1)
					{
						prefix[4] = (unsigned char)(17 + i);
						break;

					}
				}

				ip = begin;


				ip = ip >> (32 - 17 - i);
				ip = ip << (32 - 17 - i);
				/*
				if (MergedFib->FindIp(ip, 17+i))
				{
				ip = begin;
				prefix[0] = ip >> 24;
				prefix[1] = (ip << 8) >> 24;
				prefix[2] = (ip << 16) >> 24;
				prefix[3] = (ip << 24) >> 24;
				prefix[4] = (17 + j);
				#ifdef DEBUG
				cout << "ip int " << begin << endl;
				cout << "level = " << 17 + i << " find ip in bf query once,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

				#endif // DEBUG


				//		fout << "ip int " << begin << endl;
				//	fout << "level = " << 17 + i << " find ip in bf query once,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

				}
				else
				{
				error_not_find_ip++;
				}
				*/

			}//bf_mapping_num ==1
			else if (bf_mapping_num >1)
			{
				bf_query_more_than_twice++;
#ifdef DEBUG
				cout << "bf_query_more than twice " << endl;
#endif // DEBUG


				//access_num[bf_mapping_num-1]++;
				if (extraBF == NULL) //从最长前缀开始查找，访问trie树，查看访问次数
				{
					string strip;
					char tmp[4];
					int i;
					int times = 1;
					for (i = BF_NUM - 1; i >= 0; i--)
					{



						if (mapping_bf_level[i] == 0)
							continue;
						cout << "extral ==NULL  i:" << i << endl;
						strip = "";
						memset(tmp, 0, sizeof(tmp));
						ip = begin;
#ifdef DEBUG
						cout << " ip int " << ip << " i: " << i << endl;
#endif // DEBUG


						//	fout << " ip int " << ip << endl;
						ip = ip >> (32 - 17 - i);
						ip = ip << (32 - 17 - i);//ip prefix
						if (MergedFib->FindIp(ip, 17 + i))
						{
							ip = begin;
							prefix[0] = ip >> 24;
							prefix[1] = (ip << 8) >> 24;
							prefix[2] = (ip << 16) >> 24;
							prefix[3] = (ip << 24) >> 24;
							prefix[4] = (17 + i);
#ifdef DEBUG
							cout << "ip int " << begin << endl;
							cout << "level = " << 17 + i << " find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

#endif // DEBUG

							//				fout << "ip int " << begin << endl;
							//fout << "level = " << 17 + i << " find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

							break;
						}
						times++;
						/*
						prefix[0] = ip >> 24;
						sprintf(tmp, "%d", prefix[0]);
						strip.append(tmp);
						strip.append(".");
						prefix[1] = (ip << 8) >> 24;
						memset(tmp, 0, sizeof(tmp));
						sprintf(tmp, "%d", prefix[1]);
						strip.append(tmp);
						strip.append(".");
						prefix[2] = (ip << 16) >> 24;
						memset(tmp, 0, sizeof(tmp));
						sprintf(tmp, "%d", prefix[2]);
						strip.append(tmp);
						strip.append(".");
						prefix[3] = (ip << 24) >> 24;
						memset(tmp, 0, sizeof(tmp));
						sprintf(tmp, "%d", prefix[3]);
						strip.append(tmp);
						strip.append("/");
						prefix[4] = (unsigned char)(17 + i);
						sprintf(tmp, "%d", prefix[4]);
						strip.append(tmp);

						cout << " ip is " << strip << endl;


						if (routerSet.find(strip)!=routerSet.end())
						{
						break;
						}
						*/

					}//for i 
					if (i == -1)
					{

						cout << "sssssssssssssssssssssssssssssssssssss" << endl;
						ip = begin;
						prefix[0] = ip >> 24;
						prefix[1] = (ip << 8) >> 24;
						prefix[2] = (ip << 16) >> 24;
						prefix[3] = (ip << 24) >> 24;
						prefix[4] = (unsigned char)(17 + i);
#ifdef DEBUG
						cout << "ip int " << begin << endl;
						cout << " not find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << endl;

#endif // DEBUG


						fout << "sssssssssssssssssssssssssssssssssssss" << endl;

						error_outofi++;
					}
					//	Sleep(10000);
					if (i>-1)
						access_num[times - 1]++;

				}//extraBf==NULL
				else if (extraBF != NULL)
				{
					int extraBf_mapping_num = 0;
					for (int i = 0; i < BF_NUM; i++)
					{
						if (i == 7) //except level 24
							continue;
						if (mapping_bf_level[i] == 1)
						{
							ip = begin;
							ip = ip >> (32 - (17 + i));
							ip = ip << (32 - (17 + i));
							prefix[0] = ip >> 24;
							prefix[1] = (ip << 8) >> 24;
							prefix[2] = (ip << 16) >> 24;
							prefix[3] = (ip << 24) >> 24;
							prefix[4] = (unsigned char)(17 + i);

							if (extraBF->query(prefix, PREFIX_LEN))
							{
								extraBf_mapping_num++;
								extra_mapping_times++;
							}
							else
							{
								extra_not_mapping_times++;

								mapping_bf_level[i] = 0;
							}
						}

					}
					
				   if (extraBf_mapping_num == 0)
					{
						extra_bf_work++; //sucess 
						access_num[0]++;
						ip = begin;

//						for (int i = 0; i < BF_NUM; i++)
						{
				//			if (mapping_bf_level[i] == 1)
							{
								/*
								if (MergedFib->FindIp(ip, i+17))
								{
								ip = begin;
								prefix[0] = ip >> 24;
								prefix[1] = (ip << 8) >> 24;
								prefix[2] = (ip << 16) >> 24;
								prefix[3] = (ip << 24) >> 24;
								prefix[4] = (unsigned char)(17 + i);
								#ifdef DEBUG
								cout << "ip int " << begin << endl;
								cout << "level = " << 17 + i << " find ip in extraBf_mapping once,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

								#endif // DEBUG

								//		fout << "ip int " << begin << endl;
								//fout << "level = " << 17 + i << " find ip in extraBf_mapping once,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

								break;
								}
								else
								{
								error_ip_extra_mappingnoce++;

								}*/

						//		break;


							}//if 

						}//for 


					}
					else ///  extraBF is false positive,find the ip in MergedTrie
					{
						int i;
						string strip;
						int times = 1;
						//						char tmp[4];
						for (i = BF_NUM - 1; i >= 0; i--)
						{
							if (mapping_bf_level[i] == 0)
								continue;
							ip = begin;
							ip = ip >> (32 - (17 + i));
							ip = ip << (32 - (17 + i));
							if (MergedFib->FindIp(ip, 17 + i))
							{
								ip = begin;
								prefix[0] = ip >> 24;
								prefix[1] = (ip << 8) >> 24;
								prefix[2] = (ip << 16) >> 24;
								prefix[3] = (ip << 24) >> 24;
								prefix[4] = (unsigned char)(17 + i);
#ifdef DEBUG
								cout << "ip int " << begin << endl;
								cout << "level = " << 17 + i << " extra : find ip in extra MergedFib ,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

#endif // DEBUG


								//		fout << "ip int " << begin << endl;
								//fout << "level = " << 17 + i << " extra : find ip in extra MergedFib ,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

								break;
							}
							else
							{

							}
							times++;

						}// end for
						if (i == -1)
						{

							cout << "extra :sssssssssssssssss" << endl;
							ip = begin;
							prefix[0] = ip >> 24;
							prefix[1] = (ip << 8) >> 24;
							prefix[2] = (ip << 16) >> 24;
							prefix[3] = (ip << 24) >> 24;
							prefix[4] = (unsigned char)(17 + i);
#ifdef DEBUG
							cout << "ip int " << begin << endl;
							cout << " not find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << endl;

#endif // DEBUG


							fout << "extra :sssssssssssssssss not find ip " << ip << endl;
							error_outofi++;
						}
						//Sleep(10000);
						if (i>-1)
							access_num[times - 1]++;


					}// else find the ip in MergedTrie

				}//else if extraBF !=NULL
			}// bf mapping more than twice 
		}// else :not mapping 16 bf	
	}// while fin.eof()

	fin.close();
	for (int i = 0; i < BF_NUM; i++)
	{
		cout << "Num of querry " << i + 1 << " times hash table(MergedTrie): " << access_num[i] << endl;
		fout << "Num of querry " << i + 1 << " times hash table(MergedTrie): " << access_num[i] << endl;
		foutR << "Num of querry " << i + 1 << " times hash table(MergedTrie): " << access_num[i] << endl;
	}


	cout << "ip mapping more than twice " << bf_query_more_than_twice << endl;
	cout << "total num : " << total_num << endl;//// totalnum = access[0] +...+access[15]
	cout << " bit_array_work: " << bitarray_work_num << " bf_querryonce: " << bf_query_once << endl;// bitarray_work_num+
	cout << " ip nomapping: " << bf_nomapping << endl;
	cout << "error_ip_exxtra_mappingonce" << error_ip_extra_mappingnoce << endl;
	cout << " extra_mapping_times : " << extra_mapping_times << endl;
	cout << " extra_not_mapping_times: " << extra_not_mapping_times << endl;

	if (extraBF != NULL)
		cout << "extra msg: m,n,k " << extraBF->Get_bf_m() << ends << extraBF->Get_bf_n() << " " << extraBF->Get_bf_k() << endl;
	else
		cout << "ssssssssssssssssssssssssssssssss" << endl;

	fout << "ip mapping more than twice " << bf_query_more_than_twice << endl;
	fout << "total num : " << total_num << endl;//// totalnum = access[0] +...+access[15]
	fout << " bit_array_work: " << bitarray_work_num << " bf_querryonce: " << bf_query_once << endl;// bitarray_work_num+
	fout << " extra_mapping_times : " << extra_mapping_times << endl;
	fout << " extra_not_mapping_times: " << extra_not_mapping_times << endl;

	foutR << "ip mapping more than twice " << bf_query_more_than_twice << endl;
	foutR << "total num : " << total_num << endl;//// totalnum = access[0] +...+access[15]
	foutR << " bit_array_work: " << bitarray_work_num << " bf_querryonce: " << bf_query_once << endl;// bitarray_work_num+
	foutR << " extra_mapping_times : " << extra_mapping_times << endl;
	foutR << " extra_not_mapping_times: " << extra_not_mapping_times << endl;

	if (NULL != extraBF)
	{
		cout << "extra workd : " << extra_bf_work << endl;
		fout << "extra workd : " << extra_bf_work << endl;
		foutR << "extra workd : " << extra_bf_work << endl;

	}
	cout << "error_not_find_ip is " << error_not_find_ip << " error_outofi: " << error_outofi << endl;
	fout << "error_not_find_ip is " << error_not_find_ip << " error_outofi: " << error_outofi << endl;
	fout << "----------------------------------------------------------------------------" << endl;
	fout << "----------------------------------------------------------------------------" << endl;
	foutR << "error_not_find_ip is " << error_not_find_ip << " error_outofi: " << error_outofi << endl;
	foutR << "----------------------------------------------------------------------------" << endl;
	foutR << "----------------------------------------------------------------------------" << endl;
	foutR.close();
	fout.close();

}



/*
  filename: store int type ip 
        rand ip :rrctest10M
		rand base routeFib ip:rrc00_2013.6.8.8.txt_10000K.tr  去重
*/
void bf_query2(char *filename, CFib *MergedFib,StandardBF **a_BloomFilter, EXSBF *extraBF)
{	
	static int k = 4;
	static int k_extra = 4;
	//char *filename = "rrc00_2013.6.8.8.txt";
	cout << "querying "<< filename << " from bloom filter... " << endl;
	
	ifstream fin(filename);
	if (fin.fail())
	{
		cout << "open failed :" << filename << endl;
		return;
	}

//	char str[LINE_LEN];
//	char sPrefix[20];
	unsigned char prefix[PREFIX_LEN];
//	int level;
	int j=0;
	ofstream fout;
	char res_file[30] = "result_bfquery_";
	char tmp[3];
	
	if (extraBF == NULL)
	{
		sprintf(tmp,"%d",k);
		strcat(res_file,tmp);
		k++;
		//fout.open("result_bf_qurey.txt",ios::app);
		fout.open(res_file, ios::app);
	}
	else
	{
		sprintf(tmp, "%d", k_extra);
		strcat(res_file, tmp);
			k_extra++;
		//fout.open("result_extrabf_qurey.txt",ios::app);
		strcat(res_file,"extra");
		fout.open(res_file, ios::app);
	}

	if (fout.fail())
	{
		cout << "open outputfile failed.. " << endl;
		return;
	}
	
	ofstream foutR("result_ratio.txt",ios::app);
	ofstream foutR4("result_ratio_r4.txt", ios::app);

	if (extraBF == NULL)
	{
		fout << "##################################################################" << endl;
		fout << "#########################    NEW    TEST   #######################" << endl;
		fout << "######################     sdf  k = " << k -1<< "    #################" << endl;
		fout << "##################################################################" << endl;


		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "######################       sbf ratio       ####################" << endl;
		foutR << "######################       k = " << k-1 << "     ####################" << endl;
		foutR << "---------------------------------------------------------------------" << endl;		
		foutR << "---------------------------------------------------------------------" << endl;

		
		foutR4 << setw(14) << setiosflags(ios::left) << setiosflags(ios::fixed) << setprecision(7) << k-1;
		
	}
	else
	{
		fout << "##################################################################" << endl;
		fout << "#########################    NEW    TEST   #######################" << endl;
		fout << "######################       k = " << k_extra-1 << "    #################" << endl;
		fout << "##################################################################" << endl;


		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "######################       extraBf   ratio         #################" << endl;
		foutR << "######################    extra   k = " << k_extra-1 << "     ####################" << endl;
		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "---------------------------------------------------------------------" << endl;

		foutR4 << setw(14) << setiosflags(ios::left) << setiosflags(ios::fixed) << setprecision(7) << k_extra - 1;
	}
	

	
	set<unsigned int> v;

	unsigned int ip;
	int  count = 0;
	int total_num = 0;
	
	
	
//	cout << "v.size = " << v.size() << endl;
	//fout << "v.size = " << v.size() << endl;
	

	//set<unsigned int>::iterator begin = v.begin();
	//set<unsigned int>::iterator end = v.end();	

	unsigned int access_num[16] = {0};

	unsigned int bitarray_work_num = 0; //bitarray work
	unsigned int bf_query_once = 0;// 16个bf只返回一个结果
	unsigned int bf_query_more_than_twice = 0;//16个bf查询返回多于1个结果
	unsigned int extra_bf_work = 0; //extraBF 过滤起作用的ip个数（即通过extra_bf可去掉fp的结果）

	unsigned short bf_mapping_num = 0; //ip映射成功的bf的个数
	//unsigned short int mapping_bf_level[16];
	bitset<16> mapping_bf_level(0); //record the level	 that mapped the ip 

	unsigned int error_not_find_ip = 0;
	unsigned int error_outofi = 0;
	int error_ip_extra_mappingnoce = 0;
	int extra_mapping_times = 0;
	int extra_not_mapping_times = 0;


	int findIplevel;
	int bf_nomapping = 0;

	//for (begin = v.begin(); begin != end; begin++)
	unsigned int begin;
	while (!fin.eof())
	{
		fin >> ip;

		begin =ip;
		total_num++;
		//if (ip < 33554432)
		//	continue;

		//cout << ip << " ip ";
		//	v.insert(ip);
	///	cout << ip << endl;
		count++;
		if (count == 1000000)
		{
			count = 0;
			cout << "count = 1M, running waiting..." << endl;
			
			//Sleep(10000);
			//break;
		}

			
		memset(prefix, 0, sizeof(prefix));
		mapping_bf_level.reset();
		//cout << "----------------------------------------------" << endl;
		//fout << "----------------------------------------------" << endl;
		//fin >> ip;
		ip = begin;
		//cout << ip << "  ";
		//	sprintf(sPrefix,"%u.%u.%u.%u", (ip >> 24), (ip << 8) >> 24, (ip << 16) >> 24, (ip << 24) >> 24);
		//	cout << sPrefix << endl;
		
		//	cout << "perfix " << (int)prefix[0] << " " << (int)prefix[1] << " " << (int)prefix[2] << " " << (int)prefix[3] << endl;
		
		ip = ip >> 16;
		//cout << "ip >> 16 " <<ip<< " ip int "<<begin<<endl;
		//fout << "ip >> 16 " << ip << " ip int " << begin << endl;
		if (bit_array[ip] == 1) // 前缀为16,15,14,....,1
		{			
			if (bit_array[ip] == 1)
			{
				ip = ip << 16;
		/*		int prefixLen = 0;
				for (int t = 0; t < 16; t++)
				{
					if (((ip << t)&HIGHTBIT)==HIGHTBIT)  //HIGHTBIT, 2^31
					{
						prefixLen++;
					}
				}
				*/
				 
				bitarray_work_num++;
				access_num[0]++;// 1次访hash的ip个数

				/*
				if ((findIplevel=MergedFib->FindIp(ip, 16))!=0)  /// bit_array ,prefix = 16???
				{
					ip = begin;
					prefix[0] = ip >> 24;
					prefix[1] = (ip << 8) >> 24;
					prefix[2] = (ip << 16) >> 24;
					prefix[3] = (ip << 24) >> 24;
					prefix[4] = 16;
				//	cout << " int ip :" << begin << endl;

				//	cout <<"level:"<<prefixLen<< " find ip in bit_array,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << 16 << endl;
					
				//	fout << " int ip :" << begin << endl;
				//	fout << "level:" << 16 << " find ip in bit_array,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << 16 << endl;

				//	Sleep(10000);
				}
				else
				{
					error_not_find_ip++; //should find the ip ,but MergedTrie->FindIp return 0;
				}
				*/
			}
		}
		else  //search
		{
			bf_mapping_num = 0;
			for (int i = 0; i < BF_NUM; i++)
			{
				ip = begin;
				ip=ip >> (32 - (17 + i));
				ip = ip << (32 - (17 + i));
				prefix[0] = ip >> 24;
				prefix[1] = (ip << 8) >> 24;
				prefix[2] = (ip << 16) >> 24;
				prefix[3] = (ip << 24) >> 24;
				prefix[4] = (17+i);
				if (a_BloomFilter[i]->query(prefix, PREFIX_LEN))
				{
					bf_mapping_num++;
					mapping_bf_level[i]=1;
				
					#ifdef DEBUG
						cout << "ip int " << begin << endl;
						cout << "level = " << 17 + i << " find ip in bf->query,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

					#endif // DEBUG


					
				//	fout << "ip int " << begin << endl;
				//	fout << "level = " << 17 + i << " find ip in bf->query,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

				}
					
			}//for 

			if (bf_mapping_num == 0)
			{
#ifdef DEBUG
cout << "so what...." << endl;
#endif // DEBUG

				
				ip = begin;
				prefix[0] = ip >> 24;
				prefix[1] = (ip << 8) >> 24;
				prefix[2] = (ip << 16) >> 24;
				prefix[3] = (ip << 24) >> 24;
				prefix[4] = 0;
#ifdef DEBUG
cout << "ip int " << begin << endl;
cout << " not find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << endl;

#endif // DEBUG

				
			//	fout << "ip int " << begin << endl;
				fout << " not find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << endl;


				bf_nomapping++;
				
			}
			else if (bf_mapping_num == 1)
			{
#ifdef DEBUG

				cout << "bf_mapping_num==1" << endl;
#endif // DEBUG

				bf_query_once++;
				
				access_num[0]++;//只查询一次hash的ip个数


				string strip = "";
//				char tmp[4];
				int i;
				for ( i = 0; i < 16; i++)
				{
					if (mapping_bf_level[i] == 1)
					{
						prefix[4] = (unsigned char)(17 + i);
						break;

					}
				}
			
				ip = begin;


				ip = ip >> (32-17-i);
				ip = ip << (32 - 17 - i);
				/*
				if (MergedFib->FindIp(ip, 17+i))
				{
					ip = begin;
					prefix[0] = ip >> 24;
					prefix[1] = (ip << 8) >> 24;
					prefix[2] = (ip << 16) >> 24;
					prefix[3] = (ip << 24) >> 24;
					prefix[4] = (17 + j);
#ifdef DEBUG
cout << "ip int " << begin << endl;
					cout << "level = " << 17 + i << " find ip in bf query once,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

#endif // DEBUG

					
			//		fout << "ip int " << begin << endl;
				//	fout << "level = " << 17 + i << " find ip in bf query once,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

				}
				else
				{
					error_not_find_ip++;
				}
				*/
				
			}//bf_mapping_num ==1
			else if(bf_mapping_num >1)
			{
				bf_query_more_than_twice++;
#ifdef DEBUG
cout << "bf_query_more than twice " << endl;
#endif // DEBUG

				
				//access_num[bf_mapping_num-1]++;
				if (extraBF == NULL) //从最长前缀开始查找，访问trie树，查看访问次数
				{
					string strip;
					char tmp[4];
					int i;
					int times = 1;
					for ( i = BF_NUM -1; i >=0; i--)
					{
						if (mapping_bf_level[i] == 0)
							continue;
						cout << "extral ==NULL  i:"<<i << endl;
						strip = "";
						memset(tmp,0,sizeof(tmp));
						ip = begin;
#ifdef DEBUG
	cout << " ip int " << ip <<" i: "<<i<< endl;
#endif // DEBUG

					
					//	fout << " ip int " << ip << endl;
						ip = ip >> (32 - 17 - i);
						ip = ip << (32 - 17 - i);//ip prefix
						if (MergedFib->FindIp(ip, 17 +i))
						{
							ip = begin;
							prefix[0] = ip >> 24;
							prefix[1] = (ip << 8) >> 24;
							prefix[2] = (ip << 16) >> 24;
							prefix[3] = (ip << 24) >> 24;
							prefix[4] = (17 + i);
#ifdef DEBUG
cout << "ip int " << begin << endl;
							cout << "level = " << 17 + i << " find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;
						
#endif // DEBUG

				//				fout << "ip int " << begin << endl;
							//fout << "level = " << 17 + i << " find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;
							
							break;
						}
						times++;
						/*
						prefix[0] = ip >> 24;
						sprintf(tmp, "%d", prefix[0]);
						strip.append(tmp);
						strip.append(".");
						prefix[1] = (ip << 8) >> 24;
						memset(tmp, 0, sizeof(tmp));
						sprintf(tmp, "%d", prefix[1]);
						strip.append(tmp);
						strip.append(".");
						prefix[2] = (ip << 16) >> 24;
						memset(tmp, 0, sizeof(tmp));
						sprintf(tmp, "%d", prefix[2]);
						strip.append(tmp);
						strip.append(".");
						prefix[3] = (ip << 24) >> 24;
						memset(tmp, 0, sizeof(tmp));
						sprintf(tmp, "%d", prefix[3]);
						strip.append(tmp);
						strip.append("/");
						prefix[4] = (unsigned char)(17 + i);
						sprintf(tmp, "%d", prefix[4]);
						strip.append(tmp);

						cout << " ip is " << strip << endl;
						

						if (routerSet.find(strip)!=routerSet.end())
						{							
							break;
						}	
						*/

					}//for i 
					if (i == -1)
					{

						cout << "sssssssssssssssssssssssssssssssssssss" << endl;
						ip = begin;
						prefix[0] = ip >> 24;
						prefix[1] = (ip << 8) >> 24;
						prefix[2] = (ip << 16) >> 24;
						prefix[3] = (ip << 24) >> 24;
						prefix[4] = (unsigned char)(17 + i);
#ifdef DEBUG
cout << "ip int " << begin << endl;
						cout <<  " not find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] <<  endl;

#endif // DEBUG

						
						fout << "sssssssssssssssssssssssssssssssssssss" << endl;

						error_outofi++;
					}
				//	Sleep(10000);
					if (i>-1)
					access_num[times-1]++;

				}//extraBf==NULL
				else if (extraBF != NULL)
				{
					int extraBf_mapping_num = 0;
					for (int i = 0; i < BF_NUM; i++)
					{						
						if (mapping_bf_level[i] == 1)
						{
							ip = begin;
							ip = ip >> (32 - (17 + i));
							ip = ip << (32 - (17 + i));
							prefix[0] = ip >> 24;
							prefix[1] = (ip << 8) >> 24;
							prefix[2] = (ip << 16) >> 24;
							prefix[3] = (ip << 24) >> 24;
							prefix[4] = (unsigned char)(17 + i);

							if (extraBF->query(prefix, PREFIX_LEN))
							{
								extraBf_mapping_num++;
								extra_mapping_times++;
							}
							else
							{
								extra_not_mapping_times++;

								mapping_bf_level[i] = 0;
							}
						}

					}
					if (extraBf_mapping_num == 1) 
					{
						extra_bf_work++;
						access_num[0]++;
						ip = begin;
						for (int i = 0; i < BF_NUM; i++)
						{
							if (mapping_bf_level[i] == 1)
							{  
								/*
								if (MergedFib->FindIp(ip, i+17))
								{
									ip = begin;
									prefix[0] = ip >> 24;
									prefix[1] = (ip << 8) >> 24;
									prefix[2] = (ip << 16) >> 24;
									prefix[3] = (ip << 24) >> 24;
									prefix[4] = (unsigned char)(17 + i);
#ifdef DEBUG
cout << "ip int " << begin << endl;
cout << "level = " << 17 + i << " find ip in extraBf_mapping once,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;
								
#endif // DEBUG

							//		fout << "ip int " << begin << endl;
									//fout << "level = " << 17 + i << " find ip in extraBf_mapping once,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

									break;
								}
								else
								{
									error_ip_extra_mappingnoce++;

								}*/
								break;
							}//if 
							
						}//for 
						
						
					}
					else ///  extraBF is false positive,find the ip in MergedTrie
					{
						int i;
						string strip;
						int times = 1;
//						char tmp[4];
						for (i = BF_NUM - 1; i >= 0; i--)
						{
							if (mapping_bf_level[i] == 0)
								continue;
							ip = begin;
							ip = ip >> (32 - (17 + i));
							ip = ip << (32 - (17 + i));
							if (MergedFib->FindIp(ip, 17 +i))
							{
								ip = begin;
								prefix[0] = ip >> 24;
								prefix[1] = (ip << 8) >> 24;
								prefix[2] = (ip << 16) >> 24;
								prefix[3] = (ip << 24) >> 24;
								prefix[4] = (unsigned char)(17 + i);
#ifdef DEBUG
cout << "ip int " << begin << endl;
cout <<"level = "<< 17+i<< " extra : find ip in extra MergedFib ,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;
								
#endif // DEBUG

								
						//		fout << "ip int " << begin << endl;
								//fout << "level = " << 17 + i << " extra : find ip in extra MergedFib ,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

								break;
							}
							else
							{
								
							}
							times++;

						}// end for
						if (i == -1)
						{

							cout << "extra :sssssssssssssssss" << endl;
							ip = begin;
							prefix[0] = ip >> 24;
							prefix[1] = (ip << 8) >> 24;
							prefix[2] = (ip << 16) >> 24;
							prefix[3] = (ip << 24) >> 24;
							prefix[4] = (unsigned char)(17 + i);
#ifdef DEBUG
cout << "ip int " << begin << endl;
							cout << " not find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << endl;

#endif // DEBUG

							
							fout << "extra :sssssssssssssssss not find ip " <<ip<< endl;
							error_outofi++;
						}
						//Sleep(10000);
						if (i>-1)
						access_num[times - 1]++;
						

					}// else find the ip in MergedTrie

				}//else if extraBF !=NULL
			}// bf mapping more than twice 
		}// else :not mapping 16 bf	
	}// while fin.eof()

	fin.close();
	for (int i = 0; i < BF_NUM; i++)
	{
		cout << "Num of querry " << i + 1 << " times hash table(MergedTrie): " << access_num[i] << endl;
		fout << "Num of querry " << i + 1 << " times hash table(MergedTrie): " << access_num[i] << endl;
		foutR << "Num of querry " << i + 1 << " times hash table(MergedTrie): " << access_num[i] << endl;

		foutR4 << setiosflags(ios::left) << setiosflags(ios::fixed) << setprecision(7);
		foutR4 << setw(14) << access_num[i];

	}
	foutR4 << endl;


	cout << "ip mapping more than twice " << bf_query_more_than_twice << endl;
	cout << "total num : " << total_num << endl;//// totalnum = access[0] +...+access[15]
	cout << " bit_array_work: " << bitarray_work_num << " bf_querryonce: " << bf_query_once << endl;// bitarray_work_num+
	cout << " ip nomapping: " << bf_nomapping << endl;
	cout << "error_ip_exxtra_mappingonce" << error_ip_extra_mappingnoce << endl;
	cout << " extra_mapping_times : " << extra_mapping_times << endl;
	cout << " extra_not_mapping_times: " << extra_not_mapping_times << endl;
	
	if (extraBF != NULL)
		cout << "extra msg: m,n,k " << extraBF->Get_bf_m() << ends << extraBF->Get_bf_n() << " " << extraBF->Get_bf_k() << endl;
	else
		cout << "ssssssssssssssssssssssssssssssss" << endl;

	fout << "ip mapping more than twice " << bf_query_more_than_twice << endl;
	fout << "total num : " << total_num << endl;//// totalnum = access[0] +...+access[15]
	fout << " bit_array_work: " << bitarray_work_num << " bf_querryonce: " << bf_query_once << endl;// bitarray_work_num+
	fout << " extra_mapping_times : " << extra_mapping_times << endl;
	fout << " extra_not_mapping_times: " << extra_not_mapping_times << endl;

	foutR << "ip mapping more than twice " << bf_query_more_than_twice << endl;
	foutR << "total num : " << total_num << endl;//// totalnum = access[0] +...+access[15]
	foutR << " bit_array_work: " << bitarray_work_num << " bf_querryonce: " << bf_query_once << endl;// bitarray_work_num+
	foutR << " extra_mapping_times : " << extra_mapping_times << endl;
	foutR << " extra_not_mapping_times: " << extra_not_mapping_times << endl;

	if (NULL != extraBF)
	{
		cout << "extra workd : " << extra_bf_work << endl;
		fout << "extra workd : " << extra_bf_work << endl;
		foutR << "extra workd : " << extra_bf_work << endl;

	}
	cout << "error_not_find_ip is " << error_not_find_ip<< " error_outofi: " << error_outofi << endl;
	fout<< "error_not_find_ip is " << error_not_find_ip << " error_outofi: " << error_outofi << endl;
	fout << "----------------------------------------------------------------------------" << endl;
	fout << "----------------------------------------------------------------------------" << endl;
	foutR << "error_not_find_ip is " << error_not_find_ip << " error_outofi: " << error_outofi << endl;
	foutR << "----------------------------------------------------------------------------" << endl;
	foutR << "----------------------------------------------------------------------------" << endl;
	foutR.close();
	foutR.close();
	fout.close();

}

/*
  file:duplicate,need set duplicate removal

*/

void bf_query(char *filename, CFib *MergedFib, StandardBF **a_BloomFilter, EXSBF *extraBF)
{
	static int k = 8;
	static int k_extar = 8;
	//char *filename = "rrc00_2013.6.8.8.txt";
	cout << "querying " << filename << " from bloom filter... " << endl;

	ifstream fin(filename);
	if (fin.fail())
	{
		cout << "open failed :" << filename << endl;
		return;
	}

	//	char str[LINE_LEN];
	//	char sPrefix[20];
	unsigned char prefix[PREFIX_LEN];
	//	int level;
	int j = 0;
	ofstream fout;
	char res_file[30] = "result_bfquery_";
	char tmp[3];
	
	if (extraBF == NULL)
	{
		sprintf(tmp, "%d", k);
		strcat(res_file, tmp);
		k++;
		//fout.open("result_bf_qurey.txt",ios::app);
		fout.open(res_file, ios::app);
	}
	else
	{
		sprintf(tmp, "%d", k_extar);
		strcat(res_file, tmp);
		k_extar++;
		//fout.open("result_extrabf_qurey.txt",ios::app);
		strcat(res_file, "extra");
		fout.open(res_file, ios::app);
	}

	ofstream foutR("result_ratio.txt",ios::app);
	if (extraBF == NULL)
	{
		fout << "##################################################################" << endl;
		fout << "#########################    NEW    TEST   #######################" << endl;
		fout << "######################     sdf  k = " << k-1 << "    #################" << endl;
		fout << "##################################################################" << endl;

		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "######################       sbf ratio       ####################" << endl;
		foutR << "######################       k = " << k - 1 << "     ####################" << endl;
		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "---------------------------------------------------------------------" << endl;

	}
	else
	{
		fout << "##################################################################" << endl;
		fout << "#########################    NEW    TEST   #######################" << endl;
		fout << "######################     extra  k = " << k_extar-1 << "    #################" << endl;
		fout << "##################################################################" << endl;


		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "######################       extraBF   ratio         #################" << endl;
		foutR << "######################       k = " << k_extar - 1 << "     ####################" << endl;
		foutR << "---------------------------------------------------------------------" << endl;
		foutR << "---------------------------------------------------------------------" << endl;

	}


	/*
	if (extraBF == NULL)
	{
		fout.open("result_bf_qurey.txt", ios::app);
	}
	else
	{
		k = 8;
		fout.open("result_extrabf_qurey.txt", ios::app);
	}

	if (fout.fail())
	{
		cout << "open outputfile failed.. " << endl;
		return;
	}
	*/
	//ofstream fout1("extrabf_query.txt");
	set<unsigned int> v;

	unsigned int ip;
	int  count = 0;
	while (!fin.eof())
	{
		fin >> ip;
		//if (ip < 33554432)
		//	continue;

		//cout << ip << " ip ";
		v.insert(ip);

		count++;
		if (count == 1000000)
		{
			count = 0;
			cout << "count = 1M, running waiting..." << endl;
			//break;

		}
	}
	fin.close();
	

	
	//	cout << "v.size = " << v.size() << endl;
	//fout << "v.size = " << v.size() << endl;


	set<unsigned int>::iterator begin = v.begin();
	set<unsigned int>::iterator end = v.end();

	unsigned int access_num[16] = { 0 };

	unsigned int bitarray_work_num = 0; //bitarray work
	unsigned int bf_query_once = 0;// 16个bf只返回一个结果
	unsigned int bf_query_more_than_twice = 0;//16个bf查询返回多于1个结果
	unsigned int extra_bf_work = 0; //extraBF 过滤起作用的ip个数（即通过extra_bf可去掉fp的结果）

	unsigned short bf_mapping_num = 0; //ip映射成功的bf的个数
	//unsigned short int mapping_bf_level[16];
	bitset<16> mapping_bf_level(0); //record the level	 that mapped the ip 

	unsigned int error_not_find_ip = 0;
	unsigned int error_outofi = 0;
	unsigned int error_extra_not_find_ip = 0;

	int findIplevel;
	int bf_nomapping = 0;
	int error_ip_bf_map_once = 0;
    int error_ip_bitarray_once = 0;
	int error_ip_extra_mappingtwicemore = 0;
	int error_ip_extra_mappingnoce = 0;
	int extra_mapping_times = 0;
	int extra_not_mapping_times = 0;
	int extra_do_wrong_choice = 0;
	for (begin = v.begin(); begin != end; begin++)
	{
		memset(prefix, 0, sizeof(prefix));
		mapping_bf_level.reset();
		//cout << "----------------------------------------------" << endl;
		fout << "----------------------------------------------" << endl;
		//fin >> ip;
		ip = *begin;
		//cout << ip << "  ";
		//	sprintf(sPrefix,"%u.%u.%u.%u", (ip >> 24), (ip << 8) >> 24, (ip << 16) >> 24, (ip << 24) >> 24);
		//	cout << sPrefix << endl;

		//	cout << "perfix " << (int)prefix[0] << " " << (int)prefix[1] << " " << (int)prefix[2] << " " << (int)prefix[3] << endl;

		ip = ip >> 16;
		cout << "ip >> 16 " << ip << " ip int " << *begin << endl;
		fout << "ip >> 16 " << ip << " ip int " << *begin << endl;
		if (bit_array[ip] == 1) // 前缀为16,15,14,....,1
		{
			if (bit_array[ip] == 1)
			{
				ip = ip << 16;
				/*		int prefixLen = 0;
				for (int t = 0; t < 16; t++)
				{
				if (((ip << t)&HIGHTBIT)==HIGHTBIT)  //HIGHTBIT, 2^31
				{
				prefixLen++;
				}
				}
				*/

				bitarray_work_num++;
				access_num[0]++;// 1次访hash的ip个数

				if ((findIplevel = MergedFib->FindIp(ip, 16)) != 0)  /// bit_array ,prefix = 16???
				{
					ip = *begin;
					prefix[0] = ip >> 24;
					prefix[1] = (ip << 8) >> 24;
					prefix[2] = (ip << 16) >> 24;
					prefix[3] = (ip << 24) >> 24;
					prefix[4] = 16;
					//	cout << " int ip :" << *begin << endl;

					//	cout <<"level:"<<prefixLen<< " find ip in bit_array,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << 16 << endl;

				//	fout << " int ip :" << *begin << endl;
					fout << "level:" << 16 << " find ip in bit_array,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << 16 << endl;

					//	Sleep(10000);
				}
				else
				{
					error_not_find_ip++; //should find the ip ,but MergedTrie->FindIp return 0;
					error_ip_bitarray_once++;
				}
			}
		}
		else  //search
		{
			bf_mapping_num = 0;
			for (int i = 0; i < BF_NUM; i++)
			{
				ip = *begin;
				ip = ip >> (32 - (17 + i));
				ip = ip << (32 - (17 + i));
				prefix[0] = ip >> 24;
				prefix[1] = (ip << 8) >> 24;
				prefix[2] = (ip << 16) >> 24;
				prefix[3] = (ip << 24) >> 24;
				prefix[4] = (17 + i);
				if (a_BloomFilter[i]->query(prefix, PREFIX_LEN))
				{
					bf_mapping_num++;
					mapping_bf_level[i] = 1;

#ifdef DEBUG
					cout << "ip int " << *begin << endl;
					cout << "level = " << 17 + i << " find ip in bf->query,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

#endif // DEBUG



					fout << "ip int " << *begin << endl;
					fout << "level = " << 17 + i << " find ip in bf->query,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

				}

			}//for 

			if (bf_mapping_num == 0)
			{
#ifdef DEBUG
				cout << "so what...." << endl;
#endif // DEBUG


				ip = *begin;
				prefix[0] = ip >> 24;
				prefix[1] = (ip << 8) >> 24;
				prefix[2] = (ip << 16) >> 24;
				prefix[3] = (ip << 24) >> 24;
				prefix[4] = 0;
#ifdef DEBUG
				cout << "ip int " << *begin << endl;
				cout << " not find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << endl;

#endif // DEBUG


				fout << "ip int " << *begin << endl;
				fout << " not find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << endl;


				bf_nomapping++;

			}
			else if (bf_mapping_num == 1)
			{
#ifdef DEBUG

				cout << "bf_mapping_num==1" << endl;
#endif // DEBUG

				bf_query_once++;

				access_num[0]++;//只查询一次hash的ip个数


				string strip = "";
				//				char tmp[4];
				int i;
				for (i = 0; i < 16; i++)
				{
					if (mapping_bf_level[i] == 1)
					{
						prefix[4] = (unsigned char)(17 + i);
						break;

					}
				}

				if (i == 16)
				{
					return;
				}

				ip = *begin;


				ip = ip >> (32 - 17 - i);
				ip = ip << (32 - 17 - i);

				if (MergedFib->FindIp(ip, 17 + i))
				{
					ip = *begin;
					prefix[0] = ip >> 24;
					prefix[1] = (ip << 8) >> 24;
					prefix[2] = (ip << 16) >> 24;
					prefix[3] = (ip << 24) >> 24;
					prefix[4] = (17 + j);
#ifdef DEBUG
					cout << "ip int " << *begin << endl;
					cout << "level = " << 17 + i << " find ip in bf query once,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

#endif // DEBUG


					fout << "ip int " << *begin << endl;
					fout << "level = " << 17 + i << " find ip in bf query once,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

				}
				else
				{
					error_not_find_ip++;
					error_ip_bf_map_once++;
				}

			}//bf_mapping_num ==1
			else if (bf_mapping_num >1)
			{
				bf_query_more_than_twice++;
#ifdef DEBUG
				cout << "bf_query_more than twice " << endl;
#endif // DEBUG


				//access_num[bf_mapping_num-1]++;
				if (extraBF == NULL) //从最长前缀开始查找，访问trie树，查看访问次数
				{
					string strip;
//					char tmp[4];
					int i;
					int times = 1;
					for (i = BF_NUM - 1; i >= 0; i--)
					{
						if (mapping_bf_level[i] == 0)
							continue;
						cout << "extral ==NULL  i:" << i << endl;
					//	strip = "";
					//	memset(tmp, 0, sizeof(tmp));
						ip = *begin;
#ifdef DEBUG
						cout << " ip int " << ip << " i: " << i << endl;
#endif // DEBUG


						fout << " ip int " << ip << endl;
						ip = ip >> (32 - 17 - i);
						ip = ip << (32 - 17 - i);//ip prefix
						if (MergedFib->FindIp(ip, 17 + i))
						{
							ip = *begin;
							prefix[0] = ip >> 24;
							prefix[1] = (ip << 8) >> 24;
							prefix[2] = (ip << 16) >> 24;
							prefix[3] = (ip << 24) >> 24;
							prefix[4] = (17 + i);
#ifdef DEBUG
							cout << "ip int " << *begin << endl;
							cout << "level = " << 17 + i << " find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

#endif // DEBUG

							fout << "ip int " << *begin << endl;
							fout << "level = " << 17 + i << " find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;
							
							break;
						}
						times++;
						/*
						prefix[0] = ip >> 24;
						sprintf(tmp, "%d", prefix[0]);
						strip.append(tmp);
						strip.append(".");
						prefix[1] = (ip << 8) >> 24;
						memset(tmp, 0, sizeof(tmp));
						sprintf(tmp, "%d", prefix[1]);
						strip.append(tmp);
						strip.append(".");
						prefix[2] = (ip << 16) >> 24;
						memset(tmp, 0, sizeof(tmp));
						sprintf(tmp, "%d", prefix[2]);
						strip.append(tmp);
						strip.append(".");
						prefix[3] = (ip << 24) >> 24;
						memset(tmp, 0, sizeof(tmp));
						sprintf(tmp, "%d", prefix[3]);
						strip.append(tmp);
						strip.append("/");
						prefix[4] = (unsigned char)(17 + i);
						sprintf(tmp, "%d", prefix[4]);
						strip.append(tmp);

						cout << " ip is " << strip << endl;


						if (routerSet.find(strip)!=routerSet.end())
						{
						break;
						}
						*/

					}//for i 
					if (i == -1)
					{

						cout << "sssssssssssssssssssssssssssssssssssss" << endl;
						ip = *begin;
						prefix[0] = ip >> 24;
						prefix[1] = (ip << 8) >> 24;
						prefix[2] = (ip << 16) >> 24;
						prefix[3] = (ip << 24) >> 24;
						prefix[4] = (unsigned char)(17 + i);
#ifdef DEBUG
						cout << "ip int " << *begin << endl;
						cout << " not find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << endl;

#endif // DEBUG


						fout << "sssssssssssssssssssssssssssssssssssss" << endl;

						error_outofi++;
					}
					//	Sleep(10000);
					if (i>-1)
						access_num[times - 1]++;

				}//extraBf==NULL
				else if (extraBF != NULL)
				{
					int extraBf_mapping_num = 0;
				//	int flag_i = -1;
					for (int i = 0; i < BF_NUM; i++)
					{
						if (mapping_bf_level[i] == 1)
						{
							ip = *begin;
							ip = ip >> (32 - (17 + i));
							ip = ip << (32 - (17 + i));
							prefix[0] = ip >> 24;
							prefix[1] = (ip << 8) >> 24;
							prefix[2] = (ip << 16) >> 24;
							prefix[3] = (ip << 24) >> 24;
							prefix[4] = (17 + i);
							

							if (extraBF->query(prefix, PREFIX_LEN))
							{   
							//	flag_i = i;
								extraBf_mapping_num++;

								extra_mapping_times++;
							}
							else
							{
								ip = *begin;
								ip = ip >> (32 - 17 - i);
								ip = ip << (32 - 17 - i);//
								if (MergedFib->FindIp(ip, 17 + i))
									extra_do_wrong_choice++;

								extra_not_mapping_times++;
								mapping_bf_level[i] = 0;
								
							}
							
						}

					}
					if (extraBf_mapping_num == 1)
					{
						extra_bf_work++;
						access_num[0]++;
						

						for (int i = 0; i < BF_NUM; i++)
					//	int i = flag_i;
						{
							if (mapping_bf_level[i] == 1)
							{
								ip = *begin;
								ip = ip >> (32 - 17 - i);
								ip = ip << (32 - 17 - i);//ip prefix
								if (MergedFib->FindIp(ip, i + 17))
								{
									ip = *begin;
									prefix[0] = ip >> 24;
									prefix[1] = (ip << 8) >> 24;
									prefix[2] = (ip << 16) >> 24;
									prefix[3] = (ip << 24) >> 24;
									prefix[4] = (unsigned char)(17 + i);
#ifdef DEBUG
									cout << "ip int " << *begin << endl;
									cout << "level = " << 17 + i << " find ip in extraBf_mapping once,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

#endif // DEBUG

									fout << "ip int " << *begin << endl;
									fout << "level = " << 17 + i << " find ip in extraBf_mapping once,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

									break;
								}
							}
							else
							{
								error_not_find_ip++;
								error_ip_extra_mappingnoce++;
								cout << " ############ not found ip int " << *begin << endl;
								fout << " ############ not found ip int " << *begin << " ip >> 16 " << (ip >> 16) << endl;
							}
						}

					}
					else if(extraBf_mapping_num>1)///  extraBF is false positive,find the ip in MergedTrie
					{
						int i;
						string strip;
						//						char tmp[4];
						int times = 1;
						for (i = BF_NUM - 1; i >= 0; i--)
						{
							
							if (mapping_bf_level[i] == 0)
								continue;
							ip = *begin;
							ip = ip >> (32 - 17 - i);
							ip = ip << (32 - 17 - i);//ip prefix

							if (MergedFib->FindIp(ip, 17 + i))
							{
								ip = *begin;
								prefix[0] = ip >> 24;
								prefix[1] = (ip << 8) >> 24;
								prefix[2] = (ip << 16) >> 24;
								prefix[3] = (ip << 24) >> 24;
								prefix[4] = (17 + i);
#ifdef DEBUG
								cout << "ip int " << *begin << endl;
								cout << "level = " << 17 + i << " extra : find ip in extra MergedFib ,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;

#endif // DEBUG


								fout << "ip int " << *begin << endl;
								fout << "level = " << 17 + i << " extra : find ip in extra MergedFib ,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << "/" << (int)prefix[4] << endl;
								
								break;
							}
							else
							{
								
							
								
							}
							times++;

						}// end for
						if (i == -1)
						{

							error_not_find_ip++;
							error_ip_extra_mappingtwicemore++;
							cout << " ############ not found ip int " << *begin << endl;
							fout << " ############ not found ip int " << *begin << " ip >> 16 " << (ip >> 16) << endl;


							cout << "extra :sssssssssssssssss" << endl;
							ip = *begin;
							prefix[0] = ip >> 24;
							prefix[1] = (ip << 8) >> 24;
							prefix[2] = (ip << 16) >> 24;
							prefix[3] = (ip << 24) >> 24;
							prefix[4] = (unsigned char)(17 + i);
#ifdef DEBUG
							cout << "ip int " << *begin << endl;
							cout << " not find ip in bf MergedTrie,ip is: " << (int)prefix[0] << "." << (int)prefix[1] << "." << (int)prefix[2] << "." << (int)prefix[3] << endl;

#endif // DEBUG


							fout << "extra :sssssssssssssssss" << endl;
							error_outofi++;
						}
						//Sleep(10000);
						if (i>-1)
							access_num[times - 1]++;


					}// else find the ip in MergedTrie
					else if (extraBf_mapping_num == 0)
					{
						cout << "extraBf_mappping num ==0 " << endl;
						fout << "extraBf_mappping num ==0 " << endl;
						foutR << "extraBf_mappping num ==0 " << endl;
					}

				}//else if extraBF !=NULL
			}// bf mapping more than twice 
		}// else :not mapping 16 bf	
	}// for set iterator

	for (int i = 0; i < BF_NUM; i++)
	{
		cout << "Num of querry " << i + 1 << " times hash table(MergedTrie): " << access_num[i] << endl;
		fout << "Num of querry " << i + 1 << " times hash table(MergedTrie): " << access_num[i] << endl;
		foutR << "Num of querry " << i + 1 << " times hash table(MergedTrie): " << access_num[i] << endl;
	}


	cout << "ip mapping more than twice " << bf_query_more_than_twice << endl;
	cout << "total num : " << v.size() << endl;//// totalnum = access[0] +...+access[15]
	cout << " bit_array_work: " << bitarray_work_num << " bf_querryonce: " << bf_query_once << endl;// bitarray_work_num+
	cout << " ip nomapping: " << bf_nomapping << endl;
	cout << " extra_mapping_times : " << extra_mapping_times << endl;
	cout << " extra_not_mapping_times: " << extra_not_mapping_times << endl;
	cout << " extra_do_wrong_choice : " << extra_do_wrong_choice << endl;

	fout << "ip mapping more than twice " << bf_query_more_than_twice << endl;
	fout << "total num : " << v.size() << endl;//// totalnum = access[0] +...+access[15]
	fout << " bit_array_work: " << bitarray_work_num << " bf_querryonce: " << bf_query_once << endl;// bitarray_work_num+
	fout << " extra_mapping_times : " << extra_mapping_times << endl;
	fout << " extra_not_mapping_times: " << extra_not_mapping_times << endl;
	fout << " extra_do_wrong_choice : " << extra_do_wrong_choice << endl;

	foutR << "ip mapping more than twice " << bf_query_more_than_twice << endl;
	foutR << "total num : " << v.size() << endl;//// totalnum = access[0] +...+access[15]
	foutR << " bit_array_work: " << bitarray_work_num << " bf_querryonce: " << bf_query_once << endl;// bitarray_work_num+
	foutR << " extra_mapping_times : " << extra_mapping_times << endl;
	foutR << " extra_not_mapping_times: " << extra_not_mapping_times << endl;
	foutR << " extra_do_wrong_choice : " << extra_do_wrong_choice << endl;
	
	if (NULL != extraBF)
	{
		cout << "extra workd : " << extra_bf_work << endl;
		fout << "extra workd : " << extra_bf_work << endl;
		foutR << "extra workd : " << extra_bf_work << endl;

	}
	cout << "error_not_find_ip is " << error_not_find_ip << " error_outofi: " << error_outofi << endl;
	
	cout << "error_extra not_find_ip is " << error_extra_not_find_ip << endl;
	cout << "error_ip_bf_map_once :" << error_ip_bf_map_once << endl;
	cout << "error_ip_bitarray_once :" << error_ip_bitarray_once << endl;
	cout << "error_ip_exxtra_mappingtwicemore" << error_ip_extra_mappingtwicemore << endl;
	cout << "error_ip_exxtra_mappingonce" << error_ip_extra_mappingnoce << endl;
	if (extraBF!=NULL)
		cout << "extra msg: m,n,k " << extraBF->Get_bf_m() << ends << extraBF->Get_bf_n() << " " << extraBF->Get_bf_k() << endl;

	fout << "error_not_find_ip is " << error_not_find_ip << " error_outofi: " << error_outofi << endl;
	fout << "----------------------------------------------------------------------------" << endl;
	fout << "----------------------------------------------------------------------------" << endl;
	fout << "error_extra not_find_ip is " << error_extra_not_find_ip << endl;
	fout << "error_ip_bf_map_once :" << error_ip_bf_map_once << endl;
	fout << "error_ip_bitarray_once :" << error_ip_bitarray_once << endl;
	fout << "error_ip_exxtra_mappingtwicemore" << error_ip_extra_mappingtwicemore << endl;


	foutR << "error_not_find_ip is " << error_not_find_ip << " error_outofi: " << error_outofi << endl;
	foutR << "error_extra not_find_ip is " << error_extra_not_find_ip << endl;
	foutR << "error_ip_bf_map_once :" << error_ip_bf_map_once << endl;
	foutR << "error_ip_bitarray_once :" << error_ip_bitarray_once << endl;
	foutR << "error_ip_exxtra_mappingtwicemore" << error_ip_extra_mappingtwicemore << endl;
	foutR << "----------------------------------------------------------------------------" << endl;
	foutR << "----------------------------------------------------------------------------" << endl;

	foutR.close();
	fout.close();

}


/*
  filename="MergedTrie",extra bf =8, bfk = 8,--16
*/

int construct_bloom_filter(int kk,char *filename, StandardBF **a_BloomFilter,EXSBF *extraBF, const int merged_level_node_num[],int bf_num)
{
	cout << " constructing the bloom_filter ...." << endl;
	//	clock_t start, end;
	//	start = clock(); //开始时，取得开始时间。
	//time_t t_s, t_e;
	//time(&t_s);
	int x = 0, j = 0;
	//unsigned char prefix[PREFIX_NUM][PREFIX_LEN];
	unsigned char prefix[PREFIX_LEN];
	unsigned int k=kk;
	unsigned int m;
	unsigned int num_of_level_push_to_bitarray[16];
	memset(num_of_level_push_to_bitarray, 0, sizeof(num_of_level_push_to_bitarray));

	ofstream fout;
	if (extraBF == NULL)
	{
		fout.open("result_bf_qurey.txt", ios::app);
	}
	else
	{
		fout.open("result_extrabf_qurey.txt", ios::app);
	}

	if (fout.fail())
	{
		cout << "open outputfile failed.. " << endl;
		return -1;
	}

	//unsigned char *bit_array = bitarray;
	//unsigned int k_array_1[] = { 5, 5, 6, 6, 7, 7, 8, 9, 9, 10, 10, 10, 11, 11, 12, 12 }; // case 1
//	unsigned int k_array_2[] = { 6, 6, 6, 6, 6, 6, 6, 11, 6, 6, 6, 6, 6, 6, 6, 6 };     // case 2
	unsigned int extra_n = 0;
	for (int i = 0; i < bf_num; i++)
	{
		//cout << "in CBF level "<<i<<"  "<< merged_level_node_num[i] << endl;
		//if (merged_level_node_num[i] <= 0)
			//continue;
		if (kk == 1)
		{
		k = k_array_1[i]; //function num
		m = (unsigned int)((k * merged_level_node_num[i+17] / log(2))+0.5); // level 17 to level 32
		extra_n += merged_level_node_num[i + 17];
		a_BloomFilter[i]->initial(m,k);
		}
		else if (kk == 2)
		{
			k = k_array_2[i]; //function num
			m = (unsigned int)((k * merged_level_node_num[i + 17] / log(2)) + 0.5); // level 17 to level 32
			extra_n += merged_level_node_num[i + 17];
			a_BloomFilter[i]->initial(m, k);

		}

		cout << " bf level " << i << " m " << m << endl;	
	}
	cout <<"extra_n :" <<extra_n << endl;
	fout << "extra_n :" << extra_n << endl;
	if (extraBF != NULL)
	{
			k = 8; //extraBF  k is less than BF,so that the extrabf is different from bf at the hash maybe same,rather the m is not,mapping algorithm is different
			m = (unsigned int)((k*double(extra_n) / log(2)) + 0.5); // 
			extraBF->initial(m,k);
	}
	
	ifstream fin(filename);//filename=MergedTrie

	char str[LINE_LEN];
	char sPrefix[20];
	int level;

	unsigned int total_num = 0;
	unsigned int bitarray_num = 0;
	int insertcount = 0;
	while (!fin.eof())
	{
		total_num++;
		memset(sPrefix, 0, sizeof(sPrefix));
		memset(prefix,0,sizeof(prefix));

		fin >> sPrefix >> str;

	
	//	routerSet.insert(sPrefix);
		int iStart = 0;
		int iEnd = 0;
		int iLen = strlen(sPrefix);
		int prefix_b = 0;
	
		for (int i = 0; i<iLen; i++)
		{
			
			if (sPrefix[i] == '.')
			{
				iEnd = i;
				string strVal(sPrefix + iStart, iEnd - iStart);
				prefix[prefix_b] = (unsigned char)atoi(strVal.c_str());
			//	cout << (int)prefix[prefix_b] << "."<< endl;
				iStart = i + 1;
				i++;
				prefix_b++;
			}///if "."
			if (sPrefix[i] == '/')
			{

				iEnd = i;
				string strVal(sPrefix + iStart, iEnd - iStart);
				prefix[prefix_b] = (char)atoi(strVal.c_str());
				//string strtmp = sPrefix.substr(iStart, iEnd - iStart);
				//prefix[prefix_b] = (char)atoi(strtmp.c_str());
			//	cout << (int)prefix[prefix_b] << "/";
				iStart = i + 1;
				i++;
				prefix_b++;

				strVal = string(sPrefix + iStart);
				level = atoi(strVal.c_str());
		
				if (level <= 16) // insert to bitarray
				{
					int insert_num_area = pow(2.0, (16 - level));
					num_of_level_push_to_bitarray[level - 1] += insert_num_area;
					unsigned int value = 0;
					value +=(int)prefix[1];
					value += prefix[0] << 8;

					for (int i = 0; i < insert_num_area; i++)
					{
						
						bit_array[value+i]=1;				    
						bitarray_num++;
					}

					// 65535/8+1
					// bit_array[value%8192] |= 128>>(-7value%8);
					//	cout <<"bit array: "<< (int)bit_array[1] << ends<<(int)bit_array[0]<<endl;
					
		//			cout << "value " << ends;
					//cout <<bit_array<< endl;
					//Sleep(5000);
				}
				else
				{
					prefix[prefix_b] = (char)level;
				//	cout << (int)prefix[prefix_b] << " level " << level << " " << str << endl;
				
					a_BloomFilter[level-17]->insert(prefix, PREFIX_LEN);//bf_num total is 16

					if (extraBF != NULL)
					{
						extraBF->insert(prefix,PREFIX_LEN);
					}

					//a_BloomFilter[level]->outputOHABF("sbf1.bf");
				}				
			}			
		}//for
				
	}//while
	fin.close();

	unsigned int bf_sum = 0;
	for (int i = 0; i < bf_num; i++)
	{
		cout << "in CBF level " << i << "  " << merged_level_node_num[i+17] << endl;
	//	if (merged_level_node_num[i] <= 0)
		//	continue;

		k = 10; //function num
		m = (int)(k * merged_level_node_num[i+17] / log(2)+0.5);
	///	a_BloomFilter[i] = new StandardBF(m, k);
		cout << " bf level " << i << " m " << m << endl;		
		cout << "k = " << a_BloomFilter[i]->Get_bf_k() << endl;
		cout << "m = " << a_BloomFilter[i]->Get_bf_m() << endl;
		cout << "n = " << a_BloomFilter[i]->Get_bf_n() << endl;

		fout << " bf level " << i << " m " << m << endl;
		fout << "k = " << a_BloomFilter[i]->Get_bf_k() << endl;
		fout << "m = " << a_BloomFilter[i]->Get_bf_m() << endl;
		fout << "n = " << a_BloomFilter[i]->Get_bf_n() << endl;

		bf_sum += a_BloomFilter[i]->Get_bf_n();
		//test_bf_query(a_BloomFilter[level]);

	}
	if (extraBF != NULL)
	{
		cout << endl;
		cout << "extraBf: " << endl;
		fout << "extraBf: " << endl;
		fout << "k = " << extraBF->Get_bf_k() << endl;
		fout << "m = " << extraBF->Get_bf_m() << endl;
		fout << "n = " << extraBF->Get_bf_n() << endl;
	}
	
	cout << "in CBF total_num: " << total_num  << " bit_array count: " << bit_array.count() << " bitarray_num " << bitarray_num << " bf_sum " << bf_sum << endl;
	fout << "in CBF total_num: " << total_num << " bit_array count: " << bit_array.count() << " bitarray_num " << bitarray_num << " bf_sum " << bf_sum << endl;

	for (int i = 0; i < 16; i++)
	{
		cout << "level " << i + 1 << " push to bitarray num : " << num_of_level_push_to_bitarray[i] << endl;
		fout << "level " << i + 1 << " push to bitarray num : " << num_of_level_push_to_bitarray[i] << endl;
	}
	//bf_query(a_BloomFilter);

	fout.close();
	
	return 0;
}


/*
  bf k = 8
  extrabf k =4,5,6,---16
*/
int construct_bloom_filter2(int case_index ,int kk, char *filename, StandardBF **a_BloomFilter, EXSBF *extraBF, const int merged_level_node_num[], int bf_num)
{
	cout << " constructing the bloom_filter ...." << endl;
	//	clock_t start, end;
	//	start = clock(); //开始时，取得开始时间。
	//time_t t_s, t_e;
	//time(&t_s);
	int x = 0, j = 0;
	//unsigned char prefix[PREFIX_NUM][PREFIX_LEN];
	unsigned char prefix[PREFIX_LEN];
	unsigned int k = kk;
	unsigned int m;
	unsigned int num_of_level_push_to_bitarray[16];
	memset(num_of_level_push_to_bitarray, 0, sizeof(num_of_level_push_to_bitarray));

	ofstream fout;
	if (extraBF == NULL)
	{
		fout.open("result_bf_qurey.txt", ios::app);
	}
	else
	{
		fout.open("result_extrabf_qurey.txt", ios::app);
	}

	if (fout.fail())
	{
		cout << "open outputfile failed.. " << endl;
		return -1;
	}

	//unsigned char *bit_array = bitarray;
	unsigned int extra_n = 0;
	for (int i = 0; i < bf_num; i++)
	{
		//cout << "in CBF level "<<i<<"  "<< merged_level_node_num[i] << endl;
		//if (merged_level_node_num[i] <= 0)
		//continue;
		if (case_index == 1)
		{
			k = k_array_1[i]; //function num
			m = (unsigned int)((k * merged_level_node_num[i + 17] / log(2)) + 0.5); // level 17 to level 32
			extra_n += merged_level_node_num[i + 17];
			a_BloomFilter[i]->initial(m, k);



		}
		else if (case_index == 2)
		{
			k = k_array_2[i]; //function num
			m = (unsigned int)((k * merged_level_node_num[i + 17] / log(2)) + 0.5); // level 17 to level 32
			extra_n += merged_level_node_num[i + 17];
			a_BloomFilter[i]->initial(m, k);

		}

		cout << " bf level " << i << " m " << m << endl;
	}
	cout << "extra_n :" << extra_n << endl;
	fout << "extra_n :" << extra_n << endl;
	if (extraBF != NULL)
	{
		k = kk; //extraBF  k is less than BF,so that the extrabf is different from bf at the hash maybe same,rather the m is not,mapping algorithm is different
		m = (unsigned int)((k*double(extra_n) / log(2)) + 0.5); // 
		extraBF->initial(m, k);
	}

	ifstream fin(filename);//filename=MergedTrie

	char str[LINE_LEN];
	char sPrefix[20];
	int level;

	unsigned int total_num = 0;
	unsigned int bitarray_num = 0;
	int insertcount = 0;
	while (!fin.eof())
	{
		total_num++;
		memset(sPrefix, 0, sizeof(sPrefix));
		memset(prefix, 0, sizeof(prefix));

		fin >> sPrefix >> str;


		//	routerSet.insert(sPrefix);
		int iStart = 0;
		int iEnd = 0;
		int iLen = strlen(sPrefix);
		int prefix_b = 0;

		for (int i = 0; i<iLen; i++)
		{

			if (sPrefix[i] == '.')
			{
				iEnd = i;
				string strVal(sPrefix + iStart, iEnd - iStart);
				prefix[prefix_b] = (unsigned char)atoi(strVal.c_str());
				//	cout << (int)prefix[prefix_b] << "."<< endl;
				iStart = i + 1;
				i++;
				prefix_b++;
			}///if "."
			if (sPrefix[i] == '/')
			{

				iEnd = i;
				string strVal(sPrefix + iStart, iEnd - iStart);
				prefix[prefix_b] = (char)atoi(strVal.c_str());
				//string strtmp = sPrefix.substr(iStart, iEnd - iStart);
				//prefix[prefix_b] = (char)atoi(strtmp.c_str());
				//	cout << (int)prefix[prefix_b] << "/";
				iStart = i + 1;
				i++;
				prefix_b++;

				strVal = string(sPrefix + iStart);
				level = atoi(strVal.c_str());

				if (level <= 16) // insert to bitarray
				{
					int insert_num_area = pow(2.0, (16 - level));
					num_of_level_push_to_bitarray[level - 1] += insert_num_area;
					unsigned int value = 0;
					value += (int)prefix[1];
					value += prefix[0] << 8;

					for (int i = 0; i < insert_num_area; i++)
					{

						bit_array[value + i] = 1;
						bitarray_num++;
					}

					// 65535/8+1
					// bit_array[value%8192] |= 128>>(-7value%8);
					//	cout <<"bit array: "<< (int)bit_array[1] << ends<<(int)bit_array[0]<<endl;

					//			cout << "value " << ends;
					//cout <<bit_array<< endl;
					//Sleep(5000);
				}
				else
				{
					prefix[prefix_b] = (char)level;
					//	cout << (int)prefix[prefix_b] << " level " << level << " " << str << endl;

					a_BloomFilter[level - 17]->insert(prefix, PREFIX_LEN);//bf_num total is 16

					if (extraBF != NULL)
					{
						extraBF->insert(prefix, PREFIX_LEN);
					}

					//a_BloomFilter[level]->outputOHABF("sbf1.bf");
				}
			}
		}//for

	}//while
	fin.close();

	unsigned int bf_sum = 0;
	for (int i = 0; i < bf_num; i++)
	{
		cout << "in CBF level " << i << "  " << merged_level_node_num[i + 17] << endl;
		//	if (merged_level_node_num[i] <= 0)
		//	continue;

		k = 10; //function num
		m = (int)(k * merged_level_node_num[i + 17] / log(2) + 0.5);
		///	a_BloomFilter[i] = new StandardBF(m, k);
		cout << " bf level " << i << " m " << m << endl;
		cout << "k = " << a_BloomFilter[i]->Get_bf_k() << endl;
		cout << "m = " << a_BloomFilter[i]->Get_bf_m() << endl;
		cout << "n = " << a_BloomFilter[i]->Get_bf_n() << endl;

		fout << " bf level " << i << " m " << m << endl;
		fout << "k = " << a_BloomFilter[i]->Get_bf_k() << endl;
		fout << "m = " << a_BloomFilter[i]->Get_bf_m() << endl;
		fout << "n = " << a_BloomFilter[i]->Get_bf_n() << endl;

		bf_sum += a_BloomFilter[i]->Get_bf_n();
		//test_bf_query(a_BloomFilter[level]);

	}
	if (extraBF != NULL)
	{
		cout << endl;
		cout << "extraBf: " << endl;
		fout << "extraBf: " << endl;
		fout << "k = " << extraBF->Get_bf_k() << endl;
		fout << "m = " << extraBF->Get_bf_m() << endl;
		fout << "n = " << extraBF->Get_bf_n() << endl;
	}

	cout << "in CBF total_num: " << total_num << " bit_array count: " << bit_array.count() << " bitarray_num " << bitarray_num << " bf_sum " << bf_sum << endl;
	fout << "in CBF total_num: " << total_num << " bit_array count: " << bit_array.count() << " bitarray_num " << bitarray_num << " bf_sum " << bf_sum << endl;

	for (int i = 0; i < 16; i++)
	{
		cout << "level " << i + 1 << " push to bitarray num : " << num_of_level_push_to_bitarray[i] << endl;
		fout << "level " << i + 1 << " push to bitarray num : " << num_of_level_push_to_bitarray[i] << endl;
	}
	//bf_query(a_BloomFilter);

	fout.close();

	return 0;
}



/*
  except 24level
*/
int construct_bloom_filter24(int case_index,int kk, char *filename, StandardBF **a_BloomFilter, EXSBF *extraBF, const int merged_level_node_num[], int bf_num)
{
	cout << " constructing the bloom_filter ...." << endl;
	//	clock_t start, end;
	//	start = clock(); //开始时，取得开始时间。
	//time_t t_s, t_e;
	//time(&t_s);
	int x = 0, j = 0;
	//unsigned char prefix[PREFIX_NUM][PREFIX_LEN];
	unsigned char prefix[PREFIX_LEN];
	unsigned int k = kk;
	unsigned int m;
	unsigned int num_of_level_push_to_bitarray[16];
	memset(num_of_level_push_to_bitarray, 0, sizeof(num_of_level_push_to_bitarray));

	ofstream fout;
	if (extraBF == NULL)
	{
		fout.open("result_bf_qurey.txt", ios::app);
	}
	else
	{
		fout.open("result_extrabf_qurey.txt", ios::app);
	}

	if (fout.fail())
	{
		cout << "open outputfile failed.. " << endl;
		return -1;
	}

	//unsigned char *bit_array = bitarray;
	unsigned int extra_n = 0;
	for (int i = 0; i < bf_num; i++)
	{
		//cout << "in CBF level "<<i<<"  "<< merged_level_node_num[i] << endl;
		//if (merged_level_node_num[i] <= 0)
		//continue;

		if (case_index == 1)
		{
			k = k_array_1[i]; //function num
			m = (unsigned int)((k * merged_level_node_num[i + 17] / log(2)) + 0.5); // level 17 to level 32
			extra_n += merged_level_node_num[i + 17];
			a_BloomFilter[i]->initial(m, k);



		}
		else if (case_index == 2)
		{
			k = k_array_2[i]; //function num
			m = (unsigned int)((k * merged_level_node_num[i + 17] / log(2)) + 0.5); // level 17 to level 32
			extra_n += merged_level_node_num[i + 17];
			a_BloomFilter[i]->initial(m, k);

		}

		cout << " bf level " << i << " m " << m << endl;
	}
	cout << "extra_n :" << extra_n << endl;
	fout << "extra_n :" << extra_n << endl;
	if (extraBF != NULL)
	{
		k = kk; //extraBF  k is less than BF,so that the extrabf is different from bf at the hash maybe same,rather the m is not,mapping algorithm is different
		m = (unsigned int)((k*double(extra_n) / log(2)) + 0.5); // 
		extraBF->initial(m, k);
	}

	ifstream fin(filename);//filename=MergedTrie

	char str[LINE_LEN];
	char sPrefix[20];
	int level;

	unsigned int total_num = 0;
	unsigned int bitarray_num = 0;
	int insertcount = 0;
	while (!fin.eof())
	{
		total_num++;
		memset(sPrefix, 0, sizeof(sPrefix));
		memset(prefix, 0, sizeof(prefix));

		fin >> sPrefix >> str;


		//	routerSet.insert(sPrefix);
		int iStart = 0;
		int iEnd = 0;
		int iLen = strlen(sPrefix);
		int prefix_b = 0;

		for (int i = 0; i<iLen; i++)
		{

			if (sPrefix[i] == '.')
			{
				iEnd = i;
				string strVal(sPrefix + iStart, iEnd - iStart);
				prefix[prefix_b] = (unsigned char)atoi(strVal.c_str());
				//	cout << (int)prefix[prefix_b] << "."<< endl;
				iStart = i + 1;
				i++;
				prefix_b++;
			}///if "."
			if (sPrefix[i] == '/')
			{

				iEnd = i;
				string strVal(sPrefix + iStart, iEnd - iStart);
				prefix[prefix_b] = (char)atoi(strVal.c_str());
				//string strtmp = sPrefix.substr(iStart, iEnd - iStart);
				//prefix[prefix_b] = (char)atoi(strtmp.c_str());
				//	cout << (int)prefix[prefix_b] << "/";
				iStart = i + 1;
				i++;
				prefix_b++;

				strVal = string(sPrefix + iStart);
				level = atoi(strVal.c_str());

				if (level <= 16) // insert to bitarray
				{
					int insert_num_area = pow(2.0, (16 - level));
					num_of_level_push_to_bitarray[level - 1] += insert_num_area;
					unsigned int value = 0;
					value += (int)prefix[1];
					value += prefix[0] << 8;

					for (int i = 0; i < insert_num_area; i++)
					{

						bit_array[value + i] = 1;
						bitarray_num++;
					}

					// 65535/8+1
					// bit_array[value%8192] |= 128>>(-7value%8);
					//	cout <<"bit array: "<< (int)bit_array[1] << ends<<(int)bit_array[0]<<endl;

					//			cout << "value " << ends;
					//cout <<bit_array<< endl;
					//Sleep(5000);
				}
				else
				{
					prefix[prefix_b] = (char)level;
					//	cout << (int)prefix[prefix_b] << " level " << level << " " << str << endl;

					a_BloomFilter[level - 17]->insert(prefix, PREFIX_LEN);//bf_num total is 16

					if (level != 24)
					{
						if (extraBF != NULL)
						{
							extraBF->insert(prefix, PREFIX_LEN);
						}
					}
					//a_BloomFilter[level]->outputOHABF("sbf1.bf");
				}
			}
		}//for

	}//while
	fin.close();

	unsigned int bf_sum = 0;
	for (int i = 0; i < bf_num; i++)
	{
		cout << "in CBF level " << i << "  " << merged_level_node_num[i + 17] << endl;
		//	if (merged_level_node_num[i] <= 0)
		//	continue;

		k = 10; //function num
		m = (int)(k * merged_level_node_num[i + 17] / log(2) + 0.5);
		///	a_BloomFilter[i] = new StandardBF(m, k);
		cout << " bf level " << i << " m " << m << endl;
		cout << "k = " << a_BloomFilter[i]->Get_bf_k() << endl;
		cout << "m = " << a_BloomFilter[i]->Get_bf_m() << endl;
		cout << "n = " << a_BloomFilter[i]->Get_bf_n() << endl;

		fout << " bf level " << i << " m " << m << endl;
		fout << "k = " << a_BloomFilter[i]->Get_bf_k() << endl;
		fout << "m = " << a_BloomFilter[i]->Get_bf_m() << endl;
		fout << "n = " << a_BloomFilter[i]->Get_bf_n() << endl;

		bf_sum += a_BloomFilter[i]->Get_bf_n();
		//test_bf_query(a_BloomFilter[level]);

	}
	if (extraBF != NULL)
	{
		cout << endl;
		cout << "extraBf: " << endl;
		fout << "extraBf: " << endl;
		fout << "k = " << extraBF->Get_bf_k() << endl;
		fout << "m = " << extraBF->Get_bf_m() << endl;
		fout << "n = " << extraBF->Get_bf_n() << endl;
	}

	cout << "in CBF total_num: " << total_num << " bit_array count: " << bit_array.count() << " bitarray_num " << bitarray_num << " bf_sum " << bf_sum << endl;
	fout << "in CBF total_num: " << total_num << " bit_array count: " << bit_array.count() << " bitarray_num " << bitarray_num << " bf_sum " << bf_sum << endl;

	for (int i = 0; i < 16; i++)
	{
		cout << "level " << i + 1 << " push to bitarray num : " << num_of_level_push_to_bitarray[i] << endl;
		fout << "level " << i + 1 << " push to bitarray num : " << num_of_level_push_to_bitarray[i] << endl;
	}
	//bf_query(a_BloomFilter);

	fout.close();

	return 0;
}



int bloom_filter(char *filename)
{

//	clock_t start, end;
//	start = clock(); //开始时，取得开始时间。
	//time_t t_s;
	//time(&t_s);
	int x = 0, j = 0;
	//unsigned char prefix[PREFIX_NUM][PREFIX_LEN];
	unsigned char prefix[TEST_PREFIX_LEN];


	unsigned int k = 10;
	unsigned int m = (int)(k * PREFIX_NUM / log(2));
	QWStandardBF *sBF1 = new QWStandardBF();


	ifstream fin(filename);
	
	char str[LINE_LEN];
	char sPrefix[20];
	while (!fin.eof())
	{

		memset(sPrefix, 0, sizeof(sPrefix));

		fin >> sPrefix >> str;
		int iStart = 0;
		int iEnd = 0;
		int iLen = strlen(sPrefix);
		int prefix_b = 0;
		for (int i = 0; i<iLen; i++){

			if (sPrefix[i] == '.')
			{
				iEnd = i;
			    string strVal(sPrefix + iStart, iEnd - iStart);
				prefix[prefix_b] = (char)atoi(strVal.c_str());
			//	cout << (int)prefix[prefix_b] << ".";
				iStart = i + 1;
				i++;
				prefix_b++;
			}///if "."
			if (sPrefix[i] == '/')
			{

				iEnd = i;
				string strVal(sPrefix + iStart, iEnd - iStart);
				prefix[prefix_b] = (char)atoi(strVal.c_str());
				//string strtmp = sPrefix.substr(iStart, iEnd - iStart);
				//prefix[prefix_b] = (char)atoi(strtmp.c_str());
			//	cout <<(int)prefix[prefix_b] << "/";
				iStart = i + 1;
				i++;
				prefix_b++;

				strVal = string(sPrefix + iStart);
				prefix[prefix_b] = (char)atoi(strVal.c_str());
			//	cout << (int)prefix[prefix_b] << "  "<<str<<endl;
				sBF1->insert(prefix, TEST_PREFIX_LEN);
			}
		}//for

				
	}//while
	fin.close();


//	time(&t_e);
//	cout << "The time of constructing Bloom Filter " <<t_s << " " << t_e << "  "<<t_e - t_s << endl;
	
//	end = clock();   //结束时，取得结束时间
//	cout << "Bloom Filter time " <<start <<" " <<end<<" "<< ((end - start)/CLOCKS_PER_SEC)<< endl;
	//printf("Run time: %lf S", (double)(end - start) / CLOCKS_PER_SEC);
	//插入prefix_num个元素，4字节



	sBF1->outputOHABF("sbf1.bf");
	cout << "k = " << sBF1->Get_bf_k() << endl;
	cout << "m = " << sBF1->Get_bf_m() << endl;
	cout << "n = " << sBF1->Get_bf_n() << endl;


	uchar p[TEST_PREFIX_LEN];

	p[0] = 1;
	p[1] = 0;
	p[2] = 6;
	p[3] = 0;
	
	cout << (int)p[0] << (int)p[1] << (int)p[2] << (int)p[3] << endl;
	if (sBF1->query(p, TEST_PREFIX_LEN))
		cout << "the prefix is found.\n";
	else
		cout << "the prefix is not found.\n";

	//test_bf_query(sBF1);

	return 0;
}

/*
///测试用MegeredTrie 建bf是否正确
int bf_query_test(char *filename,  EXSBF *extraBF)
{
	cout << " constructing the bloom_filter ...." << endl;
	//	clock_t start, end;
	//	start = clock(); //开始时，取得开始时间。
	//time_t t_s, t_e;
	//time(&t_s);
	int x = 0, j = 0;
	//unsigned char prefix[PREFIX_NUM][PREFIX_LEN];
	unsigned char prefix[PREFIX_LEN];
	unsigned int k = kk;
	unsigned int m;
	unsigned int num_of_level_push_to_bitarray[16];
	memset(num_of_level_push_to_bitarray, 0, sizeof(num_of_level_push_to_bitarray));

	ofstream fout;
	if (extraBF == NULL)
	{
		fout.open("result_bf_qurey.txt", ios::app);
	}
	else
	{
		fout.open("result_extrabf_qurey.txt", ios::app);
	}

	if (fout.fail())
	{
		cout << "open outputfile failed.. " << endl;
		return -1;
	}

	//unsigned char *bit_array = bitarray;
	unsigned int extra_n = 0;
	
	cout << "extra_n :" << extra_n << endl;
	fout << "extra_n :" << extra_n << endl;
	if (extraBF != NULL)
	{
		k = 16; //extraBF  k is less than BF,so that the extrabf is different from bf at the hash maybe same,rather the m is not,mapping algorithm is different
		m = (int)((double(extra_n) / log(2)) + 0.5); // 
		extraBF->initial(m, k);
	}

	ifstream fin(filename);//filename=MergedTrie

	char str[LINE_LEN];
	char sPrefix[20];
	int level;

	unsigned int total_num = 0;
	unsigned int bitarray_num = 0;
	int insertcount = 0;
	while (!fin.eof())
	{
		total_num++;
		memset(sPrefix, 0, sizeof(sPrefix));
		memset(prefix, 0, sizeof(prefix));

		fin >> sPrefix >> str;


		//	routerSet.insert(sPrefix);
		int iStart = 0;
		int iEnd = 0;
		int iLen = strlen(sPrefix);
		int prefix_b = 0;

		for (int i = 0; i<iLen; i++)
		{

			if (sPrefix[i] == '.')
			{
				iEnd = i;
				string strVal(sPrefix + iStart, iEnd - iStart);
				prefix[prefix_b] = (unsigned char)atoi(strVal.c_str());
				//	cout << (int)prefix[prefix_b] << "."<< endl;
				iStart = i + 1;
				i++;
				prefix_b++;
			}///if "."
			if (sPrefix[i] == '/')
			{

				iEnd = i;
				string strVal(sPrefix + iStart, iEnd - iStart);
				prefix[prefix_b] = (char)atoi(strVal.c_str());
				//string strtmp = sPrefix.substr(iStart, iEnd - iStart);
				//prefix[prefix_b] = (char)atoi(strtmp.c_str());
				//	cout << (int)prefix[prefix_b] << "/";
				iStart = i + 1;
				i++;
				prefix_b++;

				strVal = string(sPrefix + iStart);
				level = atoi(strVal.c_str());

				if (level <= 16) // insert to bitarray
				{
					int insert_num_area = pow(2.0, (16 - level));
					num_of_level_push_to_bitarray[level - 1] += insert_num_area;
					unsigned int value = 0;
					value += (int)prefix[1];
					value += prefix[0] << 8;

					for (int i = 0; i < insert_num_area; i++)
					{

						bit_array[value + i] = 1;
						bitarray_num++;
					}

					// 65535/8+1
					// bit_array[value%8192] |= 128>>(-7value%8);
					//	cout <<"bit array: "<< (int)bit_array[1] << ends<<(int)bit_array[0]<<endl;

					//			cout << "value " << ends;
					//cout <<bit_array<< endl;
					//Sleep(5000);
				}
				else
				{
					prefix[prefix_b] = (char)level;
					//	cout << (int)prefix[prefix_b] << " level " << level << " " << str << endl;

					
				
						extraBF->insert(prefix, PREFIX_LEN);
				

					//a_BloomFilter[level]->outputOHABF("sbf1.bf");
				}
			}
		}//for

	}//while
	fin.close();

	unsigned int bf_sum = 0;
	
	if (extraBF != NULL)
	{
		cout << endl;
		cout << "extraBf: " << endl;
		fout << "extraBf: " << endl;
		fout << "k = " << extraBF->Get_bf_k() << endl;
		fout << "m = " << extraBF->Get_bf_m() << endl;
		fout << "n = " << extraBF->Get_bf_n() << endl;
	}

	cout << "in CBF total_num: " << total_num << " bit_array count: " << bit_array.count() << " bitarray_num " << bitarray_num << " bf_sum " << bf_sum << endl;
	fout << "in CBF total_num: " << total_num << " bit_array count: " << bit_array.count() << " bitarray_num " << bitarray_num << " bf_sum " << bf_sum << endl;

	
	

	fout.close();

	return 0;
}
*/




