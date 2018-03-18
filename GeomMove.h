#pragma once


// CGeomMove dialog

class CGeomMove : public CDialogEx
{
	DECLARE_DYNAMIC(CGeomMove)

public:
	CGeomMove(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGeomMove();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_Move };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_dX;
	double m_dY;
};
