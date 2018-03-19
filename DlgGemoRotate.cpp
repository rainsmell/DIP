// DlgGemoRotate.cpp : implementation file
//

#include "stdafx.h"
#include "DIP.h"
#include "DlgGemoRotate.h"
#include "afxdialogex.h"


// CDlgGemoRotate dialog

IMPLEMENT_DYNAMIC(CDlgGemoRotate, CDialogEx)

CDlgGemoRotate::CDlgGemoRotate(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_Rotate, pParent)
	, m_fAng(0)
{

}

CDlgGemoRotate::~CDlgGemoRotate()
{
}

void CDlgGemoRotate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fAng);
}


BEGIN_MESSAGE_MAP(CDlgGemoRotate, CDialogEx)
END_MESSAGE_MAP()


// CDlgGemoRotate message handlers
