#pragma once

#include "Huffman.h"

// CDlgHuffman dialog

class CDlgHuffman : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHuffman)

public:
	CDlgHuffman(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgHuffman();


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_Huffman };
#endif

private:
	void update();
	HuffCode huffcode;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_sInputPath;
	CString m_sOutputPath;
	afx_msg void OnClickedBtnInput();
	afx_msg void OnClickedBtnOutput();
	afx_msg void OnClickedBtnEncode();
	afx_msg void OnClickedBtnDecode();
	double m_dEntropy;
	double m_dAverCodeLen;
	double m_dRate;
};
