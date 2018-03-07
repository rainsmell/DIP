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
	BOOL LinTran(CImgProcess* pTo, double dFa, double dFb);
};

