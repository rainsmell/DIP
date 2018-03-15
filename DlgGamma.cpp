// DlgGamma.cpp : implementation file
//

#include "stdafx.h"
#include "DIP.h"
#include "DlgGamma.h"
#include "afxdialogex.h"


// CDlgGamma dialog

IMPLEMENT_DYNAMIC(CDlgGamma, CDialogEx)

CDlgGamma::CDlgGamma(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_Gamma, pParent)
	, m_dEsp(0)
	, m_dGamma(0)
{

}

CDlgGamma::~CDlgGamma()
{
}

void CDlgGamma::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dEsp);
	DDX_Text(pDX, IDC_EDIT2, m_dGamma);
}


BEGIN_MESSAGE_MAP(CDlgGamma, CDialogEx)
END_MESSAGE_MAP()


// CDlgGamma message handlers
