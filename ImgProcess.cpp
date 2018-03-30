#include "stdafx.h"
#include "ImgProcess.h"

#include <math.h>
#include <vector>

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

	Template(&img1, 3, 3, 1, 1, Template_Laplacian2, 1);
	
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

void CImgProcess::IFFT(std::complex<double>* TD, std::complex<double>* FD, int r)
{
	LONG count;

	std::complex<double>* X;

	count = 1 << r;

	X = new std::complex<double>[count];

	memcpy(X, TD, sizeof(std::complex<double>) * count);

	for (int i = 0; i < count; i++)
		X[i] = std::complex<double>(X[i].real(), -X[i].imag());

	FFT(X, FD, r);

	for (int i = 0; i < count; i++)
		FD[i] = std::complex<double>(FD[i].real() / count, -FD[i].imag() / count);

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

	if ((bExpand) && (w != GetWidthPixel()) && (h != GetHeight()))
	{
		w *= 2; wp++;
		h *= 2; hp++;
	}

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
			pOutput[i * w + j] = FD[i * w + j];

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