#include "stdafx.h"
#include "ImgProcess.h"


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
			bGray = GetPixel(i, j);
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
			gray = GetPixel(i, j);

			target = gray * dFa + dFb;
			if (target < 0) target = 0;
			if (target > 255) target = 255;

			pTo->SetPixel(i, j, target);
		}
	}
}