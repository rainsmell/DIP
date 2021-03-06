
// DIPView.cpp : CDIPView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DIP.h"
#endif

#include "DIPDoc.h"
#include "DIPView.h"
#include "DlgHist.h"
#include "DlgLinerPara.h"
#include "DlgLog.h"
#include "DlgGamma.h"
#include "DlgThreshold.h"
#include "DlgParLineTran.h"
#include "GeomMove.h"
#include "DlgGemoScale.h"
#include "DlgGemoRotate.h"
#include "DlgSmoothConfig.h"
#include "DlgFreqLPF.h"
#include "DlgGaussFilter.h"
#include "DlgInvRad.h"
#include "DlgHuffman.h"

#include <iostream>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDIPView

IMPLEMENT_DYNCREATE(CDIPView, CView)

BEGIN_MESSAGE_MAP(CDIPView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDIPView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_32771, &CDIPView::OnFileClean)
	ON_COMMAND(ID_32772, &CDIPView::OnViewIntensity)
	ON_COMMAND(ID_32773, &CDIPView::OnPointLiner)
	ON_COMMAND(ID_32774, &CDIPView::OnPointLog)
	ON_COMMAND(ID_32775, &CDIPView::OnPointGamma)
	ON_COMMAND(ID_32776, &CDIPView::OnThreshold)
	ON_COMMAND(ID_32777, &CDIPView::OnPointParLineTran)
	ON_COMMAND(ID_32778, &CDIPView::OnPointHistEq)
	ON_COMMAND(ID_32779, &CDIPView::OnPointHistst)
	ON_COMMAND(ID_32780, &CDIPView::OnGemoTran)
	ON_COMMAND(ID_32781, &CDIPView::OnGemoHorMirror)
	ON_COMMAND(ID_32782, &CDIPView::OnGemoVerMirror)
	ON_COMMAND(ID_32783, &CDIPView::OnGemoTranspose)
	ON_COMMAND(ID_32784, &CDIPView::OnGemoScale)
	ON_COMMAND(ID_32785, &CDIPView::OnGemoRotate)
	ON_COMMAND(ID_32786, &CDIPView::OnEnhaSmooth)
	ON_COMMAND(ID_32787, &CDIPView::OnEnhaMedian)
	ON_COMMAND(ID_32788, &CDIPView::OnEnhaAdaptiveMeidanFilter)
	ON_COMMAND(ID_32789, &CDIPView::OnEnhaGradSobel)
	ON_COMMAND(ID_32790, &CDIPView::OnEnhaGradLaplacian)
	ON_COMMAND(ID_32791, &CDIPView::OnEnhanceFilter)
	ON_COMMAND(ID_32792, &CDIPView::OnFreFilterFFT2)
	ON_COMMAND(ID_32793, &CDIPView::OnFreFilterIFFT2)
	ON_COMMAND(ID_32794, &CDIPView::OnFreqIdealLPF)
	ON_COMMAND(ID_32795, &CDIPView::OnFreqGaussFilter)
	ON_COMMAND(ID_32796, &CDIPView::OnFreqGaussHPF)
	ON_COMMAND(ID_32797, &CDIPView::OnFreqLaplace)
	ON_COMMAND(ID_32798, &CDIPView::OnAddUniform)
	ON_COMMAND(ID_32799, &CDIPView::OnAddGaussian)
	ON_COMMAND(ID_32800, &CDIPView::OnAddSlatPepper)
	ON_COMMAND(ID_32801, &CDIPView::OnAddRayleigh)
	ON_COMMAND(ID_32802, &CDIPView::OnInvTuihua)
	ON_COMMAND(ID_32803, &CDIPView::OnInvFilter)
	ON_COMMAND(ID_32804, &CDIPView::OnFreqWienerFilter)
	ON_COMMAND(ID_32805, &CDIPView::OnColorRGB2CMY)
	ON_COMMAND(ID_32806, &CDIPView::OnColorRGB2HSI)
	ON_COMMAND(ID_32807, &CDIPView::OnColorHSI2RGB)
	ON_COMMAND(ID_32808, &CDIPView::OnColorRGB2HSV)
	ON_COMMAND(ID_32809, &CDIPView::OnColorHSV2RGB)
	ON_COMMAND(ID_32810, &CDIPView::OnColorRGB2YUV)
	ON_COMMAND(ID_32811, &CDIPView::OnColorYUV2RGB)
	ON_COMMAND(ID_32812, &CDIPView::OnColorRGB2YIQ)
	ON_COMMAND(ID_32813, &CDIPView::OnColorYIQ2RGB)
	ON_COMMAND(ID_32814, &CDIPView::OnDctAll)
	ON_COMMAND(ID_32815, &CDIPView::OnIdctAll)
	ON_COMMAND(ID_32816, &CDIPView::OnHuffcode)
	ON_COMMAND(ID_32817, &CDIPView::OnErode)
	ON_COMMAND(ID_32818, &CDIPView::OnDilate)
	ON_COMMAND(ID_32819, &CDIPView::OnMorphOpen)
	ON_COMMAND(ID_32820, &CDIPView::OnMorphClose)
	ON_COMMAND(ID_32821, &CDIPView::OnGetEdge)
	ON_COMMAND(ID_32822, &CDIPView::OnConvex)
	ON_COMMAND(ID_32823, &CDIPView::OnThining)
	ON_COMMAND(ID_32824, &CDIPView::OnLabelConnRgn)
	ON_COMMAND(ID_32825, &CDIPView::OnGrayErode)
	ON_COMMAND(ID_32826, &CDIPView::OnGrayDilate)
	ON_COMMAND(ID_32827, &CDIPView::OnGrayOpen)
	ON_COMMAND(ID_32828, &CDIPView::OnGrayClose)
	ON_COMMAND(ID_32829, &CDIPView::OnTopHat)
	ON_COMMAND(ID_32830, &CDIPView::OnEdgeSobel)
	ON_COMMAND(ID_32831, &CDIPView::OnEdgeLoG)
	ON_COMMAND(ID_32833, &CDIPView::OnEdgePrewitt)
	ON_COMMAND(ID_32832, &CDIPView::OnEdgeCanny)
END_MESSAGE_MAP()

// CDIPView 构造/析构

CDIPView::CDIPView()
{
	// TODO: 在此处添加构造代码

}

CDIPView::~CDIPView()
{
}

BOOL CDIPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDIPView 绘制

void CDIPView::OnDraw(CDC* pDC)
{
	CDIPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	BeginWaitCursor();

	// TODO: 在此处为本机数据添加绘制代码
	if (pDoc->m_Image.IsValidate())
	{
		CPalette* pOldPalette;
		CPalette* pPalette = pDoc->GetDocPalette();

		if (pPalette != NULL)
		{
			pOldPalette = pDC->SelectPalette(pPalette, FALSE);
			pDC->RealizePalette();
		}

		pDoc->m_Image.Draw(pDC);

		if (pPalette != NULL)
			pDC->SelectPalette(pOldPalette, FALSE);
	}
	EndWaitCursor();
}


// CDIPView 打印


void CDIPView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDIPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDIPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDIPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CDIPView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDIPView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDIPView 诊断

#ifdef _DEBUG
void CDIPView::AssertValid() const
{
	CView::AssertValid();
}

void CDIPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDIPDoc* CDIPView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDIPDoc)));
	return (CDIPDoc*)m_pDocument;
}
#endif //_DEBUG


// CDIPView 消息处理程序


void CDIPView::OnFileClean()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImg imgInput = pDoc->m_Image;

	if (imgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理!");
		return;
	}

	imgInput.InitPixels(255);
	pDoc->m_Image = imgInput;
	pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnViewIntensity()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();
	CImgProcess imgInput = pDoc->m_Image;
	double hist[256];

	BeginWaitCursor();

	imgInput.GenHist(hist);

	CDlgHist dlg;
	dlg.m_pdHist = hist;

	if (dlg.DoModal() != IDOK)
		return;

	pDoc->UpdateAllViews(NULL);
	EndWaitCursor();
}


void CDIPView::OnPointLiner()
{
	// TODO: Add your command handler code here

	CDIPDoc* pDoc = GetDocument();

	CImgProcess imgInput = pDoc->m_Image;

	if (imgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CDlgLinerPara dlg;

	double fA, fB;

	dlg.m_fA = 2.0;
	dlg.m_fB = -128.0;

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	fA = dlg.m_fA;
	fB = dlg.m_fB;

	BeginWaitCursor();

	CImgProcess imgOutput = imgInput;

	imgInput.LinTran(&imgOutput, fA, fB);

	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(TRUE);

	pDoc->UpdateAllViews(NULL);

	EndWaitCursor();
}


void CDIPView::OnPointLog()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess imgInput = pDoc->m_Image;

	if (imgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	DlgLog dlg;
	dlg.m_dC = 20;

	if (dlg.DoModal() != IDOK)
		return;

	CImgProcess imgOutput = imgInput;

	imgInput.LogTran(&imgOutput, dlg.m_dC);

	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);

}


void CDIPView::OnPointGamma()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();
	CImgProcess imgInput = pDoc->m_Image;

	if (imgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CDlgGamma dlg;
	dlg.m_dEsp = 0;
	dlg.m_dGamma = 1;

	if (dlg.DoModal() != IDOK)
		return;

	CImgProcess imgOutput = imgInput;

	imgInput.GammaTran(&imgOutput, dlg.m_dGamma, dlg.m_dEsp);

	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnThreshold()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess imgInput = pDoc->m_Image;

	CDlgThreshold dlg;
	dlg.m_dThre = 128;

	if (dlg.DoModal() != IDOK)
		return;

	CImgProcess imgOutput = imgInput;

	imgInput.Threshold(&imgOutput, dlg.m_dThre);

	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnPointParLineTran()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess imgInput = pDoc->m_Image;

	if (imgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CDlgParLineTran dlg;
	dlg.m_dX1 = 64;
	dlg.m_dY1 = 32;
	dlg.m_dX2 = 192;
	dlg.m_dY2 = 225;

	if (dlg.DoModal() != IDOK)
		return;

	CImgProcess imgOutput = imgInput;

	imgInput.ParLinTran(&imgOutput, dlg.m_dX1, dlg.m_dX2, dlg.m_dY1, dlg.m_dY2);

	pDoc->m_Image = imgOutput;
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnPointHistEq()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess imgInput = pDoc->m_Image;

	CImgProcess imgOutput = imgInput;

	imgInput.Histeq(&imgOutput);

	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnPointHistst()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess imgInput = pDoc->m_Image;

	if (imgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess* pStdImage = new CImgProcess();

	CString filePath;

	CFileDialog openDlg(TRUE, 0, 0, 4 | 2, L"Bitmap Files (*.bmp)||");
	if (openDlg.DoModal() == IDOK)
		filePath = openDlg.GetPathName();
	else
		return;

	pStdImage->AttachFromFile(filePath);
	if (pStdImage->m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	double pdStdHist[256];
	pStdImage->GenHist(pdStdHist);
	CDlgHist histDlg;
	histDlg.m_pdHist = pdStdHist;
	if (histDlg.DoModal() != IDOK)
		return;

	CImgProcess imgOutput = imgInput;
	imgInput.Histst(&imgOutput, pdStdHist);

	pDoc->m_Image = imgOutput;

	delete pStdImage;

	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnGemoTran()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	CGeomMove dlg;

	dlg.m_dX = 0;
	dlg.m_dY = 0;

	if (dlg.DoModal() != IDOK)
		return;

	CImgProcess ImgOutput = ImgInput;

	ImgInput.InMove(&ImgOutput, dlg.m_dX, dlg.m_dY);

	pDoc->m_Image = ImgOutput;

	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnGemoHorMirror()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;
	CImgProcess ImgOutput = ImgInput;

	ImgInput.HorMirror(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnGemoVerMirror()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;
	CImgProcess ImgOutput = ImgInput;

	ImgInput.VerMirror(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnGemoTranspose()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;
	CImgProcess ImgOutput = ImgInput;

	ImgInput.Transpose(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnGemoScale()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;
	CImgProcess ImgOutput = ImgInput;

	CDlgGemoScale dlg;
	dlg.m_dTimes = 0.5;

	if (dlg.DoModal() != IDOK)
		return;

	ImgInput.Scale(&ImgOutput, dlg.m_dTimes);

	pDoc->m_Image = ImgOutput;
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnGemoRotate()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();
	CImgProcess ImgInput = pDoc->m_Image;
	CImgProcess ImgOutput = ImgInput;

	CDlgGemoRotate dlg;
	dlg.m_fAng = 30.0f;

	if (dlg.DoModal() != IDOK)
		return;

	ImgInput.Rotate(&ImgOutput, dlg.m_fAng);

	pDoc->m_Image = ImgOutput;
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnEnhaSmooth()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	// 模板元素数组赋初值（默认为平均模板）
	FLOAT	aValue[25] = { 1.0, 1.0, 1.0, 0.0, 0.0,
		1.0, 1.0, 1.0, 0.0, 0.0,
		1.0, 1.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, };

	// 创建对话框
	CDlgSmoothConfig dlgPara;

	// 初始化对话框变量值
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH = 3;
	dlgPara.m_iTempW = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC = (FLOAT)(1.0 / 9.0);
	dlgPara.m_fpArray = aValue;

	if (dlgPara.DoModal() != IDOK)
		return;

	int	nTempH;	// 模板高度
	int	nTempW; // 模板宽度
	FLOAT fTempC;// 模板系数
	int		nTempMX;// 模板中心元素X坐标
	int		nTempMY;// 模板中心元素Y坐标

	nTempH = dlgPara.m_iTempH;
	nTempW = dlgPara.m_iTempW;
	nTempMX = dlgPara.m_iTempMX;
	nTempMY = dlgPara.m_iTempMY;
	fTempC = dlgPara.m_fTempC;



	BeginWaitCursor();

	//ImgInput.Template(&ImgOutput, dlgPara.m_iTempH, dlgPara.m_iTempW,
		//dlgPara.m_iTempMY, dlgPara.m_iTempMY, aValue, dlgPara.m_fTempC);

	ImgInput.Template(&ImgOutput, nTempH, nTempW, nTempMY, nTempMX, aValue, fTempC);

	pDoc->m_Image = ImgOutput;
	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}
	pDoc->UpdateAllViews(NULL);

	EndWaitCursor();
}


void CDIPView::OnEnhaMedian()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	ImgInput.MedianFilter(&ImgOutput, 3, 3, 1, 1);
	
	pDoc->m_Image = ImgOutput;
	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}
	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnEnhaAdaptiveMeidanFilter()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	ImgInput.MedianFilter(&ImgOutput, 3, 3, 1, 1);

	pDoc->m_Image = ImgOutput;
	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}
	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnEnhaGradSobel()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	ImgInput.FilterSobel(&ImgOutput);

	pDoc->m_Image = ImgOutput;
	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}
	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnEnhaGradLaplacian()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	ImgInput.FilterLaplacian(&ImgOutput);

	pDoc->m_Image = ImgOutput;
	
	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}
		
	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnEnhanceFilter()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	ImgInput.EnhanceFilter(&ImgOutput, 1.8, 3, 3, 1, 1, Template_Laplacian2, 1);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnFreFilterFFT2()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int w = ImgInput.GetWidthPixel();
	int h = ImgInput.GetHeight();

	std::complex<double>* dft = new std::complex<double>[w * h];
	ImgOutput.FFT2(&ImgOutput, FALSE, dft, 0);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);

	delete[] dft;
}


void CDIPView::OnFreFilterIFFT2()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int w = ImgInput.GetWidthPixel();
	int h = ImgInput.GetHeight();

	std::complex<double>* dft = new std::complex<double>[w * h];
	ImgOutput.FFT2(&ImgOutput, FALSE, dft, 0);
	ImgOutput.IFFT2(&ImgOutput, dft, 0, 0, ImgOutput.GetHeight(), ImgOutput.GetWidthPixel());

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);

	delete[] dft;
}


void CDIPView::OnFreqIdealLPF()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int w = ImgOutput.GetFreqWidth(ImgOutput.GetWidthPixel(), TRUE);
	int h = ImgOutput.GetFreqHeight(ImgOutput.GetHeight(), TRUE);

	double* dpFilter = new double[w * h];

	CDlgFreqLPF dlg;
	dlg.m_dFreq = 20;

	if (dlg.DoModal() != IDOK)
	{
		delete[] dpFilter;
		return;
	}

	ImgOutput.FreqIdealLPF(dpFilter, dlg.m_dFreq);
	ImgOutput.FreqFilter(&ImgOutput, dpFilter, 0);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);

	delete[] dpFilter;
}


void CDIPView::OnFreqGaussFilter()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int w = ImgOutput.GetFreqWidth(ImgOutput.GetWidthPixel(), FALSE);
	int h = ImgOutput.GetFreqHeight(ImgOutput.GetHeight(), FALSE);

	double* dpFilter = new double[w * h];

	CDlgGaussFilter dlg;
	dlg.m_dSigma = 20;

	if (dlg.DoModal() != IDOK)
	{
		delete[] dpFilter;
		return;
	}

	ImgOutput.FreqGaussLPF(dpFilter, dlg.m_dSigma);
	ImgOutput.FreqFilter(&ImgOutput, dpFilter, 0);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);

	delete[] dpFilter;
}


void CDIPView::OnFreqGaussHPF()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int w = ImgOutput.GetFreqWidth(ImgOutput.GetWidthPixel(), FALSE);
	int h = ImgOutput.GetFreqHeight(ImgOutput.GetHeight(), FALSE);

	double* dpFilter = new double[w * h];

	CDlgGaussFilter dlg;
	dlg.m_dSigma = 20;

	if (dlg.DoModal() != IDOK)
	{
		delete[] dpFilter;
		return;
	}

	ImgOutput.FreqGaussHPF(dpFilter, dlg.m_dSigma);
	ImgOutput.FreqFilter(&ImgOutput, dpFilter, 0);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);

	delete[] dpFilter;
}


void CDIPView::OnFreqLaplace()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int w = ImgOutput.GetFreqWidth(ImgOutput.GetWidthPixel(), FALSE);
	int h = ImgOutput.GetFreqHeight(ImgOutput.GetHeight(), FALSE);

	double* dpFilter = new double[w * h];

	ImgOutput.FreqLaplace(dpFilter);
	ImgOutput.FreqFilter(&ImgOutput, dpFilter, 0);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);

	delete[] dpFilter;
}


void CDIPView::OnAddUniform()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	ImgInput.AddUniform(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);

}

void CDIPView::OnAddGaussian()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	ImgInput.AddGaussian(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnAddSlatPepper()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	ImgInput.AddSlat_Pepper(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnAddRayleigh()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	ImgInput.AddRayleigh(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnInvTuihua()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int w = ImgOutput.GetFreqWidth(ImgOutput.GetWidthPixel(), FALSE);
	int h = ImgOutput.GetFreqHeight(ImgOutput.GetHeight(), FALSE);
	double* dFilter = new double[w * h];
	ImgOutput.FreqInvTuihua(dFilter);

	//if (AllocConsole())
	//{
	//	freopen("CONOUT$", "w", stdout);
	//	for (int i = 0; i < h; i += 10)
	//	{
	//		for (int j = 0; j < w; j += 10)
	//		{
	//			std::cout << int(dFilter[i * w + j] + 0.2) << ' ';
	//		}
	//		std::cout << std::endl;
	//	}
	//}
	//FreeConsole();

	ImgOutput.FreqFilter(&ImgOutput, dFilter, 0);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);

	delete[] dFilter;
}

void CDIPView::OnInvFilter()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	CDlgInvRad dlg;

	dlg.m_nRad = 75;

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	int w = ImgOutput.GetFreqWidth(ImgOutput.GetWidthPixel(), FALSE);
	int h = ImgOutput.GetFreqHeight(ImgOutput.GetHeight(), FALSE);
	double* dFilter = new double[w * h];
	ImgInput.FreqInvFilter(dFilter, dlg.m_nRad);
	ImgInput.FreqFilter(&ImgOutput, dFilter, 0);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);

	delete[] dFilter;
}


void CDIPView::OnFreqWienerFilter()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int w = ImgOutput.GetFreqWidth(ImgOutput.GetWidthPixel(), FALSE);
	int h = ImgOutput.GetFreqHeight(ImgOutput.GetHeight(), FALSE);
	double* dFilter = new double[w * h];
	ImgInput.FreqWienerFilter(dFilter, 78, 0.05);
	ImgInput.FreqFilter(&ImgOutput, dFilter, 0);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);

	delete[] dFilter;
}


void CDIPView::OnColorRGB2CMY()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;
	CImgProcess ImgOutput = ImgInput;

	ImgInput.RGB2CMY(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnColorRGB2HSI()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;
	CImgProcess ImgOutput = ImgInput;

	ImgInput.RGB2HSI(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnColorHSI2RGB()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;
	CImgProcess ImgOutput = ImgInput;

	ImgInput.HSI2RGB(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnColorRGB2HSV()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;
	CImgProcess ImgOutput = ImgInput;

	ImgInput.RGB2HSV(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnColorHSV2RGB()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;
	CImgProcess ImgOutput = ImgInput;

	ImgInput.HSV2RGB(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnColorRGB2YUV()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;
	CImgProcess ImgOutput = ImgInput;

	ImgInput.RGB2YUV(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnColorYUV2RGB()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;
	CImgProcess ImgOutput = ImgInput;

	ImgInput.YUV2RGB(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnColorRGB2YIQ()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;
	CImgProcess ImgOutput = ImgInput;

	ImgInput.RGB2YIQ(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnColorYIQ2RGB()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;
	CImgProcess ImgOutput = ImgInput;

	ImgInput.YIQ2RGB(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnDctAll()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	if (ImgInput.GetHeight() % 8 != 0 || ImgInput.GetWidthPixel() % 8 != 0)
	{
		AfxMessageBox(L"图像宽高不是8的整数倍，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	ImgInput.DCT_ALL(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnIdctAll()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	if (ImgInput.GetHeight() % 8 != 0 || ImgInput.GetWidthPixel() % 8 != 0)
	{
		AfxMessageBox(L"图像宽高不是8的整数倍，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	ImgInput.IDCT_ALL(&ImgOutput);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnHuffcode()
{
	// TODO: Add your command handler code here
	CDlgHuffman dlg;

	dlg.DoModal();
}


void CDIPView::OnErode()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int kernel[3][3] = { 1,1,1,
		1,1,1,
		1,1,1, };

	ImgInput.Erode(&ImgOutput, kernel);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnDilate()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int kernel[3][3] = { 1,1,1,
		1,1,1,
		1,1,1, };

	ImgInput.Dilate(&ImgOutput, kernel);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnMorphOpen()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int kernel[3][3] = { 1,1,1,
		1,1,1,
		1,1,1, };

	ImgInput.Open(&ImgOutput, kernel);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnMorphClose()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int kernel[3][3] = { 1,1,1,
		1,1,1,
		1,1,1, };

	ImgInput.Close(&ImgOutput, kernel);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnGetEdge()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int kernel[3][3] = { 1,1,1,
		1,1,1,
		1,1,1, };

	ImgInput.Erode(&ImgOutput, kernel);

	CImgProcess ImgEdge = *(CImgProcess*)&(ImgOutput - ImgInput);

	pDoc->m_Image = ImgEdge;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnConvex()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgInvert = *(CImgProcess *)&(!ImgInput);
	CImgProcess ImgOutput = ImgInvert;

	ImgInvert.Convex(&ImgOutput, true);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnThining()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgInvert = *(CImgProcess *)&(!ImgInput);
	CImgProcess ImgOutput = ImgInvert;

	ImgInvert.Thining(&ImgOutput);

	pDoc->m_Image = ImgInvert;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnLabelConnRgn()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgInvert = *(CImgProcess *)&(!ImgInput);
	CImgProcess ImgOutput = ImgInvert;

	ImgInvert.LabelConnRgn(&ImgOutput, 8);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnGrayErode()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int** se = new int*[3];

	for (int i = 0; i < 3; i++)
	{
		se[i] = new int[3];
		std::fill(&se[i][0], &se[i][2], 1);
	}

	ImgInput.GrayErode(&ImgOutput, 3, 3, 1, 1, se);

	for (int i = 0; i < 3; i++)
		delete [] se[i];
	delete[] se;

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnGrayDilate()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int** se = new int*[3];

	for (int i = 0; i < 3; i++)
	{
		se[i] = new int[3];
		std::fill(&se[i][0], &se[i][2], 1);
	}

	ImgInput.GrayDilate(&ImgOutput, 3, 3, 1, 1, se);

	for (int i = 0; i < 3; i++)
		delete[] se[i];
	delete[] se;

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnGrayOpen()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int** se = new int*[3];

	for (int i = 0; i < 3; i++)
	{
		se[i] = new int[3];
		std::fill(&se[i][0], &se[i][2], 1);
	}

	ImgInput.GrayOpen(&ImgOutput, 3, 3, 1, 1, se);

	for (int i = 0; i < 3; i++)
		delete[] se[i];
	delete[] se;

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnGrayClose()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}
	
	CImgProcess ImgOutput = ImgInput;

	int** se = new int*[3];

	for (int i = 0; i < 3; i++)
	{
		se[i] = new int[3];
		std::fill(&se[i][0], &se[i][2], 1);
	}

	ImgInput.GrayClose(&ImgOutput, 3, 3, 1, 1, se);

	for (int i = 0; i < 3; i++)
		delete[] se[i];
	delete[] se;

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnTopHat()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	int** se = new int*[3];

	for (int i = 0; i < 3; i++)
	{
		se[i] = new int[3];
		std::fill(&se[i][0], &se[i][2], 1);
	}

	ImgInput.TopHat(&ImgOutput, 3, 3, 1, 1, se);

	for (int i = 0; i < 3; i++)
		delete[] se[i];
	delete[] se;

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnEdgeSobel()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	ImgInput.EdgeSobel(&ImgOutput, 100, CImgProcess::Edge_ALL, true, false);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnEdgeLoG()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	ImgInput.EdgeLoG(&ImgOutput, 100, 0, true);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnEdgePrewitt()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImgProcess ImgInput = pDoc->m_Image;

	if (ImgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"不是8-bpp灰度图像，无法处理！");
		return;
	}

	CImgProcess ImgOutput = ImgInput;

	ImgInput.EdgePrewitt(&ImgOutput, 100, CImgProcess::Edge_ALL, true, false);

	pDoc->m_Image = ImgOutput;

	if (!pDoc->IsModified())
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->SetTitle(pDoc->GetTitle() + L"*");
	}

	pDoc->UpdateAllViews(NULL);
}


void CDIPView::OnEdgeCanny()
{
	// TODO: Add your command handler code here
}
