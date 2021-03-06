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
	typedef enum { Edge_ALL, Edge_H, Edge_V, Edge_CW, Edge_CCW }Edge_t;

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
		int nTempMY, int nTempMX, FLOAT *pfArray, FLOAT fCoef); //�����˲�

	// һά�Ͷ�ά���ٸ���Ҷ�任
	void FFT(std::complex<double>* TD, std::complex<double>* FD, int r);
	void IFFT(std::complex<double>* TD, std::complex<double>* FD, int r);
	void FFT2(CImgProcess* pTo, BOOL bExpand, std::complex<double>* pOutput, BYTE bFillColor);
	void IFFT2(CImgProcess* pTo, std::complex<double>* pDFT, int iOutH, int iOutW, int iHeight, int iWidth);

	int GetFreqWidth(int width, BOOLEAN bPadding);
	int GetFreqHeight(int height, BOOLEAN bPadding);
	
	// Ƶ���˲�������pdFilterΪƵ���˲���
	void FreqFilter(CImgProcess* pTo, double* pdFilter, BYTE fillColor);

	// �����ͨ�˲���
	void FreqIdealLPF(double* pdFilter, int nFreq);
	
	// ��˹��ͨ�˲���
	void FreqGaussLPF(double* pdFilter, int nSigma);
	// ��˹��ͨ�˲���
	void FreqGaussHPF(double* pdFilter, int nSigma);
	// ������˹�˲���
	void FreqLaplace(double* pdFilter);

	void AddUniform(CImgProcess* pTo);
	void AddGaussian(CImgProcess* pTo);
	void AddSlat_Pepper(CImgProcess* pTo);
	void AddRayleigh(CImgProcess* pTo);

	void FreqInvTuihua(double* pdFilter);
	void FreqInvFilter(double* pdFilter, int nRad);
	void FreqWienerFilter(double* pdFilter, int nRad, double k);

	void RGB2CMY(CImgProcess* pTo);
	void RGB2HSI(CImgProcess* pTo);
	void HSI2RGB(CImgProcess* pTo);
	void HSV2RGB(CImgProcess *pTo);
	void RGB2HSV(CImgProcess *pTo);
	void YUV2RGB(CImgProcess *pTo);
	void RGB2YUV(CImgProcess *pTo);
	void YIQ2RGB(CImgProcess *pTo);
	void RGB2YIQ(CImgProcess *pTo);

	// ͼ��ѹ������㷨
	void dct8x8(double* in, double* out);
	void idct8x8(double* in, double* out);
	void quant(double* in, int* out);
	void iquant(int* in, double* out);
	void DCT_ALL(CImgProcess* pTo);
	void IDCT_ALL(CImgProcess* pTo);

	// ��̬ѧ���
	void Erode(CImgProcess* pTo, int se[3][3]);
	void Dilate(CImgProcess* pTo, int se[3][3]);
	void Open(CImgProcess* pTo, int se[3][3]);
	void Close(CImgProcess* pTo, int se[3][3]);

	void LabelConnRgn(CImgProcess* pTo, int nConn);
	void Thining(CImgProcess* pTo);
	void Convex(CImgProcess* pTo, BOOL constrain);

	void GrayDilate(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se);
	void GrayErode(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se);
	void GrayOpen(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se);
	void GrayClose(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se);

	void TopHat(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se);

	// ��Ե���
	BOOL EdgeSobel(CImgProcess* pTo, BYTE bThre, Edge_t bEdgeType, BOOL bThinning, BOOL bGOnly);
	BOOL EdgePrewitt(CImgProcess* pTo, BYTE bThre, Edge_t bEdgeType, BOOL bThinning, BOOL bGOnly);
	void EdgeLoG(CImgProcess* pTo, BYTE bThre, double dSigma, BOOL bThinning);
	void EdgeCanny(CImgProcess* pTo, BYTE bThreL, BYTE bThreH, BOOL bThinning);

	void Threshold(CImgProcess* pTo, BYTE nThres);
	int DetectThreshould(int nMaxIter, int& nDiffRet);

};

