// RSAUIView.h : CRSAUIView ��Ľӿ�
//


#pragma once
#include "XEdit.h"

enum ShowMode
{
	eShowFileRecord,
	eShowHistoryRecrod,
};

class CRSAUIView : public CListView
{
protected: // �������л�����
	CRSAUIView();
	DECLARE_DYNCREATE(CRSAUIView)

// ����
public:
	CRSAUIDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CRSAUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void loadFile();
	void query();
	void show();
	void insertItem(boost::shared_ptr<Recordst> record);
	void Clear();
	bool GetItem(DWORD item, boost::shared_ptr<Recordst>& record);
	void OnItemChanged(int item);

private:
	ShowMode m_show_mode;
	int m_sub_item;			// ����Э�����, �����ʾ��

	int m_iSubItem;            //�����ʶ��
	int m_iItem;               //�����ʶ��
	
	CXEdit *		m_pEdit;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
public:
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnXEditKillFocus(WPARAM, LPARAM);
	afx_msg LRESULT OnXEditEscape(WPARAM, LPARAM);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

#ifndef _DEBUG  // RSAUIView.cpp �еĵ��԰汾
inline CRSAUIDoc* CRSAUIView::GetDocument() const
   { return reinterpret_cast<CRSAUIDoc*>(m_pDocument); }
#endif

