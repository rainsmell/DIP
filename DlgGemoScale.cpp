// DlgGemoScale.cpp : implementation file
//

#include "stdafx.h"
#include "DIP.h"
#include "DlgGemoScale.h"
#include "afxdialogex.h"


// CDlgGemoScale dialog

IMPLEMENT_DYNAMIC(CDlgGemoScale, CDialogEx)

CDlgGemoScale::CDlgGemoScale(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_Scale, pParent)
	, m_dTimes(0)
{

}

CDlgGemoScale::~CDlgGemoScale()
{
}

void CDlgGemoScale::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dTimes);
}


BEGIN_MESSAGE_MAP(CDlgGemoScale, CDialogEx)
END_MESSAGE_MAP()


// CDlgGemoScale message handlers
