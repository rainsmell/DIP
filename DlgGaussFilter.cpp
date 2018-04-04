// DlgGaussFilter.cpp : implementation file
//

#include "stdafx.h"
#include "DIP.h"
#include "DlgGaussFilter.h"
#include "afxdialogex.h"


// CDlgGaussFilter dialog

IMPLEMENT_DYNAMIC(CDlgGaussFilter, CDialogEx)

CDlgGaussFilter::CDlgGaussFilter(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_GaussFilter, pParent)
	, m_dSigma(0)
{

}

CDlgGaussFilter::~CDlgGaussFilter()
{
}

void CDlgGaussFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dSigma);
}


BEGIN_MESSAGE_MAP(CDlgGaussFilter, CDialogEx)
END_MESSAGE_MAP()


// CDlgGaussFilter message handlers
