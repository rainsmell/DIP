// DlgLog.cpp : implementation file
//

#include "stdafx.h"
#include "DIP.h"
#include "DlgLog.h"
#include "afxdialogex.h"


// DlgLog dialog

IMPLEMENT_DYNAMIC(DlgLog, CDialogEx)

DlgLog::DlgLog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LOG, pParent)
	, m_dC(0)
{

}

DlgLog::~DlgLog()
{
}

void DlgLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOG_C, m_dC);
}


BEGIN_MESSAGE_MAP(DlgLog, CDialogEx)
END_MESSAGE_MAP()


// DlgLog message handlers
