#pragma once
#include "Img.h"
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

	void Threshold(CImgProcess* pTo, BYTE nThres);
};

