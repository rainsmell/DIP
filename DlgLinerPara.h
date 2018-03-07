#pragma once


// CDlgLinerPara dialog

class CDlgLinerPara : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLinerPara)

public:
	CDlgLinerPara(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLinerPara();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LinerPara };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_fA;
	double m_fB;
};
