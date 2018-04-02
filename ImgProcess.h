#pragma once
#include <complex>
#include "Img.h"

extern float Template_Log[];
extern float Template_VSobel[];
extern float Template_HSobel[];
extern float Template_Smooth_Gauss[];
extern float Template_Smooth_Avg[];
extern float Template_Laplacian1[];
extern float Template_Laplacian2[];

class CImgProcess :
	public CImg
{
public:
	CImgProcess();
	~CImgProcess();

	CImgProcess& operator=(CImg& img);

	BOOL GenHist(double* hist, int n = 256);
	BOOL Histeq(CImgProcess* pTo);
	BOOL Histst(CImgProcess* pTo, double* pdStdHist);
	BOOL Histst(CImgProcess* pTo, CImgProcess* pStd);
	BOOL LinTran(CImgProcess* pTo, double dFa, double dFb);
	BOOL LogTran(CImgProcess* pTo, double dC);
	BOOL GammaTran(CImgProcess* pTo, double gamma, double comp);
	BOOL ParLinTran(CImgProcess* pTo, BYTE x1, BYTE x2, BYTE y1, BYTE y2);

	void InMove(CImgProcess* pTo, int x, int y);
	void Scale(CImgProcess* pTo, double times);
	void Rotate(CImgProcess* pTo, float ang);
	void HorMirror(CImgProcess* pTo);
	void VerMirror(CImgProcess* pTo);
	void Transpose(CImgProcess* pTo);

	int InterpBilinear(double x, double y);

	void Template(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, FLOAT *pfArray, FLOAT fCoef);
	int GetMedianValue(int* pArrGray, int len);
	void MedianFilter(CImgProcess* pTo, int nFilterH, int nFilterW, int nFilterMY, int nFilterMX);
	void AdaptiveMedianFilter(CImgProcess* pTo, int nFilterH, int nFilterW, int nFilterMY, int nFilterMX);
	void FilterSobel(CImgProcess* pTo);
	void FilterLaplacian(CImgProcess* pTo);
	void EnhanceFilter(CImgProcess *pTo, double dProportion,
		int nTempH, int nTempW,
		int nTempMY, int nTempMX, FLOAT *pfArray, FLOAT fCoef); //提升滤波

	// 一维和二维快速傅里叶变换
	void FFT(std::complex<double>* TD, std::complex<double>* FD, int r);
	void IFFT(std::complex<double>* TD, std::complex<double>* FD, int r);
	void FFT2(CImgProcess* pTo, BOOL bExpand, std::complex<double>* pOutput, BYTE bFillColor);
	void IFFT2(CImgProcess* pTo, std::complex<double>* pDFT, int iOutH, int iOutW, int iHeight, int iWidth);
	
	// 频域滤波函数，pdFilter为频域滤波器
	void FreqFilter(CImgProcess* pTo, double* pdFilter, BYTE fillColor);

	// 理想低通滤波器
	void FreqIdealLPF(double* pdFilter, int nFreq);
	
	// 高斯低通滤波器
	void FreqGaussLPF(double* pdFilter, int nSigma);

	void Threshold(CImgProcess* pTo, BYTE nThres);
};

