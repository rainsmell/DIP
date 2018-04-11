#include "stdafx.h"
#include "Huffman.h"

void HuffCode::xInit_Tree()
{
	for (int i = 0; i < N_Node; i++)
	{
		m_HTree[i].parent = -1;
		m_HTree[i].lchild = -1;
		m_HTree[i].rchild = -1;
		m_HTree[i].count = 0;
		m_HTree[i].c = (unsigned char)i;
		memset(m_HTree[i].bits, '\0', 256);
	}
}

void HuffCode::BuildTree()
{
	// 节点初始化
	xInit_Tree();

	// 统计原文件各个字符出现数量
	unsigned char c = 0; // 这里使用unsigned是必须的，扩展ascii包含256个字符，对于8-bit图像也包含255级灰度
	m_nRawFileLen = 0;
	while (!feof(m_fpInFile))
	{
		fread(&c, 1, 1, m_fpInFile);
		m_HTree[c].count++;
		m_nRawFileLen++;
	}
	m_HTree[c].count--;
	m_nRawFileLen--;

	// 按数量从大到小排序
	HuffNode tmp;
	for (int i = 0; i < 255; i++)
	{
		for (int j = i + 1; j < 256; j++)
		{
			if (m_HTree[j].count > m_HTree[i].count)
			{
				tmp = m_HTree[j];
				m_HTree[j] = m_HTree[i];
				m_HTree[i] = tmp;
			}
		}
	}

	// 统计被使用的字符数
	m_nUsedChar = 0;
	for (int i = 0; i < N_Node / 2; i++)
		if (m_HTree[i].count != 0)
			m_nUsedChar++;
		else
			break;
	
	int m = m_nUsedChar * 2 - 1; // 编码树的规模
	m_nUsedChar--; // 从0开始

	// 构建霍夫曼编码树
	unsigned int min_count;
	int min_index;
	for (int i = m_nUsedChar + 1; i < m; i++)
	{
		min_count = UINT_MAX;
		for (int j = 0; j < i; j++)
		{
			if (m_HTree[j].parent != -1)
				continue;
			if (min_count > m_HTree[j].count)
			{
				min_count = m_HTree[j].count;
				min_index = j;
			}
		}
		m_HTree[i].lchild = min_index;
		m_HTree[min_index].parent = i;
		m_HTree[i].count = min_count;

		min_count = UINT_MAX;
		for (int j = 0; j < i; j++)
		{
			if (m_HTree[j].parent != -1)
				continue;
			if (min_count > m_HTree[j].count)
			{
				min_count = m_HTree[j].count;
				min_index = j;
			}
		}
		m_HTree[i].rchild = min_index;
		m_HTree[i].count += min_count;
		m_HTree[min_index].parent = i;
	}

	// 为字符编码
	for (int i = 0; i <= m_nUsedChar; i++)
	{
		int k = i;
		m_HTree[i].bits[0] = 0;

		while (m_HTree[k].parent != -1)
		{
			int j = k;
			k = m_HTree[k].parent;

			if (m_HTree[k].lchild == j)
			{
				j = strlen(m_HTree[i].bits);
				memmove(m_HTree[i].bits + 1, m_HTree[i].bits, j + 1); // 包含'/0'
				m_HTree[i].bits[0] = '0';
			}
			else
			{
				j = strlen(m_HTree[i].bits);
				memmove(m_HTree[i].bits + 1, m_HTree[i].bits, j + 1); // 包含'/0'
				m_HTree[i].bits[0] = '1';
			}
		}
	}

	m_nMaxCodeLen = 0;
	for (int i = 0; i <= m_nUsedChar; i++)
		if (m_nMaxCodeLen < strlen(m_HTree[i].bits))
			m_nMaxCodeLen = strlen(m_HTree[i].bits);
}

int HuffCode::EncodeFile()
{
	if (m_fpOutFile == NULL || m_fpInFile == NULL)
		return -1;

	fseek(m_fpOutFile, 0, 0);
	fwrite(&m_nRawFileLen, sizeof(unsigned int), 1, m_fpOutFile);
	fwrite(&m_nUsedChar, sizeof(unsigned int), 1, m_fpOutFile);
	fwrite(&m_nMaxCodeLen, sizeof(unsigned int), 1, m_fpOutFile);

	for (int i = 0; i <= m_nUsedChar; i++)
	{
		fwrite(&m_HTree[i].c, 1, 1, m_fpOutFile);
		fwrite(m_HTree[i].bits, m_nMaxCodeLen, 1, m_fpOutFile);
	}

	char buffer[N_Node];
	int c; // fgetc返回int
	int j = 0, k = 0;
	unsigned int n_wr = 12 + (m_nMaxCodeLen + 1) * (m_nUsedChar + 1);

	buffer[0] = '\0';
	fseek(m_fpInFile, 0, SEEK_SET);
	while (!feof(m_fpInFile))
	{
		c = fgetc(m_fpInFile);
		int i;
		j++;

		for (i = 0; i <= m_nUsedChar; i++)
		{
			if (m_HTree[i].c == c)
				break;
		}
		strcat(buffer, m_HTree[i].bits);

		k = strlen(buffer);
		c = 0;
		while (k >= 8)
		{
			for (i = 0; i < 8; i++)
			{
				if (buffer[i] == '1')
					c = (c << 1) | 0x01;
				else
					c = c << 1;
			}
				
			fwrite(&c, 1, 1, m_fpOutFile);
			n_wr++;
			strcpy(buffer, buffer + 8);
			k = strlen(buffer);
		}
		if (j == m_nRawFileLen)
			break;
	}

	if (k > 0)
	{
		strcat(buffer, "00000000");
		for (int i = 0; i < 8; i++)
		{
			if (buffer[i] == '1')
				c = (c << 1) | 0x01;
			else
				c = c << 1;
		}
			
		n_wr++;
		fwrite(&c, 1, 1, m_fpOutFile);
	}
	m_nCompressFileLen = n_wr;
	return n_wr;
}

int HuffCode::DecodeFile()
{
	xInit_Tree();
	if (m_fpInFile == NULL || m_fpOutFile == NULL)
		return -1;

	fseek(m_fpInFile, 0, SEEK_END);
	m_nCompressFileLen = ftell(m_fpInFile);
	fseek(m_fpInFile, 0, 0);

	fread(&m_nRawFileLen, sizeof(unsigned int), 1, m_fpInFile);
	fread(&m_nUsedChar, sizeof(unsigned int), 1, m_fpInFile);
	fread(&m_nMaxCodeLen, sizeof(unsigned int), 1, m_fpInFile);

	for (int i = 0; i <= m_nUsedChar; i++)
	{
		fread(&m_HTree[i].c, sizeof(unsigned char), 1, m_fpInFile);
		fread(&m_HTree[i].bits, m_nMaxCodeLen, 1, m_fpInFile);
	}

	unsigned char c;
	char buffer[256];
	char digital_num[256];
	int nOutChar = 0;

	buffer[0] = '\0';

	while (true)
	{
		while (strlen(buffer) < m_nMaxCodeLen)
		{
			fread(&c, 1, 1, m_fpInFile);
			int num = c;
			_itoa_s(num, digital_num, 2);
			int k = strlen(digital_num);
			for (int l = 8; l > k; l--)
				strcat(buffer, "0");
			strcat(buffer, digital_num);
		}

		int i;
		for (i = 0; i <= m_nUsedChar; i++)
		{
			if (memcmp(m_HTree[i].bits, buffer, strlen(m_HTree[i].bits)) == 0)
				break;
		}

		strcpy(buffer, buffer + strlen(m_HTree[i].bits));
		c = m_HTree[i].c;
		m_HTree[i].count++;
		fwrite(&c, 1, 1, m_fpOutFile);
		if (++nOutChar == m_nRawFileLen)
			break;
	}
	return nOutChar;
}

double HuffCode::GetRawFileEntropy()
{
	if (NULL == m_fpInFile)
		return -1;

	double entropy = 0.0;

	for (int i = 0; i < m_nUsedChar; i++)
	{
		if (m_HTree[i].count != 0)
		{
			double rate = 1.0 * m_HTree[i].count / (double)m_nRawFileLen;
			entropy += rate * log(double(rate)) / log(2.0);
		}
	}
	return entropy;
}

double HuffCode::GetAverCodeLen()
{
	if (NULL == m_fpInFile)
		return -1;

	double len = 0.0;
	int s = 0;

	for (int i = 0; i < m_nUsedChar; i++)
	{
		len += strlen(m_HTree[i].bits) * m_HTree[i].count;
		s += m_HTree[i].count;
	}

	return len * 1.0 / s;
}