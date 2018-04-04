// DlgFreqLPF.cpp : implementation file
//

#include "stdafx.h"
#include "DIP.h"
#include "DlgFreqLPF.h"
#include "afxdialogex.h"


// CDlgFreqLPF dialog

IMPLEMENT_DYNAMIC(CDlgFreqLPF, CDialogEx)

CDlgFreqLPF::CDlgFreqLPF(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LPF, pParent)
	, m_dFreq(0)
{

}

CDlgFreqLPF::~CDlgFreqLPF()
{
}

void CDlgFreqLPF::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dFreq);
}


BEGIN_MESSAGE_MAP(CDlgFreqLPF, CDialogEx)
END_MESSAGE_MAP()


// CDlgFreqLPF message handlers
