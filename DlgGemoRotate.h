#pragma once


// CDlgGemoRotate dialog

class CDlgGemoRotate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGemoRotate)

public:
	CDlgGemoRotate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGemoRotate();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_Rotate };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float m_fAng;
};
