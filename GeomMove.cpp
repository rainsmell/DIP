// GeomMove.cpp : implementation file
//

#include "stdafx.h"
#include "DIP.h"
#include "GeomMove.h"
#include "afxdialogex.h"


// CGeomMove dialog

IMPLEMENT_DYNAMIC(CGeomMove, CDialogEx)

CGeomMove::CGeomMove(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_Move, pParent)
	, m_dX(0)
	, m_dY(0)
{

}

CGeomMove::~CGeomMove()
{
}

void CGeomMove::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dX);
	DDX_Text(pDX, IDC_EDIT2, m_dY);
}


BEGIN_MESSAGE_MAP(CGeomMove, CDialogEx)
END_MESSAGE_MAP()


// CGeomMove message handlers
