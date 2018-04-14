#include "stdafx.h"
#include "ImgProcess.h"

#include <math.h>
#include <vector>
#include <conio.h>

#include <iostream>
#include <iomanip>

#define PI 3.1415926

// 平均平滑  1/9
float Template_Smooth_Avg[9] = { 1, 1, 1,
	1, 1, 1,
	1, 1, 1 };
// Gauss平滑  1/16
float Template_Smooth_Gauss[9] = { 1, 2, 1,
	2, 4, 2,
	1, 2, 1 };
// Sobel垂直边缘检测
float Template_HSobel[9] = { -1, 0, 1,
	-2, 0, 2,
	-1 ,0 , 1 };

// Sobel水平边缘检测
float Template_VSobel[9] = { -1, -2, -1,
	0, 0, 0,
	1, 2, 1 };

// LOG边缘检测
float Template_Log[25] = { 0, 0, -1, 0, 0,
	0, -1, -2, -1, 0,
	-1, -2, 16, -2, -1,
	0, -1, -2, -1, 0,
	0, 0, -1, 0, 0 };

//Sigma = 2，较平滑
double Template_Log1[25] = { 0.0448,    0.0468,    0.0564,    0.0468,    0.0448,
	0.0468,    0.3167,    0.7146,    0.3167,    0.0468,
	0.0564,    0.7146,   -4.9048,    0.7146,    0.0564,
	0.0468,    0.3167,    0.7146,    0.3167,    0.0468,
	0.0448,    0.0468,    0.0564,    0.0468,    0.0448
};

// Laplacian边缘检测
float Template_Laplacian1[9] = { 0, -1, 0,
	-1, 4, -1,
	0, -1, 0
};

float Template_Laplacian2[9] = { -1, -1, -1,
	-1, 8, -1,
	-1, -1, -1
};

float Template_Laplacian3[9] = { 1, 1, 1,
1, -8, 1,
1, 1, 1
};

// DCT量化数组
int Quant_table[8][8] =
{ 16,11,10,16,24,40,51,61,
12,12,14,19,26,58,60,55,
14,13,16,24,40,57,69,56,
14,17,22,29,51,87,80,62,
18,22,37,56,68,109,103,77,
24,35,55,64,81,104,113,92,
49,64,78,87,103,121,120,101,
72,92,95,98,112,100,103,99 };

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

void CImgProcess::Template(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, FLOAT *pfArray, FLOAT fCoef)
{
	//pTo->InitPixels(0);

	for (int i = nTempMY; i < GetHeight() - (nTempH - nTempMY) + 1; i++)
		for (int j = nTempMX; j < GetWidthPixel() - (nTempW - nTempMX) + 1; j++)
		{
			float fResult = 0.0f;
			for (int x = 0; x < nTempH; x++)
				for (int y = 0; y < nTempW; y++)
					fResult += GetGray(j - nTempMX + y, i - nTempMY + x) * pfArray[x * nTempW + y];
			fResult *= fCoef;
			fResult = (FLOAT)::fabs(fResult);
			BYTE byte;
			if (fResult > 255)
				byte = 255;
			else
				byte = (BYTE)(fResult + 0.5);

			pTo->SetPixel(j, i, RGB(byte, byte, byte));
		}
}

int CImgProcess::GetMedianValue(int* pArrGray, int len)
{
	for (int i = 1; i < len; i++)
		for (int j = i; j > 0 && (pArrGray[j] < pArrGray[j - 1]); j--)
		{
			int nTmp = pArrGray[j];
			pArrGray[j] = pArrGray[j - 1];
			pArrGray[j - 1] = nTmp;
		}

	if (len & 1)
		return pArrGray[len / 2];
	else
		return (pArrGray[len / 2] + pArrGray[len / 2 - 1]) / 2;
}

void CImgProcess::MedianFilter(CImgProcess* pTo, int nFilterH, int nFilterW, int nFilterMY, int nFilterMX)
{
	pTo->InitPixels(0);

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int* pArrGray = new int[nFilterH * nFilterW];

	for (int i = nFilterMY; i < nHeight - nFilterH + nFilterMY + 1; i++)
		for (int j = nFilterMX; j < nWidth - nFilterW + nFilterMX + 1; j++)
		{
			for (int y = 0; y < nFilterH; y++)
				for (int x = 0; x < nFilterW; x++)
					pArrGray[y * nFilterH + x] = GetGray(j - nFilterMX + x, i - nFilterMY + y);
			int nGray = GetMedianValue(pArrGray, nFilterH * nFilterW);
			pTo->SetPixel(j, i, RGB(nGray, nGray, nGray));
		}

	delete [] pArrGray;
}

void CImgProcess::AdaptiveMedianFilter(CImgProcess* pTo, int nFilterH, int nFilterW, int nFilterMY, int nFilterMX)
{
	pTo->InitPixels(0);

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int* pArrGray = new int[nFilterH * nFilterW];

	for (int i = nFilterMY; i < nHeight - nFilterH + nFilterMY + 1; i++)
		for (int j = nFilterMX; j < nWidth - nFilterW + nFilterMX + 1; j++)
		{
			for (int y = 0; y < nFilterH; y++)
				for (int x = 0; x < nFilterW; x++)
					pArrGray[y * nFilterH + x] = GetGray(j - nFilterMX + x, i - nFilterMY + y);
			int nGray = GetMedianValue(pArrGray, nFilterH * nFilterW);
			if (GetGray(j, i) == pArrGray[0] || GetGray(j, i) == pArrGray[nFilterH * nFilterW - 1])
				pTo->SetPixel(j, i, RGB(nGray, nGray, nGray));
			else
				pTo->SetPixel(j, i, GetPixel(j, i));
		}

	delete[] pArrGray;
}

void CImgProcess::FilterSobel(CImgProcess* pTo)
{
	CImgProcess img1, img2;
	img1 = *pTo;
	img2 = *pTo;

	Template(&img1, 3, 3, 1, 1, Template_HSobel, 1);
	Template(&img2, 3, 3, 1, 1, Template_VSobel, 1);

	*pTo = img1 + img2;
}

void CImgProcess::FilterLaplacian(CImgProcess* pTo)
{
	CImgProcess img1;
	img1 = *pTo;

	Template(&img1, 3, 3, 1, 1, Template_Laplacian3, 1);
	
	*pTo = img1;
}

void CImgProcess::EnhanceFilter(CImgProcess *pTo, double dProportion,
	int nTempH, int nTempW,
	int nTempMY, int nTempMX, FLOAT *pfArray, FLOAT fCoef)
{
	int i, j;
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	*pTo = *this; //目标图像初始化

				  //GrayMat暂存按比例叠加图像（不能在CImg类对象中直接进行像素相加，因为相加的结果可能超出范围[0,255]）
	std::vector< std::vector<int> > GrayMat;
	std::vector<int> vecRow(nWidth, 0); //GrayMat中的一行（初始化为0）
	for (i = 0; i<nHeight; i++)
	{
		GrayMat.push_back(vecRow);
	}

	//锐化图像，输出带符号响应，并与原图像按比例叠加			
	for (i = nTempMY; i<GetHeight() - (nTempH - nTempMY) + 1; i++)
	{
		for (j = nTempMX; j<GetWidthPixel() - (nTempW - nTempMX) + 1; j++)
		{
			// (j,i)为中心点
			float fResult = 0;
			for (int k = 0; k<nTempH; k++)
			{
				for (int l = 0; l<nTempW; l++)
				{
					//计算加权和
					fResult += GetGray(j + l - nTempMX, i + k - nTempMY) * pfArray[k * nTempW + l];
				}
			}

			// 乘以系数
			fResult *= fCoef;

			//限制响应值范围
			if (fResult > 255)
				fResult = 255;
			if (fResult < -255)
				fResult = -255;

			GrayMat[i][j] = (int)double(dProportion * GetGray(j, i) + fResult + 0.5);//求和，结果四舍五入
		}//for j
	}//for i



	int nMax = 0;//最大灰度和值
	int nMin = 65535; //最小灰度和值

					  //统计最大、最小值
	for (i = nTempMY; i<GetHeight() - (nTempH - nTempMY) + 1; i++)
	{
		for (j = nTempMX; j<GetWidthPixel() - (nTempW - nTempMX) + 1; j++)
		{
			if (GrayMat[i][j] > nMax)
				nMax = GrayMat[i][j];
			if (GrayMat[i][j] < nMin)
				nMin = GrayMat[i][j];
		}// j
	}// i

	 //将GrayMat的取值范围重新归一化到[0, 255]
	int nSpan = nMax - nMin;

	for (i = nTempMY; i<GetHeight() - (nTempH - nTempMY) + 1; i++)
	{
		for (j = nTempMX; j<GetWidthPixel() - (nTempW - nTempMX) + 1; j++)
		{
			BYTE bt;
			if (nSpan > 0)
				bt = (GrayMat[i][j] - nMin) * 255 / nSpan;
			else if (GrayMat[i][j] <= 255)
				bt = GrayMat[i][j];
			else
				bt = 255;

			pTo->SetPixel(j, i, RGB(bt, bt, bt));

		}// for j
	}// for i
}

void CImgProcess::FFT(std::complex<double>* TD, std::complex<double>* FD, int r)
{
	LONG	count;
	int		i, j, k;
	int		bfsize, p;
	double  angle;

	std::complex<double> *W, *X1, *X2, *X;

	count = 1 << r;

	W = new std::complex<double>[count / 2];
	X = new std::complex<double>[count];
	X1 = new std::complex<double>[count];
	X2 = new std::complex<double>[count];

	for (i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2.0 / count;
		W[i] = std::complex<double>(cos(angle), sin(angle));
	}

	memcpy(X1, TD, sizeof(std::complex<double>) * count);

	for (k = 0; k < r; k++)
	{
		for (j = 0; j < 1 << k; j++)
		{
			bfsize = 1 << (r - k);
			for (i = 0; i < bfsize / 2; i++)
			{
				p = j * bfsize;
				X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
				X2[i + p + bfsize / 2] = (X1[i + p] - X1[i + p + bfsize / 2]) * W[i * (1 << k)];
			}
		}
		X = X1;
		X1 = X2;
		X2 = X;
	}

	for (j = 0; j < count; j++)
	{
		p = 0;
		for (i = 0; i < r; i++)
			if (j & (1 << i))
				p += 1 << (r - i - 1);
		FD[j] = X1[p];
	}

	delete [] W;
	delete [] X1;
	delete [] X2;
}

void CImgProcess::IFFT(std::complex<double>* FD, std::complex<double>* TD, int r)
{
	LONG count;

	std::complex<double>* X;

	count = 1 << r;

	X = new std::complex<double>[count];

	memcpy(X, FD, sizeof(std::complex<double>) * count);

	for (int i = 0; i < count; i++)
		X[i] = std::complex<double>(X[i].real(), -X[i].imag());

	FFT(X, TD, r);

	for (int i = 0; i < count; i++)
		TD[i] = std::complex<double>(TD[i].real() / count, -TD[i].imag() / count);

	delete X;
}

void CImgProcess::FFT2(CImgProcess* pTo, BOOL bExpand, std::complex<double>* pOutput, BYTE bFillColor)
{
	double dTemp;
	LONG i, j;
	LONG w, h;

	int wp, hp;

	w = h = 1;
	wp = hp = 0;

	while (w * 2 <= GetWidthPixel())
	{
		w *= 2;
		wp++;
	}

	while (h * 2 <= GetHeight())
	{
		h *= 2;
		hp++;
	}

	if ((bExpand) && (20 < GetWidthPixel() - w)) { w *= 2; wp++; }
	if ((bExpand) && (20 < GetHeight() - h)) { h *= 2; hp++; }
		

	std::complex<double>* TD = new std::complex<double>[w * h];
	std::complex<double>* FD = new std::complex<double>[w * h];

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			if (bExpand)
			{
				if ((j < GetWidthPixel()) && (i < GetHeight()))
					TD[j + w * i] = std::complex<double>(GetGray(j, i), 0);
				else
					TD[j + w * i] = std::complex<double>(bFillColor, 0);
			}
			else
			{
				TD[j + w * i] = std::complex<double>(GetGray(j, i), 0);
			}
		}
	}

	for (i = 0; i < h; i++)
		FFT(&TD[i * w], &FD[i * w], wp);

	// 转置矩阵
	for (i = 0; i < h; i++)
		for (j = 0; j < w; j++)
			TD[i + h * j] = FD[j + w * i];

	for (i = 0; i < w; i++)
		FFT(&TD[i * h], &FD[i * h], hp);

	for (i = 0; i < h; i++)
		for (j = 0; j < w; j++)
			pOutput[i * w + j] = FD[j * h + i];

	if (pTo)
	{
		pTo->ImResize(h, w);

		double dMax = 0, dMin = 1E+6;

		for (i = 0; i < w * h; i++)
		{
			dTemp = sqrt(FD[i].real() * FD[i].real() + FD[i].imag() * FD[i].imag());
			dTemp = log(1 + dTemp);
			if (dTemp > dMax) dMax = dTemp;
			if (dTemp < dMin) dMin = dTemp;
		}

		for (i = 0; i < w * h; i++)
		{
			dTemp = sqrt(FD[i].real() * FD[i].real() + FD[i].imag() * FD[i].imag());
			dTemp = log(1 + dTemp);
			dTemp = (dTemp - dMin) / (dMax - dMin) * 255;
			int hi = i / w; int wi = i % w;
			pTo->SetPixel((wi >= w/2 ? wi - w/2 : wi + w/2), (hi >= h/2 ? hi - h/2 : hi + h/2), RGB(dTemp, dTemp, dTemp));
		}
	}
	delete [] TD;
	delete [] FD;
}

void CImgProcess::IFFT2(CImgProcess* pTo, std::complex<double>* pDFT, int iOutH, int iOutW, int iHeight, int iWidth)
{
	int w = 1, h = 1;
	int wp = 0, hp = 0;

	if (iOutW == 0) iOutW = iWidth;
	if (iOutH == 0) iOutH = iHeight;

	while (w * 2 <= iOutW) { w *= 2; wp++; }
	while (h * 2 <= iOutH) { h *= 2; hp++; }

	pTo->ImResize(h, w);

	std::complex<double>* FD = new std::complex<double>[w * h];

	memcpy(FD, pDFT, sizeof(std::complex<double>) * w * h);

	for (int i = 0; i < h; i++)
		IFFT(&FD[i * w], &pDFT[i * w], wp);

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			FD[j * h + i] = pDFT[i * w + j];

	for (int i = 0; i < w; i++)
		IFFT(&FD[i * h], &pDFT[i * h], hp);

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			FD[i * w + j] = pDFT[j * h + i];

	if (pTo)
	{
		double dTemp = 0;
		double dMax = 0, dMin = 1E+6;
		for (int i = 0; i < w * h; i++)
		{
			dTemp = FD[i].real();
			if (dTemp > dMax) dMax = dTemp;
			if (dTemp < dMin) dMin = dTemp;
		}

		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				dTemp = FD[i * w + j].real();
				dTemp = (dTemp - dMin) / (dMax - dMin) * 255;
				pTo->SetPixel(j, i, RGB(dTemp, dTemp, dTemp));
			}
		}
	}

	delete[] FD;
}

int CImgProcess::GetFreqHeight(int nHeight, BOOLEAN bPadding = TRUE)
{
	int h = 1;
	while (h * 2 <= nHeight) h *= 2;
	if (bPadding && (20 < nHeight - h))
		h *= 2;
	return h;
}

int CImgProcess::GetFreqWidth(int nWidth, BOOLEAN bPadding = TRUE)
{
	int w = 1;
	while (w * 2 <= nWidth) w *= 2;
	if (bPadding && w != nWidth)
		w *= 2;
	return w;
}

void CImgProcess::FreqFilter(CImgProcess* pTo, double* dpFilter, BYTE fillColor)
{
	int w = 1, h = 1;

	w = GetFreqWidth(GetWidthPixel(), FALSE);
	h = GetFreqHeight(GetHeight(), FALSE);

	std::complex<double>* cdFreqImg = new std::complex<double>[w * h];

	FFT2(NULL, FALSE, cdFreqImg, fillColor);

	for (int i = 0; i < w * h; i++)
		cdFreqImg[i] *= dpFilter[i];

	IFFT2(pTo, cdFreqImg, 0, 0, h, w);

	delete[] cdFreqImg;
}

void CImgProcess::FreqIdealLPF(double* dpFilter, int nFreq)
{
	/*int w = GetWidthPixel();
	int h = GetHeight();*/

	int w = 1, h = 1;

	w = GetFreqWidth(GetWidthPixel(), FALSE);
	h = GetFreqHeight(GetHeight(), FALSE);

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			if (sqrt(pow(double(i - h / 2), 2) + pow(double(j - w / 2), 2)) > nFreq)
				//dpFilter[i * w + j] = 0;
				dpFilter[(i < h / 2 ? i + h / 2 : i - h / 2) * w + (j < w / 2 ? j + w / 2 : j - w / 2)] = 0;
			else
				//dpFilter[i * w + j] = 1;
				dpFilter[(i < h / 2 ? i + h / 2 : i - h / 2) * w + (j < w / 2 ? j + w / 2 : j - w / 2)] = 1;

	
}

void CImgProcess::FreqGaussLPF(double* dpFilter, int nSigma)
{
	int w = 1, h = 1;
	
	w = GetFreqWidth(GetWidthPixel(), FALSE);
	h = GetFreqHeight(GetHeight(), FALSE);

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			dpFilter[(i < h / 2 ? i + h / 2 : i - h / 2) * w + (j < w / 2 ? j + w / 2 : j - w / 2)] =
			exp(-(pow(i - h/2, 2) + pow(j - w/2, 2)) / (2 * pow(nSigma, 2)));
}

void CImgProcess::FreqGaussHPF(double* dpFilter, int nSigma)
{
	int w = 1, h = 1;

	w = GetFreqWidth(GetWidthPixel(), FALSE);
	h = GetFreqHeight(GetHeight(), FALSE);

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			dpFilter[(i < h / 2 ? i + h / 2 : i - h / 2) * w + (j < w / 2 ? j + w / 2 : j - w / 2)] =
			1 - exp(-(pow(i - h / 2, 2) + pow(j - w / 2, 2)) / (2 * pow(nSigma, 2)));
}

void CImgProcess::FreqLaplace(double* dpFilter)
{
	int w = 1, h = 1;

	w = GetFreqWidth(GetWidthPixel(), FALSE);
	h = GetFreqHeight(GetHeight(), FALSE);

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			dpFilter[(i < h / 2 ? i + h / 2 : i - h / 2) * w + (j < w / 2 ? j + w / 2 : j - w / 2)] =
			-((pow(i - h / 2, 2) + pow(j - w / 2, 2)));
}

void CImgProcess::AddUniform(CImgProcess* pTo)
{
	double rate = 0.99;
	int w = pTo->GetWidthPixel();
	int h = pTo->GetHeight();
	double lo = -20, hi = 20;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			double a0 = double(rand()) / RAND_MAX;
			if (a0 < rate)
			{
				double a1 = double(rand()) / RAND_MAX;
				a1 *= (hi - lo);
				a1 += lo;
				a1 += GetGray(j, i);
				int it = int(a1 + 0.5);
				if (it > 255)
					it = 255;
				if (it < 0)
					it = 0;
				pTo->SetPixel(j, i, RGB(it, it, it));
			}
		}
	}
}

void CImgProcess::AddGaussian(CImgProcess* pTo)
{
	int w = GetWidthPixel();
	int h = GetHeight();
	double rate = 0.99;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			double a0 = double(rand()) / RAND_MAX;
			if (a0 < rate)
			{
				double a1 = double(rand()) / RAND_MAX;
				double a2 = double(rand()) / RAND_MAX;
				double a = sqrt(-2 * log(1 - a1)) * cos(2 * PI * a2);
				a *= 15;
				a += GetGray(j, i);
				int it = int(a + 0.5);
				if (it > 255)
					it = 255;
				if (it < 0)
					it = 0;
				pTo->SetPixel(j, i, RGB(it, it, it));
			}
		}
	}
}

void CImgProcess::AddSlat_Pepper(CImgProcess* pTo)
{
	int w = GetWidthPixel();
	int h = GetHeight();

	double rate = 0.02;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			double a0 = double(rand()) / RAND_MAX;
			if (a0 < rate)
			{
				double a1 = double(rand()) / RAND_MAX;
				if (a1 < 0.5)
					pTo->SetPixel(j, i, RGB(0, 0, 0));
				else
					pTo->SetPixel(j, i, RGB(255, 255, 255));
			}
		}
	}
}

void CImgProcess::AddRayleigh(CImgProcess* pTo)
{
	int w = GetWidthPixel();
	int h = GetHeight();

	double rate = 0.99;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			double a0 = double(rand()) / RAND_MAX;
			if (a0 < rate)
			{
				double a1 = double(rand()) / RAND_MAX;
				double a2 = double(rand()) / RAND_MAX;
				double a3 = sqrt(-2 * log(a1)) * cos(2 * PI * a2);
				double a4 = sqrt(-2 * log(a2)) * cos(2 * PI * a1);
				a3 *= 15;
				a4 *= 15;
				a0 = sqrt(a3 * a3 + a4 * a4);
				a0 += GetGray(j, i);
				int gray = int(a0);
				if (gray > 255)
					gray = 255;
				if (gray < 0)
					gray = 0;
				pTo->SetPixel(j, i, RGB(gray, gray, gray));
			}
		}
	}
}

void CImgProcess::FreqInvTuihua(double* dpFilter)
{
	int w = GetFreqWidth(GetWidthPixel(), FALSE);
	int h = GetFreqHeight(GetHeight(), FALSE);

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{   // f1: 退化函数模型
			double f1 = ((i - h / 2) * (i - h / 2) + (j - w / 2) * (j - w / 2));
			f1 = pow(f1, 5.0 / 6);
			f1 *= -0.0025;
			f1 = exp(f1);
			dpFilter[(i < h / 2 ? i + h / 2 : i - h / 2) * w + (j < w / 2 ? j + w / 2 : j - w / 2)] = f1;
		}
	}
}

void CImgProcess::FreqInvFilter(double* dpFilter, int nRad)
{
	int w = GetFreqWidth(GetWidthPixel(), FALSE);
	int h = GetFreqHeight(GetHeight(), FALSE);

	memset(dpFilter, 1.0, sizeof(double) * w * h);

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (sqrt(pow(i - h / 2, 2) + pow(j - w / 2, 2)) <= nRad)
			{
				double f1 = ((i - h / 2) * (i - h / 2) + (j - w / 2) * (j - w / 2));
				f1 = pow(f1, 5.0 / 6);
				f1 *= -0.0025;
				f1 = exp(f1);
				dpFilter[(i < h / 2 ? i + h / 2 : i - h / 2) * w + (j < w / 2 ? j + w / 2 : j - w / 2)] = 1.0 / (f1 + 0.000001);
			}
		}
	}
}

void CImgProcess::FreqWienerFilter(double* dpFilter, int nRad, double k)
{
	int w = GetFreqWidth(GetWidthPixel(), FALSE);
	int h = GetFreqHeight(GetHeight(), FALSE);

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{   // f1: 退化函数模型
			double f1 = ((i - h / 2) * (i - h / 2) + (j - w / 2) * (j - w / 2));
			f1 = pow(f1, 5.0 / 6);
			f1 *= -0.0025;
			f1 = exp(f1);
			dpFilter[(i < h / 2 ? i + h / 2 : i - h / 2) * w + (j < w / 2 ? j + w / 2 : j - w / 2)] = 1.0 * f1 * f1 / ((f1 * f1 + k) * (f1 + 0.000001));
		}
	}
}

void CImgProcess::RGB2CMY(CImgProcess* pTo)
{
	int w = GetWidthPixel();
	int h = GetHeight();

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			COLORREF rgb = GetPixel(j, i);
			int r = GetRValue(rgb);
			int g = GetGValue(rgb);
			int b = GetBValue(rgb);
			int c = 255 - r;
			int m = 255 - g;
			int y = 255 - b;
			pTo->SetPixel(j, i, RGB(c, m, y));
		}
	}
}

void CImgProcess::RGB2HSI(CImgProcess* pTo)
{
	int nWidth = GetWidthPixel();
	int nHeight = GetHeight();

	double radians, angle;

	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			COLORREF rgbPixel = GetPixel(j, i);
			double r = GetRValue(rgbPixel) / 255.0;
			double g = GetGValue(rgbPixel) / 255.0;
			double b = GetBValue(rgbPixel) / 255.0;

			double h, s, in;
			int maxRGB = max(max(r, g), b);
			int minRGB = min(min(r, g), b);

			in = (r + g + b) / 3;

			if (in < 0.078431)
				s = 0;
			else if (in > 0.92)
				s = 0;
			else
				s = 1.0 - 3 / (r + g + b) * minRGB;

			if (minRGB == maxRGB)
			{
				h = 0;
				s = 0;
			}

			double q1 = ((r - g) + (r - b)) / 2;
			double q2 = sqrt((r - g) * (r - g) + (r - b) * (g - b));
			double q = q1 / q2;
			if (q > 0.99999)
				radians = 0;
			else if (q < -0.99999)
				radians = PI;
			else
				radians = acos(q);
			angle = radians * 180 / PI;

			if (b > g)
				h = 360.0 - angle;
			else
				h = angle;

			in = 255 * in;
			s = 255 * s;
			pTo->SetPixel(j, i, RGB(h, s, in));
		}
	}
}

void CImgProcess::HSI2RGB(CImgProcess* pTo)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();
	
	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			double H = GetRValue(RGBPixel);
			double S = GetGValue(RGBPixel) * 255.0;
			double I = GetBValue(RGBPixel) * 255.0;
			double R, G, B;

			if (H >= 0 && H < 120)
			{
				H = H;
				B = I * (1.0 - S);
				R = I * (1.0 + ((S * cos(H)) / cos(60 - H)));
				G = 3.0 * I - R - B;
			}
			else if (H >= 120 && H < 240)
			{
				H = H - 120;
				R = I * (1.0 - S);
				G = I * (1.0 + ((S * cos(H)) / cos(60 - H)));
				B = 3.0 * I - R - G;
			}
			else 
			{
				H = H - 240;
				G = I * (1.0 - S);
				B = I * (1.0 + ((S * cos(H)) / cos(60 - H)));
				R = 3.0 * I - B - G;
			}

			R *= 255.0;
			G *= 255.0;
			B *= 255.0;

			pTo->SetPixel(j, i, RGB(R, G, B));
		}
	}
}

void CImgProcess::RGB2HSV(CImgProcess *pTo)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;

	for (i = 0; i<nHeight; i++)
	{
		for (j = 0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//抽取RGB分量
			double R = GetRValue(RGBPixel) / 255.0;
			double G = GetGValue(RGBPixel) / 255.0;
			double B = GetBValue(RGBPixel) / 255.0;

			//计算HSV
			double H, S, V, MAX, MIN, TEMP;
			MAX = max(max(R, G), B);
			MIN = min(min(R, G), B);
			V = MAX;
			TEMP = MAX - MIN;

			if (MAX != 0)
			{
				S = TEMP / MAX;
			}
			else
			{
				S = 0;
				//H = UNDEFINEDCOLOR;
				return;
			}
			if (R == MAX)
				H = (G - B) / TEMP;
			else if (G == MAX)
				H = 2 + (B - R) / TEMP;
			else
				H = 4 + (R - G) / TEMP;
			H *= 60;
			if (H < 0)
				H += 360;

			//将HSV分量化为能在计算机上范围为[0，255]可显示的图像
			H /= 360.0;
			H *= 255.0;
			S *= 255.0;
			V *= 255.0;

			//将分量联合形成HSV图像
			pTo->SetPixel(j, i, RGB(H, S, V));
		}
	}
}

void CImgProcess::HSV2RGB(CImgProcess *pTo)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;

	for (i = 0; i<nHeight; i++)
	{
		for (j = 0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//抽取HSV分量
			double H = (GetRValue(RGBPixel) / 255.0)*360.0;
			double S = GetGValue(RGBPixel) / 255.0;
			double V = GetBValue(RGBPixel) / 255.0;

			//计算RGB
			double R, G, B, f, p, q, t, TEMP;
			int n;

			if (S == 0)
			{
				R = G = B = V;
			}

			n = int(floor(H / 60));
			TEMP = H / 60;
			f = TEMP - n;
			p = V*(1 - S);
			q = V*(1 - f*S);
			t = V*(1 - (1 - f)*S);

			switch (n)
			{
			case 0:
				R = V;
				G = t;
				B = p;
				break;
			case 1:
				R = q;
				G = V;
				B = p;
				break;
			case 2:
				R = p;
				G = V;
				B = t;
				break;
			case 3:
				R = p;
				G = q;
				B = V;
				break;
			case 4:
				R = t;
				G = p;
				B = V;
				break;
			default:  //case 5:
				R = V;
				G = p;
				B = q;
				break;
			}

			R *= 255.0;
			G *= 255.0;
			B *= 255.0;

			//将分量联合形成RGB图像
			pTo->SetPixel(j, i, RGB(R, G, B));
		}// for j
	}//for i
}

void CImgProcess::RGB2YUV(CImgProcess *pTo)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;

	for (i = 0; i<nHeight; i++)
	{
		for (j = 0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//抽取RGB分量
			double R = GetRValue(RGBPixel);
			double G = GetGValue(RGBPixel);
			double B = GetBValue(RGBPixel);

			//计算YUV
			double Y, U, V;
			Y = 0.299*R + 0.587*G + 0.114*B;
			U = (B - Y)*0.567;
			V = (R - Y)*0.713;

			//防止溢出
			if (Y > 255)
				Y = 255;
			if (Y < 0)
				Y = 0;
			if (U > 255)
				U = 255;
			if (U < 0)
				U = 0;
			if (V > 255)
				V = 255;
			if (V < 0)
				V = 0;

			//将分量联合形成YUV图像
			pTo->SetPixel(j, i, RGB(Y, U, V));
		}//for j
	}//for i
}

void CImgProcess::YUV2RGB(CImgProcess *pTo)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;

	for (i = 0; i<nHeight; i++)
	{
		for (j = 0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//抽取YUV分量
			double Y = GetRValue(RGBPixel);
			double U = GetGValue(RGBPixel);
			double V = GetBValue(RGBPixel);

			//计算RGB
			double R, G, B;
			R = Y + 1.402*V;
			G = Y - 0.344*U - 0.714*V;
			B = Y + 1.772*U;

			//防止溢出
			if (R > 255)
				R = 255;
			if (R < 0)
				R = 0;
			if (G > 255)
				G = 255;
			if (G < 0)
				G = 0;
			if (B > 255)
				B = 255;
			if (B < 0)
				B = 0;

			//将分量联合形成RGB图像
			pTo->SetPixel(j, i, RGB(R, G, B));
		}
	}
}

void CImgProcess::RGB2YIQ(CImgProcess *pTo)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;

	for (i = 0; i<nHeight; i++)
	{
		for (j = 0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//抽取RGB分量
			double R = GetRValue(RGBPixel);
			double G = GetGValue(RGBPixel);
			double B = GetBValue(RGBPixel);

			//计算YUV
			double Y, I, Q;
			Y = 0.299*R + 0.587*G + 0.114*B;
			I = 0.596*R - 0.274*G - 0.322*B;
			Q = 0.211*R - 0.523*G + 0.312*B;

			//防止溢出
			if (Y > 255)
				Y = 255;
			if (Y < 0)
				Y = 0;
			if (I > 255)
				I = 255;
			if (I < 0)
				I = 0;
			if (Q > 255)
				Q = 255;
			if (Q < 0)
				Q = 0;

			//将分量联合形成YIQ图像
			pTo->SetPixel(j, i, RGB(Y, I, Q));
		}
	}
}

void CImgProcess::YIQ2RGB(CImgProcess *pTo)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;

	for (i = 0; i<nHeight; i++)
	{
		for (j = 0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//抽取YIQ分量
			double Y = GetRValue(RGBPixel);
			double I = GetGValue(RGBPixel);
			double Q = GetBValue(RGBPixel);

			//计算RGB
			double R, G, B;
			R = Y + 0.956*I + 0.114*Q;
			G = Y - 0.272*I - 0.647*Q;
			B = Y - 1.106*I + 1.703*Q;

			//防止溢出
			if (R > 255)
				R = 255;
			if (R < 0)
				R = 0;
			if (G > 255)
				G = 255;
			if (G < 0)
				G = 0;
			if (B > 255)
				B = 255;
			if (B < 0)
				B = 0;

			//将分量联合形成RGB图像
			pTo->SetPixel(j, i, RGB(R, G, B));
		}//for j
	}//for i
}

void CImgProcess::dct8x8(double* in, double* out)
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			in[i * 8 + j] -= 128.0;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			double s = 0.0;
			for (int m = 0; m < 8; m++)
				for (int n = 0; n < 8; n++)
					s += in[m * 8 + n] * cos((2 * m + 1) * PI * i / 16.0) * cos((2 * n + 1) * PI * j / 16.0);
			out[i * 8 + j] = s / 4;
			if (i == 0)
				out[i * 8 + j] /= sqrt(2.0);
			if (j == 0)
				out[i * 8 + j] /= sqrt(2.0);		
		}
	}

	//if (AllocConsole())
	//{
	//	freopen("CONOUT$", "w", stdout);
	//	for (int m = 0; m < 8; m++)
	//	{
	//		for (int n = 0; n < 8; n++)
	//		{
	//			std::cout << std::setw(8) << out[m * 8 + n] << " ";
	//		}
	//		std::cout << std::endl;
	//	}
	//	FreeConsole();
	//}
}

void CImgProcess::idct8x8(double* in, double* out)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			double s = 0;
			for (int m = 0; m < 8; m++)
			{
				for (int n = 0; n < 8; n++)
				{
					double f = in[m * 8 + n] * cos((2 * i + 1) * m * PI / 16.0) * cos((2 * j + 1) * n * PI / 16.0);
					if (m == 0)
						f /= sqrt(2.0);
					if (n == 0)
						f /= sqrt(2.0);
					s += f;
				}
			}
			out[i * 8 + j] = s / 4;
		}
	}

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			out[i * 8 + j] += 128.0;
}

void CImgProcess::quant(double* in, int* out)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			double s = in[i * 8 + j] / Quant_table[i][j];
			if (in[i * 8 + j] >= 0)
				out[i * 8 + j] = int(floor(s + 0.5));
			else
				out[i * 8 + j] = int(ceil(s - 0.5));
			if (out[i * 8 + j] < -128)
				out[i * 8 + j] = -128;
			if (out[i * 8 + j] > 127)
				out[i * 8 + j] = 127;
		}
	}

	//if (AllocConsole())
	//{
	//	freopen("CONOUT$", "w", stdout);
	//	for (int m = 0; m < 8; m++)
	//	{
	//		for (int n = 0; n < 8; n++)
	//		{
	//			std::cout << std::setw(8) << out[m * 8 + n] << " ";
	//		}
	//		std::cout << std::endl;
	//	}
	//	FreeConsole();
	//}
}

void CImgProcess::iquant(int* in, double* out)
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			out[i * 8 + j] = in[i * 8 + j] * double(Quant_table[i][j]);
}

void CImgProcess::DCT_ALL(CImgProcess* pTo)
{
	int nHeight = GetHeight() / 8;
	int nWidth = GetWidthPixel() / 8;

	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			double din[64], dout[64];
			int iout[64];
			for (int m = 0; m < 8; m++)
				for (int n = 0; n < 8; n++)
					din[m * 8 + n] = double(GetGray(j * 8 + n, i * 8 + m));
			dct8x8(din, dout);
			quant(dout, iout);

			for (int m = 0; m < 8; m++)
				for (int n = 0; n < 8; n++)
				{
					unsigned char t = (unsigned char)(iout[m * 8 + n] + 50);
					pTo->SetPixel(j * 8 + n, i * 8 + m, RGB(t, t, t));
				}
		}
	}
}

void CImgProcess::IDCT_ALL(CImgProcess* pTo)
{
	int nHeight = GetHeight() / 8;
	int nWidth = GetWidthPixel() / 8;

	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			double dout[64], dout1[64];
			int iin[64];
			for (int m = 0; m < 8; m++)
				for (int n = 0; n < 8; n++)
					iin[m * 8 + n] = GetGray(j * 8 + n, i * 8 + m) - 50;

			iquant(iin, dout);
			idct8x8(dout, dout1);

			for (int m = 0; m < 8; m++)
				for (int n = 0; n < 8; n++)
				{
					double d = dout1[m * 8 + n];
					if (d < 0)
						d = 0;
					if (d > 254.5)
						d = 254.0;
					unsigned char t = (unsigned char)(int)(d + 0.5);
					pTo->SetPixel(j * 8 + n, i * 8 + m, RGB(d, d, d));
				}
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

void CImgProcess::Erode(CImgProcess* pTo, int se[3][3])
{
	bool bMatched;

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	pTo->InitPixels(255);

	for (int i = 1; i < nHeight - 1; i++)
	{
		for (int j = 1; j < nWidth - 1; j++)
		{
			bMatched = true;
			for (int m = 0; m < 3; m++)
			{
				for (int n = 0; n < 3; n++)
				{
					if (se[m][n] == -1)
						continue;
					if (se[m][n] == 0)
					{
						if (GetGray(j - 1 + n, i - 1 + m) != 255)
						{
							bMatched = false;
							break;
						}
					}
					else if (se[m][n] == 1)
					{
						if (GetGray(j - 1 + n, i - 1 + m) != 0)
						{
							bMatched = false;
							break;
						}
					}
				}
			}
			if (bMatched)
				pTo->SetPixel(j, i, RGB(0, 0, 0));
		}
	}
}

void CImgProcess::Dilate(CImgProcess* pTo, int se[3][3])
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	for (int i = 1; i < nHeight - 1; i++)
	{
		for (int j = 1; j < nWidth - 1; j++)
		{
			for (int m = 0; m < 3; m++)
			{
				for (int n = 0; n < 3; n++)
				{
					if (se[m][n] == -1)
						continue;
					if (se[m][n] == 1)
					{
						if (GetGray(j - 1 + n, i - 1 + m) == 0)
						{
							pTo->SetPixel(j, i, RGB(0, 0, 0));
							break;
						}
					}
				}
			}
		}
	}
}

void CImgProcess::Open(CImgProcess* pTo, int se[3][3])
{
	pTo->InitPixels(255);
	Erode(pTo, se);
	CImgProcess tmpImg = *pTo;
	tmpImg.Dilate(pTo, se);
}

void CImgProcess::Close(CImgProcess* pTo, int se[3][3])
{
	pTo->InitPixels(255);
	Dilate(pTo, se);
	CImgProcess tmpImg = *pTo;
	tmpImg.Erode(pTo, se);
}

void CImgProcess::LabelConnRgn(CImgProcess* pTo, int nConn)
{
	int se[3][3] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	if (nConn == 4)
	{
		se[0][0] = -1;
		se[0][2] = -1;
		se[2][0] = -1;
		se[2][2] = -1;
	}

	CImgProcess ImgBackup = *this;
	CImgProcess ImgTmp = *pTo;

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();
	int nConnRgn = 1;

	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			BYTE gray = GetGray(j, i);
			if (gray == 0)
			{
				pTo->InitPixels(255);
				pTo->SetPixel(j, i, RGB(0, 0, 0));

				ImgTmp = *pTo;

				while (true)
				{
					ImgTmp.Dilate(pTo, se);
					*pTo = *pTo & ImgBackup;
					if (*pTo == ImgTmp)
						break;
					ImgTmp = *pTo;
				}

				for (int m = 0; m < nHeight; m++)
				{
					for (int n = 0; n < nWidth; n++)
					{
						gray = pTo->GetGray(n, m);
						if (gray == 0)
							SetPixel(n, m, RGB(nConnRgn, nConnRgn, nConnRgn));
					}
				}
				nConnRgn++;
				if (nConnRgn > 255)
				{
					AfxMessageBox(L"目前该函数最多支持标注255个连通分量");
					i = nHeight;
					break;
				}
			}
		}
	}
}

void CImgProcess::Thining(CImgProcess* pTo)
{
	bool condition1, condition2, condition3, condition4;
	bool bModified = true;
	unsigned char neighbour[5][5];

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	while (bModified)
	{
		bModified = false;
		CImgProcess pic = *this;
		pic.InitPixels(255);

		for (int i = 2; i < nHeight - 2; i++)
		{
			for (int j = 2; j < nWidth - 2; j++)
			{
				condition1 = false;
				condition2 = false;
				condition3 = false;
				condition4 = false;

				BYTE data = GetPixel(j, i);
				if (data == 255)
					continue;

				for (int n = 0; n < 5; n++)
					for (int m = 0; m < 5; m++)
						neighbour[n][m] = (GetPixel(j - 2 + m, i - 2 + n) == 0);

				// condition1 test
				int count = neighbour[1][1] + neighbour[1][2] + neighbour[1][3]
					+ neighbour[2][1] + neighbour[2][3]
					+ neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
				if (count >= 2 && count <= 6)
					condition1 = true;

				// condition2 test
				count = 0;
				if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
					count++;
				if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
					count++;
				if (neighbour[2][1] == 0 && neighbour[3][1] == 1)
					count++;
				if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
					count++;
				if (neighbour[3][2] == 0 && neighbour[3][3] == 1)
					count++;
				if (neighbour[3][3] == 0 && neighbour[2][3] == 1)
					count++;
				if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
					count++;
				if (neighbour[1][3] == 0 && neighbour[1][2] == 1)
					count++;
				if (count == 1)
					condition2 = true;

				// condition3 test
				if (neighbour[1][2] * neighbour[2][1] * neighbour[2][3] == 0)
					condition3 = true;
				else
				{
					count = 0;
					if (neighbour[0][2] == 0 && neighbour[0][1] == 1)
						count++;
					if (neighbour[0][1] == 0 && neighbour[1][1] == 1)
						count++;
					if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
						count++;
					if (neighbour[2][1] == 0 && neighbour[2][2] == 1)
						count++;
					if (neighbour[2][2] == 0 && neighbour[2][3] == 1)
						count++;
					if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
						count++;
					if (neighbour[1][3] == 0 && neighbour[0][3] == 1)
						count++;
					if (neighbour[0][3] == 0 && neighbour[0][2] == 1)
						count++;
					if (count != 1)
						condition3 = true;
				}

				// condition4 test
				if (neighbour[1][2] * neighbour[2][1] * neighbour[3][2] == 0)
					condition4 = true;
				else
				{
					count = 0;
					if (neighbour[1][1] == 0 && neighbour[1][0] == 1)
						count++;
					if (neighbour[1][0] == 0 && neighbour[2][0] == 1)
						count++;
					if (neighbour[2][0] == 0 && neighbour[3][0] == 1)
						count++;
					if (neighbour[3][0] == 0 && neighbour[3][1] == 1)
						count++;
					if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
						count++;
					if (neighbour[3][2] == 0 && neighbour[2][2] == 1)
						count++;
					if (neighbour[2][2] == 0 && neighbour[1][2] == 1)
						count++;
					if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
						count++;
					if (count != 1)
						condition4 = true;
				}

				if (condition1 && condition2 && condition3 && condition4)
				{
					pic.SetPixel(j, i, RGB(255, 255, 255));
					bModified = true;
				}
				else
					pic.SetPixel(j, i, RGB(0, 0, 0));
			}
		}
		*this = pic;
	}
}

void CImgProcess::Convex(CImgProcess* pTo, BOOL constrain)
{
	int se1[3][3] = { {1, -1, -1}, {1, 0, -1}, {1, -1, -1} };
	int se2[3][3] = { {1, 1, 1,}, {-1, 0, -1}, {-1, -1, -1} };
	int se3[3][3] = { {-1, -1, 1}, {-1, 0, 1}, {-1, -1, 1} };
	int se4[3][3] = { {-1, -1, -1}, {-1, 0, -1}, {1, 1, 1} };

	pTo->InitPixels(255);

	CImgProcess tmp1 = *this;
	while (true)
	{
		tmp1.Erode(pTo, se1);
		*pTo = *pTo | tmp1;
		if (*pTo == tmp1)
			break;
		tmp1 = *pTo;
	}

	CImgProcess tmp2 = *this;
	while (true)
	{
		tmp2.Erode(pTo, se2);
		*pTo = *pTo | tmp2;
		if (*pTo == tmp2)
			break;
		tmp2 = *pTo;
	}

	CImgProcess tmp3 = *this;
	while (true)
	{
		tmp3.Erode(pTo, se3);
		*pTo = *pTo | tmp3;
		if (*pTo == tmp3)
			break;
		tmp3 = *pTo;
	}

	CImgProcess tmp4 = *this;
	while (true)
	{
		tmp4.Erode(pTo, se4);
		*pTo = *pTo | tmp4;
		if (*pTo == tmp4)
			break;
		tmp4 = *pTo;
	}

	pTo->InitPixels(255);

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			if (tmp1.GetGray(j, i) == 0 || tmp2.GetGray(j, i) == 0 ||
				tmp3.GetGray(j, i) == 0 || tmp3.GetGray(j, i) == 0)
			{
				pTo->SetPixel(j, i, RGB(0, 0, 0));
			}
		}
	}

	int nTop = nHeight;
	int nBottom = 0;
	int nLeft = nWidth;
	int nRight = 0;

	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			if (GetGray(j, i) == 0)
			{
				if (j > nRight)
					nRight = j;
				if (j < nLeft)
					nLeft = j;
				if (i > nBottom)
					nBottom = i;
				if (i < nTop)
					nTop = i;
			}
		}
	}

	if (constrain)
	{
		for (int i = 0; i < nHeight; i++)
			for (int j = 0; j < nWidth; j++)
				if (i < nTop || i > nBottom || j < nLeft || j > nRight)
					pTo->SetPixel(j, i, RGB(255, 255, 255));
	}
}

void CImgProcess::GrayDilate(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	for (int i = nTempMY; i < nHeight - nTempH + nTempMY; i++)
	{
		for (int j = nTempMX; j < nWidth - nTempW + nTempMX; j++)
		{
			BYTE maxVal = 0;
			for (int m = 0; m < nTempH; m++)
			{
				for (int n = 0; n < nTempW; n++)
				{
					if (se[m][n] == 1)
					{
						BYTE pix = GetGray(j - nTempMX + n, i - nTempMY + m);
						if (pix > maxVal)
							maxVal = pix;
					}
				}
			}
			pTo->SetPixel(j, i, RGB(maxVal, maxVal, maxVal));
		}
	}
}

void CImgProcess::GrayErode(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	for (int i = nTempMY; i < nHeight - nTempH + nTempMY; i++)
	{
		for (int j = nTempMX; j < nWidth - nTempW + nTempMX; j++)
		{
			BYTE minVal = UINT8_MAX;
			for (int m = 0; m < nTempH; m++)
			{
				for (int n = 0; n < nTempW; n++)
				{
					if (se[m][n] == 1)
					{
						BYTE pix = GetGray(j - nTempMX + n, i - nTempMY + m);
						if (pix < minVal)
							minVal = pix;
					}
				}
			}
			pTo->SetPixel(j, i, RGB(minVal, minVal, minVal));
		}
	}
}

void CImgProcess::GrayOpen(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se)
{
	GrayErode(pTo, nTempH, nTempW, nTempMY, nTempMX, se);
	CImgProcess imgTmp = *pTo;
	imgTmp.GrayDilate(pTo, nTempH, nTempW, nTempMY, nTempMX, se);
}

void CImgProcess::GrayClose(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se)
{
	GrayDilate(pTo, nTempH, nTempW, nTempMY, nTempMX, se);
	CImgProcess imgTmp = *pTo;
	imgTmp.GrayErode(pTo, nTempH, nTempW, nTempMY, nTempMX, se);
}

void CImgProcess::TopHat(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se)
{
	GrayOpen(pTo, nTempH, nTempW, nTempMY, nTempMX, se);
	*pTo = (*this) - *pTo;
}