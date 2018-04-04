// DlgInvRad.cpp : implementation file
//

#include "stdafx.h"
#include "DIP.h"
#include "DlgInvRad.h"
#include "afxdialogex.h"


// CDlgInvRad dialog

IMPLEMENT_DYNAMIC(CDlgInvRad, CDialogEx)

CDlgInvRad::CDlgInvRad(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_InvRad, pParent)
	, m_nRad(0)
{

}

CDlgInvRad::~CDlgInvRad()
{
}

void CDlgInvRad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nRad);
}


BEGIN_MESSAGE_MAP(CDlgInvRad, CDialogEx)
END_MESSAGE_MAP()


// CDlgInvRad message handlers
