#include "Fib.h"
#include <iostream>
#include <windows.h>	
#include <time.h>
#include "bloom_filter.h"
using namespace std;

bool	bIpFormat=true;						
#define LINESIZE	66
#define BF_NUM 32
string DupString(char cElement,int iLen,bool bNewLine){
	string strRet;
	for (int i=1;i<=iLen;i++)
	{
		if(i==iLen && bNewLine==true)
			strRet += "\n";
		else
			strRet += cElement;
	}
	return strRet;
}

bool Exist(char * FileName)
{
	FILE*   fp;
	if((fp=fopen(FileName, "r "))==NULL)
	{
		return false;
	}
	else
	{
		fclose(fp);
		return true;
	}
}

void ShowTipInfo()
{

	printf("                     Trie Merging Algorithm\n");
	printf("%s",DupString('=',LINESIZE,true).c_str());
	printf("RIB Format  Binary(B) or IP(I):");

	char cResponse=getchar();
	//printf("%c",cResponse);
	while(cResponse!='B'||cResponse!='b'||cResponse!='I'||cResponse!='i')
	{
		if(cResponse=='B'||cResponse=='b')
		{
			bIpFormat=false;
			break;
		}
		if(cResponse=='I'||cResponse=='i')
		{
			break;
		}

		cResponse=getchar();
		//printf("%c",cResponse);
	}
}

int TrieMerging(CFib *pFib1,char *ribFile2)
{
	char *ribFile2_IP="ribFile2_IP.txt";       

	CFib tFib2= CFib();	

	unsigned int iEntryCount1 = 0;
	unsigned int iEntryCount2 = 0;

	if(bIpFormat)
	{
		iEntryCount2=tFib2.BuildFibFromFile(ribFile2);
	}
	else
	{
		tFib2.ConvertBinToIP(ribFile2,ribFile2_IP);
		iEntryCount2=tFib2.BuildFibFromFile(ribFile2_IP);
	}

	tFib2.GetNodeCounts();

	FILE *fp=fopen("result.txt","a+");
	printf("FIB %s: solid nodes=\t%d\tleaf nodes=\t%d\tall nodes=\t%d\n",ribFile2,tFib2.solidNodeCount,tFib2.leafNodeCount,tFib2.allNodeCount);
	fprintf(fp,"FIB %s: solid nodes=\t%d\tleaf nodes=\t%d\tall nodes=\t%d\n",ribFile2,tFib2.solidNodeCount,tFib2.leafNodeCount,tFib2.allNodeCount);

	pFib1->outline(&tFib2);  //get the profile of the merged trie

	tFib2.FreeTrie(tFib2.m_pTrie);	

	pFib1->GetNodeCounts();

	printf("The merged trie: solid nodes=\t%d\tleaf nodes=\t%d\tall nodes=\t%d\n\n",pFib1->solidNodeCount,pFib1->leafNodeCount,pFib1->allNodeCount);
	fprintf(fp,"The merged trie: solid nodes=\t%d\tleaf nodes=\t%d\tall nodes=\t%d\n\n",pFib1->solidNodeCount,pFib1->leafNodeCount,pFib1->allNodeCount);

	//printf("Fib %s =\t%d\t After trie merging,prefix nodes=\t%u\tallNodeCount=\t%d\n",ribFile2,tFib2.solidNodeCount,pFib1->solidNodeCount,pFib1->allNodeCount);
	//fprintf(fp,"Fib %s =\t%d\t After trie merging,prefix nodes=\t%u\tallNodeCount=\t%d\n",ribFile2,tFib2.solidNodeCount,pFib1->solidNodeCount,pFib1->allNodeCount);

	fclose(fp);

	return 0;
}

void pushToMergedTrie(CFib *pFib1,char *ribFile2,int number)
{
	printf("pushing %s...\n",ribFile2);
	char *ribFile2_IP="ribFile2_IP.txt";
	CFib tFib2= CFib();	

	unsigned int iEntryCount1 = 0;
	unsigned int iEntryCount2 = 0;

	if(bIpFormat)
	{
		iEntryCount2=tFib2.BuildFibFromFile(ribFile2);
	}
	else
	{
		tFib2.ConvertBinToIP(ribFile2,ribFile2_IP);
		iEntryCount2=tFib2.BuildFibFromFile(ribFile2_IP);
	}

	pFib1->pushToOutline(&tFib2,number);  //number starts from 1
	tFib2.FreeTrie(tFib2.m_pTrie);
}

int _main(int argc, char * argv[])
{
	time_t t_s_main,t_e_mergedtrie,t_e_bf,t_e_bf_query;
	time(&t_s_main);

	CFib MergedFib= CFib();	

	//ShowTipInfo();
	//printf("The size of TrieNode is %d bytes\n",sizeof(TrieNode));

	//system("dir /B /ON rrc*.txt > fileList");

	FILE * fList=fopen("fileList","r");
	while (!feof(fList)) 
	{
		char filename[200];
		memset(filename,0,sizeof(filename));
		fscanf(fList,"%s",filename);

		if(strlen(filename)>3)
			TrieMerging(&MergedFib,filename);
	}
	fclose(fList);

	int number=0;   //the number of RIBs
	fList=fopen("fileList","r");
	while (!feof(fList)) 
	{
		number++;
		char filename[200];
		memset(filename,0,sizeof(filename));
		fscanf(fList,"%s",filename);

		if(strlen(filename)>3)
			pushToMergedTrie(&MergedFib,filename,number);
	}
	fclose(fList);

	number--;  

	MergedFib.routerNum=number;

	MergedFib.OutputAlltables(MergedFib.m_pTrie);

	MergedFib.OutputMergedTrie("MergedTrie(only_port).txt",true);
	MergedFib.OutputMergedTrie("MergedTrie.txt",false);
	//MergedFib.OutputMergedTrie2("MergedTrie.ip", false);
	MergedFib.GetNodeCounts();

	
	time(&t_e_mergedtrie);
	StandardBF *(a_BloomFilter[33]);
	printf("The final merged trie:\nsolid nodes=\t%d\tleaf nodes=\t%d\tall nodes=\t%d\n",MergedFib.solidNodeCount,MergedFib.leafNodeCount,MergedFib.allNodeCount);
	char *filename = "MergedTrie.txt";
	for (int i=0;i<33;i++)
	{
		printf("Level %d    Prefix nodes = %d\n",i,MergedFib.numofDepth[i]);
		a_BloomFilter[i] = new StandardBF();

	}
	int bf_num = 33;
    construct_bloom_filter(filename,a_BloomFilter,MergedFib.numofDepth,33);

	time(&t_e_bf);
	bf_query(a_BloomFilter);
	
	time(&t_e_bf_query);
	printf("\nmission complete!\n");
	
	cout << "####Time MergedTrie  " << t_e_mergedtrie - t_s_main << " Construct BF: " << t_e_bf - t_e_mergedtrie << " BF_query one file: " << t_e_bf_query - t_e_bf << "total time :" << t_e_bf_query -t_s_main << endl;
	
	for (int i = 0; i<33; i++)
	{		
		delete a_BloomFilter[i];
	}
	system("pause");
	return 0;
}