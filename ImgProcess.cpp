#include "stdafx.h"
#include "ImgProcess.h"

#include <math.h>

CImgProcess::CImgProcess()
{
}


CImgProcess::~CImgProcess()
{
}

CImgProcess& CImgProcess::operator=(CImg& img)
{
	if (IsValidate())
	{
		ImResize(img.GetHeight(), img.GetWidthPixel());
	}
	else
	{
		throw "Error in operaor '=': Left hand operand not initialized.";
		return *this;
	}

	for (int i = 0; i<GetHeight(); i++)
	{
		memcpy(m_lpData[i], img.m_lpData[i], img.GetWidthByte() * sizeof(BYTE));
	}

	return *this;
}

BOOL CImgProcess::GenHist(double* pdHist, int n)
{
	if (m_pBMIH->biBitCount != 8) return false;

	if ((n <= 0) || (n > 256)) return false;

	double dDivider;

	memset(pdHist, 0, n * sizeof(double));
	dDivider = 256.0 / (double)n;

	BYTE bGray;
	for (int i = 0; i < m_pBMIH->biHeight; i++)
	{
		for (int j = 0; j < m_pBMIH->biWidth; j++)
		{
			bGray = GetGray(j, i);
			pdHist[(int)(bGray / dDivider)]++;
		}
	}

	double square = m_pBMIH->biWidth * m_pBMIH->biHeight;
	for (int k = 0; k < n; k++)
	{
		pdHist[k] = pdHist[k] / square;
	}

	return true;
}

BOOL CImgProcess::LinTran(CImgProcess* pTo, double dFa, double dFb)
{
	if (m_pBMIH->biBitCount != 8)
		return false;

	BYTE gray;
	BYTE target;

	for (int i = 0; i < m_pBMIH->biHeight; i++)
	{
		for (int j = 0; j < m_pBMIH->biWidth; j++)
		{
			gray = GetPixel(j, i);

			target = gray * dFa + dFb;
			if (target < 0) target = 0;
			if (target > 255) target = 255;

			pTo->SetPixel(j, i, target);
		}
	}
	return TRUE;
}

BOOL CImgProcess::LogTran(CImgProcess* pTo, double dC)
{
	if (m_pBMIH->biBitCount != 8)
		return false;

	BYTE gray;
	BYTE target;

	for (int i = 0; i < m_pBMIH->biHeight; i++)
	{
		for (int j = 0; j < m_pBMIH->biWidth; j++)
		{
			gray = GetGray(j, i);
			target = dC * log((double)(gray + 1));

			if (target < 0) target = 0;
			if (target > 255) target = 255;

			pTo->SetPixel(j, i, target);
		}
	}

	return TRUE;
}

BOOL CImgProcess::GammaTran(CImgProcess* pTo, double gamma, double comp)
{
	if (m_pBMIH->biBitCount != 8)
		return false;

	BYTE gray;
	BYTE target;

	for (int i = 0; i < m_pBMIH->biHeight; i++)
	{
		for (int j = 0; j < m_pBMIH->biWidth; j++)
		{
			gray = GetGray(j, i);
			target = pow((gray + comp) / 255.0, gamma) * 255;
			if (target < 0) target = 0;
			if (target > 255) target = 255;
			pTo->SetPixel(j, i, target);
		}
	}
	return TRUE;
}

void CImgProcess::Threshold(CImgProcess* pTo, BYTE nTres)
{
	BYTE bt;
	for (int i = 0; i < m_pBMIH->biHeight; i++)
	{
		for (int j = 0; j < m_pBMIH->biWidth; j++)
		{
			bt = GetPixel(j, i);
			if (bt > nTres)
				bt = 255;
			else
				bt = 0;
			pTo->SetPixel(j, i, bt);
		}
	}
}

BOOL CImgProcess::ParLinTran(CImgProcess* pTo, BYTE x1, BYTE x2, BYTE y1, BYTE y2)
{
	if (m_pBMIH->biBitCount != 8)
		return false;

	BYTE gray;
	BYTE target;

	for (int i = 0; i < m_pBMIH->biHeight; i++)
	{
		for (int j = 0; j < m_pBMIH->biWidth; j++)
		{
			gray = GetGray(j, i);
			if (gray <= x1)
				target = gray * y1 / x1;
			else if (gray <= x2)
				target = (y2 - y1) * (gray - x1) / (x2 - x1) + y1;
			else
				target = (255 - y2) * (gray - x2) / (255 - x2) + y2;

			if (target < 0) target = 0;
			if (target > 255) target = 255;

			pTo->SetPixel(j, i, RGB(target, target, target));
		}
	}
	return TRUE;
}

BOOL CImgProcess::Histeq(CImgProcess* pTo)
{
	if (m_pBMIH->biBitCount != 8)
		return false;

	BYTE gray;
	int target;

	double hist[256];
	double dTemp;

	GenHist(hist);

	for (int i = 0; i < m_pBMIH->biHeight; i++)
	{
		for (int j = 0; j < m_pBMIH->biWidth; j++)
		{
			dTemp = 0;
			gray = GetGray(j, i);
			for (int k = 0; k < gray; k++)
				dTemp += *(hist + k);
			target = 255 * dTemp;

			if (target < 0) target = 0;
			if (target > 255) target = 255;

			pTo->SetPixel(j, i, RGB(target, target, target));
		}
	}
	return true;
}

BOOL CImgProcess::Histst(CImgProcess* pTo, double* pdStdHist)
{
	int histTmp[256];
	memset(histTmp, -1, sizeof(int) * 256);

	for (int i = 0; i < 256; i++)
	{
		double tmp = 0;
		for (int k = 0; k < i; k++)
			tmp += *(pdStdHist + k);
		*(histTmp + (int)(0.5 + 255 * tmp)) = i; // 这里如果不加0.5，诸如0.6、0.7等值不会进位到1
	}

	for (int i = 0; i < 256; i++)  // 对空缺值进行插值处理
	{
		int j = 0;
		if (histTmp[i + 1] != -1)
			continue;
		j = 1;
		while (histTmp[i + j] == -1 && (i + j) <= 255)
		{
			histTmp[i + j] = histTmp[i];
			j++;
		}
	}

	BYTE gray;
	double target;
	double hist[256];

	GenHist(hist);

	for (int i = 0; i < m_pBMIH->biHeight; i++)
	{
		double dTemp = 0;
		for (int j = 0; j < m_pBMIH->biWidth; j++)
		{
			dTemp = 0;
			gray = GetGray(j, i);
			for (int k = 0; k < gray; k++)
				dTemp += *(hist + k);
			target = 255 * dTemp;

			target = histTmp[(int)target];

			if (target < 0) target = 0;
			if (target > 255) target = 255;

			pTo->SetPixel(j, i, RGB(target, target, target));
		}
	}
	return TRUE;
}

BOOL CImgProcess::Histst(CImgProcess* pTo, CImgProcess* pStd)
{
	double hist[256];
	pStd->GenHist(hist);
	return Histst(pTo, hist);
}