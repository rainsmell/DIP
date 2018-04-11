#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct huffNode
{
	int parent, lchild, rchild;
	unsigned long count;
	unsigned char c;
	char bits[256];
}HuffNode;

#define N_Node 512

class HuffCode
{
public:
	HuffCode() : m_fpInFile(NULL), m_fpOutFile(NULL) {}
	~HuffCode() { xInit_Tree(); }

	void SetInputFile(FILE* in) { m_fpInFile = in; }
	void SetOutputFile(FILE* out) { m_fpOutFile = out; }
	double GetCodeRate() { return double(m_nCompressFileLen) / (double)(m_nRawFileLen); }

	void BuildTree();

	int EncodeFile();
	int DecodeFile();

	double GetRawFileEntropy();
	double GetAverCodeLen();

public:
	huffNode m_HTree[N_Node];

private:
	void xInit_Tree();

private:
	FILE* m_fpInFile;
	FILE* m_fpOutFile;
	unsigned int m_nRawFileLen;
	unsigned int m_nCompressFileLen;
	unsigned int m_nMaxCodeLen;
	unsigned int m_nUsedChar;
};
