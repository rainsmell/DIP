#pragma once
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
	

	void Threshold(CImgProcess* pTo, BYTE nThres);
};

