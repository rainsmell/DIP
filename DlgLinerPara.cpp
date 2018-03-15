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
	ON_WM_PAINT()
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
	CWnd* pWnd = GetDlgItem(IDC_COORD);

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


	//CDC* pDC = GetDC();
	//CPen* rPen = new CPen;
	//rPen->CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	//CPen* oldPen = pDC->SelectObject(rPen);
	//pDC->Rectangle(0, 0, 255, 256);
	//pDC->SelectObject(oldPen);
	//delete rPen;
	//ReleaseDC(pDC);

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
	if (m_bIsDraging)
	{
		if (m_MouseRect.PtInRect(point))
		{
			m_p2 = point;

			m_p2.x = m_p2.x - m_MouseRect.left + 10;
			m_p2.y = m_p2.y - m_MouseRect.top + 25;

			//if ((m_p1 != m_p2) && (m_p1.x != m_p2.x))
			if (m_p1 != m_p2)
			{
				m_p1.x = m_p1.x - 10;
				m_p1.y = 280 - m_p1.y;
				m_p2.x = m_p2.x - 10;
				m_p2.y = 280 - m_p2.y;

				m_fA = (float)(m_p2.y - m_p1.y) / (m_p2.x - m_p1.x);
				m_fB = m_p1.y - m_fA * m_p1.x;

				UpdateData(FALSE);
			}

			InvalidateRect(m_MouseRect, TRUE);
		}
		else
		{
			CWnd* pWnd = GetDlgItem(IDC_COORD);

			CDC* pDC = pWnd->GetDC();

			int nOldDrawMode = pDC->SetROP2(R2_XORPEN);

			CPen* pPen = new CPen;
			pPen->CreatePen(PS_DOT, 1, RGB(0, 0, 0));

			CGdiObject* pOldPen = pDC->SelectObject(pPen);

			if (m_bDrawed)
			{
				pDC->MoveTo(m_p1);
				pDC->LineTo(m_p2);
			}

			pDC->SelectObject(pOldPen);
			pDC->SetROP2(nOldDrawMode);

			delete pPen;
			ReleaseDC(pDC);
		}
		::ReleaseCapture();
		m_bIsDraging = FALSE;
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDlgLinerPara::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_MouseRect.PtInRect(point))
	{
		::SetCursor(::LoadCursor(NULL, IDC_CROSS));

		if (m_bIsDraging)
		{
			CWnd* pWnd = GetDlgItem(IDC_COORD);

			CDC* pDC = pWnd->GetDC();

			int nOldDrawMode = pDC->SetROP2(R2_XORPEN);

			CPen* pPen = new CPen;
			pPen->CreatePen(PS_DOT, 1, RGB(0, 0, 0));

			CGdiObject* pOldPen = pDC->SelectObject(pPen);

			if (m_bDrawed)
			{
				pDC->MoveTo(m_p1);
				pDC->LineTo(m_p2);
			}

			m_p2 = point;

			m_p2.x = m_p2.x - m_MouseRect.left + 10;
			m_p2.y = m_p2.y - m_MouseRect.top + 25;

			pDC->MoveTo(m_p1);
			pDC->LineTo(m_p2);

			m_bDrawed = TRUE;

			pDC->SelectObject(pOldPen);

			pDC->SetROP2(nOldDrawMode);

			delete pPen;
			ReleaseDC(pDC);
		}
	}
	else
	{
		if (m_bIsDraging)
			::SetCursor(::LoadCursor(NULL, IDC_NO));
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CDlgLinerPara::OnPaint()
{
	CString str;

	int x1, y1, x2, y2;

	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages

	CWnd* pWnd = GetDlgItem(IDC_COORD);

	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	pDC->Rectangle(0, 0, 330, 300);

	CPen* pPenRed = new CPen;

	pPenRed->CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

	CPen* pPenBlue = new CPen;

	pPenBlue->CreatePen(PS_SOLID, 2, RGB(0, 0, 255));

	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);

	pDC->MoveTo(10, 10);
	pDC->LineTo(10, 280); // x轴
	pDC->LineTo(320, 280); // y轴

	str.Format(L"0");
	pDC->TextOutW(10, 281, str);

	str.Format(L"255");
	pDC->TextOutW(265, 281, str);
	pDC->TextOutW(11, 25, str);

	// x轴箭头
	pDC->LineTo(315, 275);
	pDC->MoveTo(320, 280);
	pDC->LineTo(315, 285);

	// y轴箭头
	pDC->MoveTo(10, 10);
	pDC->LineTo(5, 15);
	pDC->MoveTo(10, 10);
	pDC->LineTo(15, 15);

	pDC->SelectObject(pPenBlue);

	if (m_fA >= 0)
	{
		if (((m_fA * 255 + m_fB) >= 0) && (m_fB < 255))
		{
			// 计算(x1, y1)坐标
			if (m_fB < 0)
			{
				x1 = (int)(-m_fB / m_fA + 0.5);
				y1 = 0;
			}
			else
			{
				x1 = 0;
				y1 = (int)(m_fB + 0.5);
			}

			// 计算(x2, y2)坐标
			if ((m_fA * 255 + m_fB) > 255)
			{
				x2 = (int)((255 - m_fB) / m_fA + 0.5);
				y2 = 255;
			}
			else
			{
				x2 = 255;
				y2 = (int)(255 * m_fA + m_fB + 0.5);
			}
		}
		else if (((m_fA * 255 + m_fB) < 0))
		{
			// 转换后所有像素值都小于0，直接设置为0
			x1 = 0;
			y1 = 0;
			x2 = 255;
			y2 = 0;
		}
		else
		{
			// 转换后所有像素值都大于255，直接设置为255
			x1 = 0;
			y1 = 255;
			x2 = 255;
			y2 = 255;
		}
	}
	else  // 斜率小于0
	{
		if ((m_fB > 0) && (255 * m_fA + m_fB < 255))
		{
			// 计算(x1, y1)坐标
			if (m_fB > 255)
			{
				x1 = (int)((255 - m_fB) / m_fA + 0.5);
				y1 = 255;
			}
			else
			{
				x1 = 0;
				y1 = (int)(m_fB + 0.5);
			}

			// 计算(x2, y2)坐标
			if ((m_fA * 255 + m_fB) < 0)
			{
				x2 = (int)(-m_fB / m_fA + 0.5);
				y2 = 0;
			}
			else
			{
				x2 = 255;
				y2 = (int)(255 * m_fA + m_fB + 0.5);
			}
		}
		else if (m_fB <= 0)
		{
			// 转换后所有像素值都小于0，直接设置为0
			x1 = 0;
			y1 = 0;
			x2 = 255;
			y2 = 0;
		}
		else
		{
			// 转换后所有像素值都大于255，直接设置为255
			x1 = 0;
			y1 = 255;
			x2 = 255;
			y2 = 255;
		}
	}

	str.Format(L"(%d, %d)", x1, y1);
	pDC->TextOut(x1 + 10, 280 - y1 + 1, str);
	str.Format(L"(%d, %d)", x2, y2);
	pDC->TextOut(x2 + 10, 280 - y2 + 1, str);

	pDC->MoveTo(x1 + 10, 280 - y1);
	pDC->LineTo(x2 + 10, 280 - y2);

	pDC->SelectObject(pOldPen);

	pDC->MoveTo(10, 25);
	pDC->LineTo(265, 25);
	pDC->LineTo(265, 280);

	delete pPenRed;
	delete pPenBlue;
}
