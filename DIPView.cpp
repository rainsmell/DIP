
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
