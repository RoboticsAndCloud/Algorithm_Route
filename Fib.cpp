#include "Fib.h"
#include <iostream>

#define NONROUTE_PORT -1	//the default next-hop port of root node
#define OVERLYING_PORT 66    //构建Merged trie时使用，最后merged trie的叶节点newPort取此值
#define EMPTYHOP	0		//no next hop information			

CFib::CFib(void)
{

	CreateNewNode(m_pTrie);

	allNodeCount=0;			
	solidNodeCount=0;		
	nonRouteNum=0;
	leafNodeCount=0;
	routerNum=0;
	Zero(numofDepth);
}

CFib::~CFib(void)
{
}

void CFib::FreeTrie(TrieNode* pTrie)
{
	if (NULL==pTrie)return;

	FreeTrie(pTrie->lchild);
	FreeTrie(pTrie->rchild);

	if (pTrie->lchild!=NULL)
	{
		if (IsLeaf(pTrie->lchild))
		{
			free(pTrie->lchild);
			pTrie->lchild=NULL;
		}
	}

	if (pTrie->rchild!=NULL)
	{
		if (IsLeaf(pTrie->rchild))
		{
			free(pTrie->rchild);
			pTrie->rchild=NULL;
		}
	}
}

void CFib::CreateNewNode(TrieNode* &pTrie)
{
	pTrie= (struct TrieNode*)malloc(FIBLEN);

	if (NULL==pTrie)
	{
		printf("malllll 444 failed\n");
		return;
	}

	pTrie->parent = NULL;
	pTrie->lchild = NULL;
	pTrie->rchild = NULL;
	pTrie->newPort = EMPTYHOP;
	pTrie->oldPort = EMPTYHOP;
	Zero(pTrie->portList);
}


bool CFib::IsLeaf(TrieNode * pNode)
{
	if (pNode->lchild==NULL && pNode->rchild==NULL)return true;
	else return false;	
}


unsigned int CFib::GetAncestorHop(TrieNode* pTrie)
{
	unsigned int iHop = EMPTYHOP;
	if(pTrie != NULL)
	{
		pTrie=pTrie->parent;
		if(pTrie!=NULL)
		{
			iHop = pTrie->newPort;
			if(iHop==EMPTYHOP)
				iHop=GetAncestorHop(pTrie);
		}
	}
	return iHop;
}


void CFib::Pretraversal(TrieNode* pTrie,int depth)
{
	if (NULL==pTrie)return;

	allNodeCount++;
	if (pTrie->newPort!=0)
	{
		solidNodeCount++;
		numofDepth[depth]++;
	}
	if (-1==pTrie->newPort)nonRouteNum++;
	if (pTrie->oldPort!=0)oldNodeCount++;	
	if (IsLeaf(pTrie))leafNodeCount++;

	Pretraversal(pTrie->lchild,depth+1);
	Pretraversal(pTrie->rchild,depth+1);
}

void CFib::GetNodeCounts()
{
	Zero(numofDepth);

	allNodeCount=0;
	solidNodeCount=0;
	nonRouteNum=0;
	oldNodeCount=0;
	leafNodeCount=0;

	Pretraversal(m_pTrie,0);
}

//输出根据merged trie生成的各子FIB
void CFib::OutputAlltables(TrieNode* pTrie)  
{
	int i=1;
	FILE * fList=fopen("fileList","r");
	while (!feof(fList)) 
	{
		char filename[200];
		memset(filename,0,sizeof(filename));
		fscanf(fList,"%s",filename);

		if (strlen(filename)<4)continue;

		sprintf_s(filename,"%s.vm",filename);

		ofstream fout1(filename);
		GetTrieHops(pTrie,0,0,&fout1,i);  //output the ith FIB from the merged trie
		fout1<<flush;
		fout1.close();

		i++;
	}
	fclose(fList);
}



void CFib::OutputMergedTrie(string filename,bool ifonlyPort)
{
	ofstream fout(filename.c_str());
	sub_OutputMergedTrie(m_pTrie,0,0,&fout,ifonlyPort);

	fout<<flush;
	fout.close();
}

void CFib::OutputMergedTrie2(string filename, bool ifonlyPort)
{
	cout << "MergedTrie.ip is creating..." << endl;
	ofstream fout(filename.c_str());
	sub_OutputMergedTrie2(m_pTrie,0,0,&fout,ifonlyPort);

	fout << flush;
	fout.close();
}

void CFib::sub_OutputMergedTrie2(TrieNode* pTrie, unsigned int iVal, int iBitLen, ofstream* fout, bool ifonlyPort)
{
	if (NULL == pTrie)return;

	if (IsLeaf(pTrie))
	{
		if (false == ifonlyPort)
		{
			char strVal[50];
			memset(strVal, 0, sizeof(strVal));
			sprintf(strVal, "%d %d %d %d %d\t", (iVal >> 24), (iVal << 8) >> 24, (iVal << 16) >> 24, (iVal << 24) >> 24, iBitLen);

			*fout << strVal;
		}
		int i;
		for (i = 0; i<routerNum-1; i++)
		{
			*fout << pTrie->portList[i + 1] << ',';
		}
		*fout << pTrie->portList[i + 1] ;
		*fout << endl;
	}

	iBitLen++;


	if (pTrie->lchild != NULL)
	{
		sub_OutputMergedTrie2(pTrie->lchild, iVal, iBitLen, fout, ifonlyPort);
	}

	if (pTrie->rchild != NULL)
	{
		iVal += 1 << (32 - iBitLen);
		sub_OutputMergedTrie2(pTrie->rchild, iVal, iBitLen, fout, ifonlyPort);
	}

}

void CFib::sub_OutputMergedTrie(TrieNode* pTrie,unsigned int iVal,int iBitLen,ofstream* fout,bool ifonlyPort)
{
	if (NULL==pTrie)return;

	if (IsLeaf(pTrie))
	{
		if (false==ifonlyPort)
		{
			char strVal[50];
			memset(strVal,0,sizeof(strVal));
			sprintf(strVal,"%d.%d.%d.%d/%d\t",(iVal>>24),(iVal<<8)>>24,(iVal<<16)>>24,(iVal<<24)>>24,iBitLen);

			*fout<<strVal;
		}
		int i;
		for ( i=0;i<routerNum-1;i++)
		{
			*fout<<pTrie->portList[i+1]<<',';
		}
		*fout << pTrie->portList[i + 1];
		*fout<<endl;
	}

	iBitLen++;


	if(pTrie->lchild!=NULL)
	{
		sub_OutputMergedTrie(pTrie->lchild,iVal,iBitLen,fout,ifonlyPort);
	}

	if(pTrie->rchild!=NULL)
	{
		iVal += 1<<(32-iBitLen);
		sub_OutputMergedTrie(pTrie->rchild,iVal,iBitLen,fout,ifonlyPort);
	}

}


void CFib::GetTrieHops(TrieNode* pTrie,unsigned int iVal,int iBitLen,ofstream* fout,int number)
{
	if (NULL==pTrie)return;

	if (IsLeaf(pTrie))
	{
		char strVal[50];
		memset(strVal,0,sizeof(strVal));
		sprintf(strVal,"%d.%d.%d.%d/%d\t%d\n",(iVal>>24),(iVal<<8)>>24,(iVal<<16)>>24,(iVal<<24)>>24,iBitLen,pTrie->portList[number]);
		*fout<<strVal;
	}

	iBitLen++;


	if(pTrie->lchild!=NULL)
	{
		GetTrieHops(pTrie->lchild,iVal,iBitLen,fout,number);
	}

	if(pTrie->rchild!=NULL)
	{
		iVal += 1<<(32-iBitLen);
		GetTrieHops(pTrie->rchild,iVal,iBitLen,fout,number);
	}
}

unsigned int CFib::ConvertBinToIP(string sBinFile,string sIpFile)
{
	char			sBinPrefix[32];		
	string			strIpPrefix;		
	unsigned int	iPrefixLen;			
	unsigned int	iNextHop;			
	unsigned int	iEntryCount=0;		

	ofstream fout(sIpFile.c_str());

	ifstream fin(sBinFile.c_str());
	while (!fin.eof()) 
	{
		iNextHop = 0;

		memset(sBinPrefix,0,sizeof(sBinPrefix));
		fin >> sBinPrefix>> iNextHop;

		if(iNextHop != 0)
		{
			string strBin(sBinPrefix);

			if(strBin == "*")
				iPrefixLen=0;
			else
				iPrefixLen=strBin.length();

			strBin.append(32-iPrefixLen,'0');

			strIpPrefix="";
			for(int i=0; i<32; i+=8)
			{				
				int iVal=0;
				string strVal=strBin.substr(i,8);

				for(int j=7;j>=0;j--)
				{
					if(strVal.substr(j,1)=="1")
						iVal+=(1<<(7-j));
				}

				char buffer[5];
				memset(buffer,0,sizeof(buffer));
				sprintf(buffer,"%d",iVal);
				//itoa(iVal,buffer,10);
				strVal=string(buffer);

				strIpPrefix += strVal;
				if(i<24)
				{
					strIpPrefix += ".";
				}
				strVal="";
			}

			fout<<strIpPrefix<<"/"<<iPrefixLen<<" "<<iNextHop<<endl;
		}
	}

	fin.close();
	fout<<flush;
	fout.close();

	return iEntryCount;
}


unsigned int CFib::ConvertIpToBin(string sIpFile,string sBinFile)
{
	char			sBinPrefix[32];		
	string			strIpPrefix;		
	unsigned int	iPrefixLen;			
	unsigned int	iNextHop;		
	unsigned int	iEntryCount=0;		

	char			sPrefix[20];		

	ofstream fout(sBinFile.c_str());

	ifstream fin(sIpFile.c_str());
	while (!fin.eof()) 
	{	
		iPrefixLen = 0;
		iNextHop = EMPTYHOP;

		memset(sPrefix,0,sizeof(sPrefix));	
		fin >> sPrefix>> iNextHop;

		int iLen=strlen(sPrefix);	

		if(iLen>0)
		{
			iEntryCount++;
			for ( int i=0; i<iLen; i++ )
			{
				if ( sPrefix[i] == '/' )
				{				
					string strVal(sPrefix,i);
					strIpPrefix=strVal;

					strVal= string(sPrefix+i+1,iLen-1);
					iPrefixLen=atoi(strVal.c_str());
					break;
				}
			}

			memset(sBinPrefix,0,sizeof(sBinPrefix));

			IpToBinary(strIpPrefix,sBinPrefix);

			if(iPrefixLen>0)
			{
				strIpPrefix=string(sBinPrefix,iPrefixLen);
			}
			else
			{
				strIpPrefix="*";
			}

			fout<<strIpPrefix<<"\t"<<iNextHop<<endl;
		}
	}

	fin.close();

	fout<<flush;
	fout.close();

	return iEntryCount;
}


void CFib::IpToBinary(string sIP,char saBin[32]){
	int iStart=0;				
	int iEnd=0;					
	int iFieldIndex = 3;		
	int iLen=sIP.length();		
	unsigned long	lPrefix=0;	


	for ( int i=0; i<iLen; i++ ){

		if ( sIP.substr(i,1)== "." ){
			iEnd = i;
			string strVal=sIP.substr(iStart,iEnd-iStart);
			lPrefix += atol(strVal.c_str()) << (8 * iFieldIndex); 
			iFieldIndex--;
			iStart = i+1;
			i++;
		}
		if ( iFieldIndex == 0 ){

			iEnd = iLen;
			string strVal=sIP.substr(iStart,iEnd-iStart);
			lPrefix += atol(strVal.c_str());
			iStart = i+1;
		}
	}


	unsigned long	lVal=0x80000000;
	for(int i=0;i<32;i++){
		if(lPrefix&lVal){
			saBin[i]='1';
		}
		else{
			saBin[i]='0';
		}
		lVal=lVal>>1;
	}
}


void CFib::AddNode(unsigned long lPrefix,unsigned int iPrefixLen,unsigned int iNextHop)
{

	TrieNode* pTrie = m_pTrie;
	for (unsigned int i=0; i<iPrefixLen; i++){

		if(((lPrefix<<i) & HIGHTBIT)==HIGHTBIT){

			if(pTrie->rchild == NULL){
				TrieNode* pLTChild = (struct TrieNode*)malloc(FIBLEN);

				if (NULL==pLTChild)
				{
					printf("malllll 555 failed\n");
					return;
				}

				pLTChild->parent = pTrie;
				pLTChild->lchild = NULL;
				pLTChild->rchild = NULL;
				pLTChild->oldPort=0;
				pLTChild->newPort=0;

				Zero(pLTChild->portList);

				pTrie->rchild = pLTChild;
			}

			pTrie = pTrie->rchild;

		}

		else{

			if(pTrie->lchild == NULL){
				TrieNode* pTChild = (struct TrieNode*)malloc(FIBLEN);

				if (NULL==pTChild)
				{
					printf("malllll 666 failed\n");
					return;
				}


				pTChild->parent = pTrie;
				pTChild->lchild = NULL;
				pTChild->rchild = NULL;
				pTChild->oldPort=0;
				pTChild->newPort=0;

				Zero(pTChild->portList);

				pTrie->lchild = pTChild;
			}

			pTrie = pTrie->lchild;
		}
	}

	pTrie->newPort = iNextHop;
	pTrie->oldPort = iNextHop;
}


unsigned int CFib::BuildFibFromFile(string sFileName)
{
	unsigned int	iEntryCount=0;		

	char			sPrefix[20];	
	unsigned long	lPrefix;		
	unsigned int	iPrefixLen;		
	unsigned int	iNextHop;	


	ifstream fin(sFileName.c_str());
	while (!fin.eof()) {


		lPrefix = 0;
		iPrefixLen = 0;
		iNextHop = EMPTYHOP;

		memset(sPrefix,0,sizeof(sPrefix));

		fin >> sPrefix>> iNextHop;

		int iStart=0;			
		int iEnd=0;				
		int iFieldIndex = 3;		
		int iLen=strlen(sPrefix);	


		if(iLen>0){
			iEntryCount++;
			for ( int i=0; i<iLen; i++ ){

				if ( sPrefix[i] == '.' ){
					iEnd = i;
					string strVal(sPrefix+iStart,iEnd-iStart);
					lPrefix += atol(strVal.c_str()) << (8 * iFieldIndex); 
					iFieldIndex--;
					iStart = i+1;
					i++;
				}
				if ( sPrefix[i] == '/' ){

					iEnd = i;
					string strVal(sPrefix+iStart,iEnd-iStart);
					lPrefix += atol(strVal.c_str());
					iStart = i+1;


					i++;
					strVal= string(sPrefix+iStart,iLen-1);
					iPrefixLen=atoi(strVal.c_str());
				}
			}

			AddNode(lPrefix,iPrefixLen,iNextHop);
		}
	}


	fin.close();
	return iEntryCount;
}

//leaf push a trie
void CFib::leafpushing(TrieNode* pTrie)
{

	if (NULL==pTrie)return;		

	if (NULL==pTrie->lchild && NULL==pTrie->rchild)return;	

	if (0==pTrie->newPort)   //如果是空心节点，跳过；需要把实心节点的下一跳向其左右子树push
	{
		leafpushing(pTrie->lchild);
		leafpushing(pTrie->rchild);
		return;
	}

	if (NULL!=pTrie->lchild && NULL==pTrie->rchild)
	{
		TrieNode* pTRChild = (struct TrieNode*)malloc(FIBLEN);

		if (NULL==pTRChild)
		{
			printf("malloc 222 failed\n");
			return;
		}

		pTRChild->parent  = pTrie;
		pTRChild->lchild  = NULL;
		pTRChild->rchild  = NULL;
		pTRChild->oldPort = 0;
		pTRChild->newPort = pTrie->newPort;
		Zero(pTRChild->portList);
		pTrie->rchild=pTRChild;

		if (0==pTrie->lchild->newPort)
			pTrie->lchild->newPort=pTrie->newPort;

		leafpushing(pTrie->lchild);

		pTrie->newPort=0;
	}

	else if (NULL!=pTrie->rchild && NULL==pTrie->lchild)	
	{
		TrieNode* pTLChild = (struct TrieNode*)malloc(FIBLEN);

		if (NULL==pTLChild)
		{
			printf("malloc 222 failed\n");
			return;
		}

		pTLChild->parent = pTrie;
		pTLChild->lchild = NULL;
		pTLChild->rchild = NULL;
		pTLChild->oldPort=0;
		pTLChild->newPort=pTrie->newPort;
		Zero(pTLChild->portList);
		pTrie->lchild=pTLChild;

		if (0==pTrie->rchild->newPort)
			pTrie->rchild->newPort=pTrie->newPort;

		leafpushing(pTrie->rchild);

		pTrie->newPort=0;
	}

	else			//two children nodes
	{
		if (0==pTrie->rchild->newPort)pTrie->rchild->newPort=pTrie->newPort;
		if (0==pTrie->lchild->newPort)pTrie->lchild->newPort=pTrie->newPort;

		leafpushing(pTrie->lchild);
		leafpushing(pTrie->rchild);

		pTrie->newPort=0;
	}
}

// outline algorithm: leaf push two tries, then merge them. Then merged trie has the outline of the two original tries.
unsigned int CFib::outline(CFib * tFib2)
{

	if(0==m_pTrie->oldPort)   //if there is no default route, set the nexthop of default route to -1
	{
		this->m_pTrie->oldPort=NONROUTE_PORT;
		this->m_pTrie->newPort=NONROUTE_PORT;
	}

	if (0==tFib2->m_pTrie->oldPort)
	{
		tFib2->m_pTrie->oldPort=NONROUTE_PORT;
		tFib2->m_pTrie->newPort=NONROUTE_PORT;
	}

	//leafpushing(this->m_pTrie);  
	//leafpushing(tFib2->m_pTrie);  
	overlying(this->m_pTrie,tFib2->m_pTrie);

	return 1;
}


void CFib::overlying(TrieNode *trie1, TrieNode *trie2)
{
	if (NULL==trie1 || NULL==trie2)return;


	if (IsLeaf(trie1) && false==IsLeaf(trie2))
	{
		TrieNode* pNewLeftNode;
		CreateNewNode(pNewLeftNode);
		pNewLeftNode->parent=trie1;
		pNewLeftNode->newPort=OVERLYING_PORT;
		trie1->lchild=pNewLeftNode;

		TrieNode* pNewRightNode;
		CreateNewNode(pNewRightNode);
		pNewRightNode->parent=trie1;
		pNewRightNode->newPort=OVERLYING_PORT;
		trie1->rchild=pNewRightNode;

		trie1->newPort=0;   //leaf push
	}

	overlying(trie1->lchild, trie2->lchild);
	overlying(trie1->rchild, trie2->rchild);
}


void CFib::pushToOutline(CFib * tFib2,int number)
{
	if (0==tFib2->m_pTrie->oldPort)
	{
		tFib2->m_pTrie->oldPort=NONROUTE_PORT;  //set default route
		tFib2->m_pTrie->newPort=NONROUTE_PORT;
	}

	leafpushing(tFib2->m_pTrie);
	//tFib2->OutputTrie("rib0_leafpush.txt");

	tFib2->GetNodeCounts();
	printf("Fib %d after leaf pushing: solid nodes=\t%d\tleaf nodes=\t%d\tall nodes=\t%d\n\n",number,tFib2->solidNodeCount,tFib2->leafNodeCount,tFib2->allNodeCount);

	sub_pushToOutline(this->m_pTrie,tFib2->m_pTrie,number);
}

void CFib::sub_pushToOutline(TrieNode *trie1, TrieNode *trie2, int number)
{
	if (NULL==trie1 || NULL==trie2)return;
	if (IsLeaf(trie2))
	{
		if (IsLeaf(trie1))
		{
			trie1->portList[number]=trie2->newPort;
		}
		else
		{
			CoverAllLeaves(trie1,number,trie2->newPort);
		}
	}

	sub_pushToOutline(trie1->lchild,trie2->lchild,number);
	sub_pushToOutline(trie1->rchild,trie2->rchild,number);
}

void CFib::CoverAllLeaves(TrieNode * pNode, int number, int coverPort)
{
	if (NULL==pNode)return;

	if (IsLeaf(pNode))
	{
		pNode->portList[number]=coverPort;
	}

	CoverAllLeaves(pNode->lchild,number,coverPort);
	CoverAllLeaves(pNode->rchild,number,coverPort);
}

//void CFib::OutputTrie(string filename)  
//{
//	ofstream fout(filename.c_str());
//	sub_OutputTrie(m_pTrie,0,0,&fout);
//	fout<<flush;
//	fout.close();
//}
//output the corresponding FIB of a leaf pushed trie
//void CFib::sub_OutputTrie(TrieNode* pTrie,unsigned int iVal,int iBitLen,ofstream* fout)
//{
//	if (NULL==pTrie)return;
//
//	if (IsLeaf(pTrie))
//	{
//		char strVal[50];
//		memset(strVal,0,sizeof(strVal));
//		sprintf(strVal,"%d.%d.%d.%d/%d\t",(iVal>>24),(iVal<<8)>>24,(iVal<<16)>>24,(iVal<<24)>>24,iBitLen);
//		*fout<<strVal;
//		*fout<<pTrie->oldPort<<ends<<pTrie->newPort;
//		*fout<<endl;
//	}
//
//	iBitLen++;
//
//	if(pTrie->lchild!=NULL)
//	{
//		sub_OutputTrie(pTrie->lchild,iVal,iBitLen,fout);
//	}
//
//	if(pTrie->rchild!=NULL)
//	{
//		iVal += 1<<(32-iBitLen);
//		sub_OutputTrie(pTrie->rchild,iVal,iBitLen,fout);
//	}
//}
