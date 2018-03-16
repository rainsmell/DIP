#pragma once


// CDlgParLineTran dialog

class CDlgParLineTran : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgParLineTran)

public:
	CDlgParLineTran(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgParLineTran();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_ParLineTran };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_dX1;
	double m_dY1;
	double m_dX2;
	double m_dY2;
};
