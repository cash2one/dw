// RSAUIView.h : CRSAUIView 类的接口
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
protected: // 仅从序列化创建
	CRSAUIView();
	DECLARE_DYNCREATE(CRSAUIView)

// 属性
public:
	CRSAUIDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
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
	int m_sub_item;			// 用于协议解析, 主项标示符

	int m_iSubItem;            //子项标识符
	int m_iItem;               //主项标识符
	
	CXEdit *		m_pEdit;

// 生成的消息映射函数
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

#ifndef _DEBUG  // RSAUIView.cpp 中的调试版本
inline CRSAUIDoc* CRSAUIView::GetDocument() const
   { return reinterpret_cast<CRSAUIDoc*>(m_pDocument); }
#endif

