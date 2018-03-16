// DlgParLineTran.cpp : implementation file
//

#include "stdafx.h"
#include "DIP.h"
#include "DlgParLineTran.h"
#include "afxdialogex.h"


// CDlgParLineTran dialog

IMPLEMENT_DYNAMIC(CDlgParLineTran, CDialogEx)

CDlgParLineTran::CDlgParLineTran(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_ParLineTran, pParent)
	, m_dX1(0)
	, m_dY1(0)
	, m_dX2(0)
	, m_dY2(0)
{

}

CDlgParLineTran::~CDlgParLineTran()
{
}

void CDlgParLineTran::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dX1);
	DDX_Text(pDX, IDC_EDIT2, m_dY1);
	DDX_Text(pDX, IDC_EDIT3, m_dX2);
	DDX_Text(pDX, IDC_EDIT4, m_dY2);
}


BEGIN_MESSAGE_MAP(CDlgParLineTran, CDialogEx)
END_MESSAGE_MAP()


// CDlgParLineTran message handlers
