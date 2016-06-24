#pragma once

#include "../ParseInterface/ParseInterface.h"

// CProtocolView ��ͼ

class CProtocolView : public CTreeView
{
	DECLARE_DYNCREATE(CProtocolView)

protected:
	CProtocolView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CProtocolView();
	
	void Show(const MessageMap* pmap_msg, HTREEITEM hParent);
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	virtual void OnInitialUpdate();
};


