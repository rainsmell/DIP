
// DIPDoc.h : CDIPDoc ��Ľӿ�
//


#pragma once
#include "Img.h"
#include "ImgProcess.h"

class CDIPDoc : public CDocument
{
protected: // �������л�����
	CDIPDoc();
	DECLARE_DYNCREATE(CDIPDoc)

// ����
public:

// ����
public:
	CSize GetDocSize() const
	{
		return m_sizeDoc;
	}

	CPalette* GetDocPalette() const
	{
		return m_pPalette;
	}

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	CImgProcess m_Image;
	CImgProcess m_OImage;

	CPalette *m_pPalette;

	void Init();

	virtual ~CDIPDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

protected:
	CSize m_sizeDoc;
};
