// RSAUIDoc.h : CRSAUIDoc ��Ľӿ�
//


#pragma once

typedef map<wstring, wstring> FILES_MAP;

class CRSAUIDoc : public CDocument
{
protected: // �������л�����
	CRSAUIDoc();
	DECLARE_DYNCREATE(CRSAUIDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

public:
	void Load();
	void Notify();
	void DispatchMsg(UINT message, WPARAM wparam, LPARAM lparam);
	FILES_MAP m_files;
	DataManager m_manager;
	pool m_notify;

	// ��ѡ����ļ�
	string m_selected_file;

	// ��ѯ����
	Date  m_start_time;
	Date  m_end_time;
	bool  m_is_Guid;
	Guid  m_guid;

	RecordArray m_record_array;
	HistoryRecord m_history_array;


	// ��ǰѡ�е���Ϣ
	boost::shared_ptr<Recordst> m_current_record;

private:
	// ѡ���ļ���
	CString ChoosePath();
	// ö���ļ�������ļ�
	void IterateFiles(CString& file_path);


// ʵ��
public:
	virtual ~CRSAUIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileOpen();
};


