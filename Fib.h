#ifndef FIB_H_
#define FIB_H_

#define	FIBLEN			sizeof(struct TrieNode)		
#define VR_NUM_MAX		12
#define HIGHTBIT		2147483648								
#define Zero(str)		memset(str,0,sizeof(str))

#include	<string>
#include	<fstream>
using namespace std;

struct TrieNode
{
	TrieNode*			parent;				
	TrieNode*			lchild;				
	TrieNode*			rchild;				
	int					newPort;			
	int					oldPort;		//for FIB update purpose. record the original next-hop				
	int					portList[VR_NUM_MAX];	
};

class CFib
{

public:

	TrieNode * m_pTrie;				//Root TrieNode

	int routerNum;           //number of virtual routers supported

	int allNodeCount;		 //number of all nodes
	int solidNodeCount;		 //number of prefix nodes
	int nonRouteNum;		//number of no next-hop nodes(i.e. next-hop is -1)
	int oldNodeCount;		//number of prefixes in the original RIB		
	int leafNodeCount;      //number of leaf nodes

	int numofDepth[40];		//numofDepth[i] is the number of prefix nodes in depth i

	CFib(void);
	~CFib(void);

	void CFib::FreeTrie(TrieNode* pTrie);

	void CreateNewNode(TrieNode* &pTrie);

	unsigned int CFib::outline(CFib * tFib2);

	void CFib::leafpushing(TrieNode* pTrie);

	void CFib::overlying(TrieNode *trie1, TrieNode *trie2);	

	void CFib::pushToOutline(CFib * tFib2,int number);

	void CFib::sub_pushToOutline(TrieNode *trie1, TrieNode *trie2, int number);

	void CFib::CoverAllLeaves(TrieNode * pNode, int number, int coverPort);

	void GetNodeCounts();
	void Pretraversal(TrieNode* pTrie,int depth);

	void CFib::OutputAlltables(TrieNode* pTrie);

	void CFib::OutputMergedTrie(string filename,  bool ifonlyPort);	
	void CFib::OutputMergedTrie2(string filename,bool ifonlyPort);

	void CFib::sub_OutputMergedTrie(TrieNode* pTrie,unsigned int iVal,int iBitLen,ofstream* fout,bool ifonlyPort);
	void CFib::sub_OutputMergedTrie2(TrieNode* pTrie, unsigned int iVal, int iBitLen,ofstream* fout, bool ifonlyPort);

	//void CFib::OutputTrie(string filename);

	//void CFib::sub_OutputTrie(TrieNode* pTrie,unsigned int iVal,int iBitLen,ofstream* fout);

	bool IsLeaf(TrieNode * pNode);


private:

	unsigned int GetAncestorHop(TrieNode* pTrie);

	void CFib::GetTrieHops(TrieNode* pTrie,unsigned int iVal,int iBitLen,ofstream* fout,int number);


public:

	unsigned int ConvertBinToIP(string sBinFile,string sIpFile);

	unsigned int ConvertIpToBin(string sIpFile,string sBinFile);

	void IpToBinary(string sIP,char sBin[32]);

	unsigned int BuildFibFromFile(string sFileName);

	void AddNode(unsigned long lPrefix,unsigned int iPrefixLen,unsigned int iNextHop);
};

#endif /* FIB_H_ */
