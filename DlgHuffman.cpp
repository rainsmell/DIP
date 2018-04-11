// DlgHuffman.cpp : implementation file
//

#include "stdafx.h"
#include "DIP.h"
#include "DlgHuffman.h"
#include "afxdialogex.h"
//#include <atlconv.h>

// CDlgHuffman dialog

IMPLEMENT_DYNAMIC(CDlgHuffman, CDialogEx)

CDlgHuffman::CDlgHuffman(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_Huffman, pParent)
	, m_sInputPath(_T(""))
	, m_sOutputPath(_T(""))
	, m_dEntropy(0)
	, m_dAverCodeLen(0)
	, m_dRate(0)
{

}

CDlgHuffman::~CDlgHuffman()
{
}

void CDlgHuffman::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_sInputPath);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_sOutputPath);
	DDX_Text(pDX, IDC_EDIT1, m_dEntropy);
	DDX_Text(pDX, IDC_EDIT2, m_dAverCodeLen);
	DDX_Text(pDX, IDC_EDIT3, m_dRate);
}


BEGIN_MESSAGE_MAP(CDlgHuffman, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_INPUT, &CDlgHuffman::OnClickedBtnInput)
	ON_BN_CLICKED(IDC_BTN_OUTPUT, &CDlgHuffman::OnClickedBtnOutput)
	ON_BN_CLICKED(IDC_BTN_ENCODE, &CDlgHuffman::OnClickedBtnEncode)
	ON_BN_CLICKED(IDC_BTN_DECODE, &CDlgHuffman::OnClickedBtnDecode)
END_MESSAGE_MAP()


// CDlgHuffman message handlers


void CDlgHuffman::OnClickedBtnInput()
{
	// TODO: Add your control notification handler code here
	static wchar_t szFilter[] = L"All(*.*)|*.*|HUF(*.huf)|*.huf|";
	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, (LPCTSTR)szFilter);
	if (FileDlg.DoModal() == IDOK)
		m_sInputPath = FileDlg.GetPathName();
	else
		m_sInputPath = "D:\\lena.bmp";
	if (m_sInputPath.Right(3) != "huf")
		m_sOutputPath = m_sInputPath + L".huf";
	else
		m_sOutputPath = m_sInputPath.Left(m_sInputPath.GetLength() - 4);

	UpdateData(false);
}


void CDlgHuffman::OnClickedBtnOutput()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	static wchar_t szFilter[] = L"ALL Files(*.*)|*.*|";
	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, (LPCTSTR)szFilter);
	if (FileDlg.DoModal() == IDOK)
	{
		m_sOutputPath = FileDlg.GetPathName();
	}
	else
	{
		m_sOutputPath = "D:\\out.raw";
	}

	UpdateData(false);
}


void CDlgHuffman::OnClickedBtnEncode()
{
	// TODO: Add your control notification handler code here
	FILE* fpInput, *fpOutput;
	USES_CONVERSION;
	char* in = T2A(m_sInputPath);
	if ((fpInput = fopen(in, "rb")) == NULL)
	{
		AfxMessageBox(L"输入文件打不开!\n");
		return;
	}

	if ((fpOutput = fopen(T2A(m_sOutputPath), "wb")) == NULL)
	{
		AfxMessageBox(L"输出文件无法创建!\n");
		return;
	}

	huffcode.SetInputFile(fpInput);
	huffcode.SetOutputFile(fpOutput);
	huffcode.BuildTree();
	huffcode.EncodeFile();
	
	fclose(fpInput);
	fclose(fpOutput);

	UpdateData(false);
	CString info;
	info.Format(L"霍夫曼编码结束！输出文件：%s\n", m_sOutputPath);//second
	AfxMessageBox(info);
}


void CDlgHuffman::OnClickedBtnDecode()
{
	// TODO: Add your control notification handler code here
	FILE* fpInput, *fpOutput;
	if (m_sInputPath.Right(3) != "huf")
	{
		AfxMessageBox(L"输入文件应为(*.huf)文件!\n");
		return;
	}
	USES_CONVERSION;
	if ((fpInput = fopen(T2A(m_sInputPath), "rb")) == NULL)
	{
		AfxMessageBox(L"输入文件打不开!\n");
		return;
	}

	if ((fpOutput = fopen(T2A(m_sOutputPath), "wb")) == NULL)
	{
		AfxMessageBox(L"输出文件无法创建!\n");
		return;
	}

	huffcode.SetInputFile(fpInput);
	huffcode.SetOutputFile(fpOutput);
	huffcode.DecodeFile();

	fclose(fpInput);
	fclose(fpOutput);

	update();

	UpdateData(false);
	CString info;
	info.Format(L"霍夫曼解码结束！输出文件：%s\n", m_sOutputPath);//second
	AfxMessageBox(info);
}

void CDlgHuffman::update()
{
	m_dAverCodeLen = huffcode.GetAverCodeLen();
	m_dRate = huffcode.GetCodeRate();
	m_dEntropy = huffcode.GetRawFileEntropy();
}