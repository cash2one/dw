// RSAUIDoc.h : CRSAUIDoc 类的接口
//


#pragma once

typedef map<wstring, wstring> FILES_MAP;

class CRSAUIDoc : public CDocument
{
protected: // 仅从序列化创建
	CRSAUIDoc();
	DECLARE_DYNCREATE(CRSAUIDoc)

// 属性
public:

// 操作
public:

// 重写
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

	// 已选择的文件
	string m_selected_file;

	// 查询请求
	Date  m_start_time;
	Date  m_end_time;
	bool  m_is_Guid;
	Guid  m_guid;

	RecordArray m_record_array;
	HistoryRecord m_history_array;


	// 当前选中的消息
	boost::shared_ptr<Recordst> m_current_record;

private:
	// 选择文件夹
	CString ChoosePath();
	// 枚举文件夹里的文件
	void IterateFiles(CString& file_path);


// 实现
public:
	virtual ~CRSAUIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileOpen();
};


