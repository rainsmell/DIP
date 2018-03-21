// DlgSmoothConfig.cpp : implementation file
//

#include "stdafx.h"
#include "DIP.h"
#include "DlgSmoothConfig.h"
#include "afxdialogex.h"


// CDlgSmoothConfig dialog

IMPLEMENT_DYNAMIC(CDlgSmoothConfig, CDialogEx)

CDlgSmoothConfig::CDlgSmoothConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_Smooth, pParent)
	, m_intType(-1)
	, m_fTempC(0)
	, m_iTempH(0)
	, m_iTempW(0)
	, m_iTempMX(0)
	, m_iTempMY(0)
{

}

CDlgSmoothConfig::~CDlgSmoothConfig()
{
}

void CDlgSmoothConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEMPC, m_fTempC);
	DDX_Text(pDX, IDC_EDIT_TEMPH, m_iTempH);
	DDV_MinMaxInt(pDX, m_iTempH, 2, 5);
	DDX_Text(pDX, IDC_EDIT_TEMPW, m_iTempW);
	DDV_MinMaxInt(pDX, m_iTempW, 2, 5);
	DDX_Text(pDX, IDC_EDIT_MX, m_iTempMX);
	DDX_Text(pDX, IDC_EDIT_MY, m_iTempMY);
	DDV_MinMaxInt(pDX, m_iTempMX, 1, 2);
	DDV_MinMaxInt(pDX, m_iTempMY, 1, 2);
	
	DDX_Text(pDX, IDC_EDIT_V0, m_fpArray[0]);
	DDX_Text(pDX, IDC_EDIT_V1, m_fpArray[1]);
	DDX_Text(pDX, IDC_EDIT_V2, m_fpArray[2]);
	DDX_Text(pDX, IDC_EDIT_V3, m_fpArray[3]);
	DDX_Text(pDX, IDC_EDIT_V4, m_fpArray[4]);
	DDX_Text(pDX, IDC_EDIT_V5, m_fpArray[5]);
	DDX_Text(pDX, IDC_EDIT_V6, m_fpArray[6]);
	DDX_Text(pDX, IDC_EDIT_V7, m_fpArray[7]);
	DDX_Text(pDX, IDC_EDIT_V8, m_fpArray[8]);
	DDX_Text(pDX, IDC_EDIT_V9, m_fpArray[9]);
	DDX_Text(pDX, IDC_EDIT_V10, m_fpArray[10]);
	DDX_Text(pDX, IDC_EDIT_V11, m_fpArray[11]);
	DDX_Text(pDX, IDC_EDIT_V12, m_fpArray[12]);
	DDX_Text(pDX, IDC_EDIT_V13, m_fpArray[13]);
	DDX_Text(pDX, IDC_EDIT_V14, m_fpArray[14]);
	DDX_Text(pDX, IDC_EDIT_V15, m_fpArray[15]);
	DDX_Text(pDX, IDC_EDIT_V16, m_fpArray[16]);
	DDX_Text(pDX, IDC_EDIT_V17, m_fpArray[17]);
	DDX_Text(pDX, IDC_EDIT_V18, m_fpArray[18]);
	DDX_Text(pDX, IDC_EDIT_V19, m_fpArray[19]);
	DDX_Text(pDX, IDC_EDIT_V20, m_fpArray[20]);
	DDX_Text(pDX, IDC_EDIT_V21, m_fpArray[21]);
	DDX_Text(pDX, IDC_EDIT_V22, m_fpArray[22]);
	DDX_Text(pDX, IDC_EDIT_V23, m_fpArray[23]);
	DDX_Text(pDX, IDC_EDIT_V24, m_fpArray[24]);
}


BEGIN_MESSAGE_MAP(CDlgSmoothConfig, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_AVG, &CDlgSmoothConfig::OnBnClickedRadioAvg)
	ON_BN_CLICKED(IDC_RADIO_Gauss, &CDlgSmoothConfig::OnBnClickedRadioGauss)
END_MESSAGE_MAP()

void CDlgSmoothConfig::UpdateEdit()
{
	BOOL bEnable;

	if (m_intType == 2)
		bEnable = TRUE;
	else
		bEnable = FALSE;

	(CEdit*)GetDlgItem(IDC_EDIT_TEMPC)->EnableWindow(bEnable);
	(CEdit*)GetDlgItem(IDC_EDIT_TEMPH)->EnableWindow(bEnable);
	(CEdit*)GetDlgItem(IDC_EDIT_TEMPW)->EnableWindow(bEnable);
	(CEdit*)GetDlgItem(IDC_EDIT_MX)->EnableWindow(bEnable);
	(CEdit*)GetDlgItem(IDC_EDIT_MY)->EnableWindow(bEnable);

	for (int i = IDC_EDIT_V0; i <= IDC_EDIT_V24; i++)
		(CEdit*)GetDlgItem(i)->EnableWindow(bEnable);

	for (int h = 0; h < m_iTempH; h++)
		for (int w = 0; w < m_iTempW; w++)
			(CEdit*)GetDlgItem(IDC_EDIT_V0 + h * 5 + w)->ShowWindow(SW_SHOW);

	for (int h = 0; h < m_iTempH; h++)
		for (int w = m_iTempW; w < 5; w++)
			(CEdit*)GetDlgItem(IDC_EDIT_V0 + h * 5 + w)->ShowWindow(SW_HIDE);

	for (int h = m_iTempH; h < 5; h++)
		for (int w = 0; w < 5; w++)
			(CEdit*)GetDlgItem(IDC_EDIT_V0 + h * 5 + w)->ShowWindow(SW_HIDE);
}

// CDlgSmoothConfig message handlers


void CDlgSmoothConfig::OnBnClickedRadioAvg()
{
	// TODO: Add your control notification handler code here
	m_intType = 0;
	m_iTempH = 3;
	m_iTempW = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC = (FLOAT)(1.0 / 9.0);

	m_fpArray[0] = 1.0;
	m_fpArray[1] = 1.0;
	m_fpArray[2] = 1.0;
	m_fpArray[3] = 0.0;
	m_fpArray[4] = 0.0;
	m_fpArray[5] = 1.0;
	m_fpArray[6] = 1.0;
	m_fpArray[7] = 1.0;
	m_fpArray[8] = 0.0;
	m_fpArray[9] = 0.0;
	m_fpArray[10] = 1.0;
	m_fpArray[11] = 1.0;
	m_fpArray[12] = 1.0;
	m_fpArray[13] = 0.0;
	m_fpArray[14] = 0.0;
	m_fpArray[15] = 0.0;
	m_fpArray[16] = 0.0;
	m_fpArray[17] = 0.0;
	m_fpArray[18] = 0.0;
	m_fpArray[19] = 0.0;
	m_fpArray[20] = 0.0;
	m_fpArray[21] = 0.0;
	m_fpArray[22] = 0.0;
	m_fpArray[23] = 0.0;
	m_fpArray[24] = 0.0;

	UpdateEdit();

	UpdateData(FALSE); // FALSE：将数据在控件上显示出来，TRUE:将控件显示的值复制给关联的变量

}


void CDlgSmoothConfig::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	//UpdateData(TRUE);

	if ((m_iTempMX < 0) || (m_iTempMX > m_iTempW - 1) ||
		(m_iTempMY < 0) || (m_iTempMY > m_iTempH - 1))
	{
		MessageBox(L"中心元素参数设置错误！", L"系统提示",
			MB_ICONINFORMATION | MB_OK);
		return;
	}

	for (int i = 0; i < m_iTempH; i++)
		for (int j = 0; j < m_iTempW; j++)
			m_fpArray[i * m_iTempH + j] = m_fpArray[i * 5 + j];

	// 这里很奇怪，FALSE应该是对控件产生效果，但是如果不加这句，
	// 上面对m_fpArray的赋值，居然对主文档视图(CDIPView)里面的aValue不产生任何影响。
	// m_fpArray可是指向aValue的指针啊？？？？？
	// 难道说OnOK结束后，对话框还会将控件的值再赋值给关联的成员变量，
	// 像是又调用了一遍UpdateDate(TRUE)？姑且这么认为吧。
	UpdateData(FALSE); 

	CDialogEx::OnOK();
}


void CDlgSmoothConfig::OnBnClickedRadioGauss()
{
	// TODO: Add your control notification handler code here
	m_intType = 0;
	m_iTempH = 3;
	m_iTempW = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC = (FLOAT)(1.0 / 16.0);

	m_fpArray[0] = 1.0;
	m_fpArray[1] = 2.0;
	m_fpArray[2] = 1.0;
	m_fpArray[3] = 0.0;
	m_fpArray[4] = 0.0;
	m_fpArray[5] = 2.0;
	m_fpArray[6] = 4.0;
	m_fpArray[7] = 2.0;
	m_fpArray[8] = 0.0;
	m_fpArray[9] = 0.0;
	m_fpArray[10] = 1.0;
	m_fpArray[11] = 2.0;
	m_fpArray[12] = 1.0;
	m_fpArray[13] = 0.0;
	m_fpArray[14] = 0.0;
	m_fpArray[15] = 0.0;
	m_fpArray[16] = 0.0;
	m_fpArray[17] = 0.0;
	m_fpArray[18] = 0.0;
	m_fpArray[19] = 0.0;
	m_fpArray[20] = 0.0;
	m_fpArray[21] = 0.0;
	m_fpArray[22] = 0.0;
	m_fpArray[23] = 0.0;
	m_fpArray[24] = 0.0;

	UpdateEdit();

	UpdateData(FALSE);
}
