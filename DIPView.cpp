
// DIPView.cpp : CDIPView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
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

// CDIPView ����/����

CDIPView::CDIPView()
{
	// TODO: �ڴ˴���ӹ������

}

CDIPView::~CDIPView()
{
}

BOOL CDIPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CDIPView ����

void CDIPView::OnDraw(CDC* pDC)
{
	CDIPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	BeginWaitCursor();

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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


// CDIPView ��ӡ


void CDIPView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDIPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CDIPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CDIPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CDIPView ���

#ifdef _DEBUG
void CDIPView::AssertValid() const
{
	CView::AssertValid();
}

void CDIPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDIPDoc* CDIPView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDIPDoc)));
	return (CDIPDoc*)m_pDocument;
}
#endif //_DEBUG


// CDIPView ��Ϣ�������


void CDIPView::OnFileClean()
{
	// TODO: Add your command handler code here
	CDIPDoc* pDoc = GetDocument();

	CImg imgInput = pDoc->m_Image;

	if (imgInput.m_pBMIH->biBitCount != 8)
	{
		AfxMessageBox(L"����8-bpp�Ҷ�ͼ���޷�����!");
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
		AfxMessageBox(L"����8-bpp�Ҷ�ͼ���޷�����");
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
		AfxMessageBox(L"����8-bpp�Ҷ�ͼ���޷�����");
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
		AfxMessageBox(L"����8-bpp�Ҷ�ͼ���޷�����");
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
