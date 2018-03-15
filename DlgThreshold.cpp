// DlgThreshold.cpp : implementation file
//

#include "stdafx.h"
#include "DIP.h"
#include "DlgThreshold.h"
#include "afxdialogex.h"


// CDlgThreshold dialog

IMPLEMENT_DYNAMIC(CDlgThreshold, CDialogEx)

CDlgThreshold::CDlgThreshold(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_dThre(0)
{

}

CDlgThreshold::~CDlgThreshold()
{
}

void CDlgThreshold::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_THRESHOLD, m_dThre);
}


BEGIN_MESSAGE_MAP(CDlgThreshold, CDialogEx)
END_MESSAGE_MAP()


// CDlgThreshold message handlers
