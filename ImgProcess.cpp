#include "stdafx.h"
#include "ImgProcess.h"

#include <math.h>

#define PI 3.1415926

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

void CImgProcess::InMove(CImgProcess* pTo, int x, int y)
{
	int nHeight = pTo->GetHeight();
	int nWidth = pTo->GetWidthPixel();

	if (x > nWidth || y > nHeight)
	{
		MessageBox(NULL, L"超过图片大小", L"错误", MB_OK | MB_ICONERROR);
		return;
	}

	for (int i = 0; i < nWidth; i++)
	{
		for (int j = 0; j < nHeight; j++)
		{
			if (i - x < nWidth && i - x > 0 && j - y < nHeight && j - y > 0)
				pTo->SetPixel(i, j, GetPixel(i - x, j - y));
			else
				pTo->SetPixel(i, j, RGB(0, 0, 0));
		}
	}
}

void CImgProcess::Scale(CImgProcess* pTo, double times)
{
	int nHeight = pTo->GetHeight();
	int nWidth = pTo->GetWidthPixel();

	for (int i = 0; i < nWidth; i++)
	{
		for (int j = 0; j < nHeight; j++)
		{
			if ((int)(i / times + 0.5) < nWidth && (int)(j / times + 0.5) < nHeight)
				pTo->SetPixel(i, j, GetPixel(int(i / times + 0.5), int(j / times + 0.5)));
			else
				pTo->SetPixel(i, j, RGB(255, 255, 255));
		}
	}
}

void CImgProcess::HorMirror(CImgProcess* pTo)
{
	int nWidth = pTo->GetWidthPixel();
	int nHeight = pTo->GetHeight();

	for (int i = 0; i < nWidth; i++)
		for (int j = 0; j < nHeight; j++)
			pTo->SetPixel(i, j, GetPixel(nWidth - i - 1, j));
}

void CImgProcess::VerMirror(CImgProcess* pTo)
{
	int nWidth = pTo->GetWidthPixel();
	int nHeight = pTo->GetHeight();

	for (int i = 0; i < nWidth; i++)
		for (int j = 0; j < nHeight; j++)
			pTo->SetPixel(i, j, GetPixel(i, nHeight - j - 1));
} 

void CImgProcess::Transpose(CImgProcess* pTo)
{
	int nWidth = pTo->GetWidthPixel();
	int nHeight = pTo->GetHeight();

	for (int i = 0; i < nWidth; i++)
	{
		for (int j = 0; j < nHeight; j++)
		{
			if (i < nHeight && j < nWidth)
				pTo->SetPixel(i, j, GetPixel(j, i));
			else
				pTo->SetPixel(i, j, RGB(0, 0, 0));
		}
	}
}

void CImgProcess::Rotate(CImgProcess* pTo, float ang)
{
	int nHeight = pTo->GetHeight();
	int nWidth = pTo->GetWidthPixel();

	for (int i = 0; i < nWidth; i++)
	{
		for (int j = 0; j < nHeight; j++)
		{
			int x = (int)(i * ::cos(ang * PI / 180) + j * ::sin(ang * PI / 180) + 0.5);
			int y = (int)(j * ::cos(ang * PI / 180) - i * ::sin(ang * PI / 180) + 0.5);
			if (x < nWidth && y < nHeight && x >= 0 && y >= 0)
				pTo->SetPixel(i, j, GetPixel(x, y));
			else
				pTo->SetPixel(i, j, RGB(0, 0, 0));
		}
	}
}

// 双线性插值计算
int CImgProcess::InterpBilinear(double x, double y)
{
	double epsilon = 0.0001;

	int x1, x2;
	int y1, y2;

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	x1 = (int)x;
	x2 = (int)(x + 1);
	y1 = (int)y;
	y2 = (int)(y + 1);

	if ((x < 0) || (x > nWidth - 1) || (y < 0) || (y > nHeight - 1))
	{
		return -1;
	}
	else
	{
		// x 超出右边界
		if (::fabs(x - nWidth + 1) <= epsilon)
		{
			// y 超出底边
			if (::fabs(y - nHeight + 1) <= epsilon)
			{
				return GetPixel(x1, y1);
			}
			else // 取样点位于右边界
			{
				unsigned char f1 = (unsigned char)GetPixel(x1, y1);
				unsigned char f3 = (unsigned char)GetPixel(x1, y2);
				return ((int)((y - y1) *(f3 - f1) + f1));
			}
		}
		else if (::fabs(y - nHeight + 1) <= epsilon) // 取样点位于下边界
		{
			unsigned char f1 = (unsigned char)GetPixel(x1, y1);
			unsigned char f2 = (unsigned char)GetPixel(x2, y1);
			return ((int)((x - x1) * (f2 - f1) + f1));
		}
		else
		{
			unsigned char f1 = (unsigned char)GetPixel(x1, y1);
			unsigned char f2 = (unsigned char)GetPixel(x2, y1);
			unsigned char f3 = (unsigned char)GetPixel(x1, y2);
			unsigned char f4 = (unsigned char)GetPixel(x2, y2);

			unsigned char f12 = ((int)(f1 + (f2 - f1) * (x - x1)));
			unsigned char f34 = ((int)(f3 + (f4 - f3) * (x - x1)));

			return ((int)(f12 + (y - y1) * (f34 - f12)));
		}
	}
}