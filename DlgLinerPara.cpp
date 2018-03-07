// DlgLinerPara.cpp : implementation file
//

#include "stdafx.h"
#include "DIP.h"
#include "DlgLinerPara.h"
#include "afxdialogex.h"


// CDlgLinerPara dialog

IMPLEMENT_DYNAMIC(CDlgLinerPara, CDialogEx)

CDlgLinerPara::CDlgLinerPara(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LinerPara, pParent)
	, m_fA(0)
	, m_fB(0)
{

}

CDlgLinerPara::~CDlgLinerPara()
{
}

// 对话框创建和销毁都会调用该函数
void CDlgLinerPara::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_A, m_fA);
	DDX_Text(pDX, IDC_EDIT_B, m_fB);
}


BEGIN_MESSAGE_MAP(CDlgLinerPara, CDialogEx)
	ON_EN_KILLFOCUS(IDC_EDIT_A, &CDlgLinerPara::OnEnKillfocusEditA)
	ON_EN_KILLFOCUS(IDC_EDIT_B, &CDlgLinerPara::OnEnKillfocusEditB)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CDlgLinerPara message handlers


// 当编辑器A失去焦点时触发该事件
void CDlgLinerPara::OnEnKillfocusEditA()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); // 触发DoDataExchange

	InvalidateRect(m_MouseRect, TRUE);
}


void CDlgLinerPara::OnEnKillfocusEditB()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	InvalidateRect(m_MouseRect, TRUE);
}


BOOL CDlgLinerPara::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CWnd* pWnd = GetDlgItem(IDC_STATIC);

	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);

	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);

	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;

	m_MouseRect.top += 25;
	m_MouseRect.left += 10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;

	m_bIsDraging = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgLinerPara::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	// point应该是窗口内坐标
	if (m_MouseRect.PtInRect(point))
	{
		m_p1 = point;

		m_p1.x = m_p1.x - m_MouseRect.left + 10;
		m_p1.y = m_p1.y - m_MouseRect.top + 25;

		m_bIsDraging = TRUE;

		m_bDrawed = FALSE;

		::SetCursor(::LoadCursor(NULL, IDC_CROSS));

		SetCapture();// 确保鼠标到窗口外面也可以接收到消息
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgLinerPara::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDlgLinerPara::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnMouseMove(nFlags, point);

	if (m_MouseRect.PtInRect(point))
	{
		::SetCursor(::LoadCursor(NULL, IDC_CROSS));

		if (m_bIsDraging)
		{
			CWnd* pWnd = GetDlgItem(IDC_STATIC);

			CDC* pDC = pWnd->GetDC();

			int nOldDrawMode = pDC->SetROP2(R2_XORPEN);

			CPen* pPen = new CPen;
			pPen->CreatePen(PS_DOT, 1, RGB(0, 0, 0));

			CGdiObject* pOldPen = pDC->SelectObject(pPen);

			if (m_bDrawed)
			{
				pDC->MoveTo(m_p1);
				pDC->MoveTo(m_p2);
			}

			m_p2 = point;

			m_p2.x = m_p2.x - m_MouseRect.left + 10;
			m_p2.y = m_p2.x - m_MouseRect.top + 25;

			pDC->MoveTo(m_p1);
			pDC->LineTo(m_p2);

			m_bDrawed = TRUE;

			pDC->SelectObject(pOldPen);

			pDC->SetROP2(nOldDrawMode);

			delete pPen;
			ReleaseDC(pDC);
		}
	}
}
