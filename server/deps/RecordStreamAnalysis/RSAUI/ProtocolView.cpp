// ProtocolView.cpp : 实现文件
//

#include "stdafx.h"
#include "RSAUI.h"
#include "ProtocolView.h"
#include "RSAUIDoc.h"


using namespace google::protobuf;

// CProtocolView

IMPLEMENT_DYNCREATE(CProtocolView, CTreeView)

CProtocolView::CProtocolView()
{

}

CProtocolView::~CProtocolView()
{
}

BEGIN_MESSAGE_MAP(CProtocolView, CTreeView)
END_MESSAGE_MAP()


// CProtocolView 诊断

#ifdef _DEBUG
void CProtocolView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CProtocolView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CProtocolView 消息处理程序

BOOL CProtocolView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{	
	switch(message)
	{
	case UM_ITEMCHANGED:
		{
			CRSAUIDoc *pDoc = (CRSAUIDoc *)GetDocument();
			ParseInterface parser;
			string type_name;
			MessageMap* pmap_msg = parser.Parse(pDoc->m_current_record->object_guid,
				&(pDoc->m_current_record->stream[0]), pDoc->m_current_record->stream.size(), type_name);
			if( NULL == pmap_msg)
				break;
			
			GetTreeCtrl().DeleteAllItems();
			HTREEITEM hRoot = GetTreeCtrl().InsertItem(Utf8ToUnicode(type_name).c_str());
			Show(pmap_msg, hRoot);
			GetTreeCtrl().Expand(hRoot, TVE_EXPAND);

			parser.DelMapMsg(pmap_msg);
		}
		break;
	}

	return CTreeView::OnWndMsg(message, wParam, lParam, pResult);
}

void CProtocolView::Show(const MessageMap* pmap_msg, HTREEITEM hParent)
{	
	CTreeCtrl& tree = GetTreeCtrl();
	MessageMap::const_iterator it = pmap_msg->begin();
	for(; it != pmap_msg->end(); ++it)
	{	
		StructValue* pSt = it->second;
	
		// 重复的
		if(FieldDescriptor::LABEL_REPEATED == pSt->type1)
		{
			wstring key = Utf8ToUnicode(string(pSt->key_name));
			HTREEITEM hRepeated = tree.InsertItem(key.c_str(), hParent);

			if(FieldDescriptor::TYPE_MESSAGE == pSt->type2)
			{
				for(int i = 0; i < pSt->vec_message.size(); i++)
				{	
					TCHAR trunk[10];
					_stprintf_s(trunk, 10, _T("<%d>"), i);
					HTREEITEM hTrunk = tree.InsertItem(trunk, hRepeated);
					Show(pSt->vec_message[i], hTrunk);
				}
			}
			else
			{	
				for(int i = 0; i < pSt->vec_value.size(); i++)
				{	
					TCHAR buf[10];
					_stprintf_s(buf, 10, _T("[%d]"), i);
					wstring value_obj = buf;
					value_obj += Utf8ToUnicode(pSt->vec_value[i]);
					tree.InsertItem(value_obj.c_str(), hRepeated);
				}

			}	

		}
		else
		{	// 单个的
			
			if(FieldDescriptor::TYPE_MESSAGE == pSt->type2)
			{	
				wstring key = Utf8ToUnicode(string(pSt->key_name));
				HTREEITEM hRepeated = tree.InsertItem(key.c_str(), hParent);

				for(int i = 0; i < pSt->vec_message.size(); i++)
				{
					Show(pSt->vec_message[i], hRepeated);
				}
			}
			else
			{	

				wstring key_value = L"[";
				key_value += Utf8ToUnicode(string(pSt->key_name));
				key_value += L"]";
				key_value += Utf8ToUnicode(string(pSt->vec_value[0]));
				tree.InsertItem(key_value.c_str(), hParent);
			}	
		}
	
	}
}

void CProtocolView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	GetTreeCtrl().ModifyStyle(0, TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_EDITLABELS);
}
