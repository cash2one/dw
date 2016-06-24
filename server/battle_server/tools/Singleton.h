
#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <string>

/////////////////////////////////////////////////////////////////////////
///CClearup�����˵�ʵ����ģ����Ļ��࣬���ڶ�����������������
///@author	Jacky
///@version	 
/////////////////////////////////////////////////////////////////////////
class Cleanup
{
public:
	virtual ~Cleanup();
	///��������ö�����ռ�õ���Դ
	virtual void cleanup();

	virtual void registerSingleton();
};


/////////////////////////////////////////////////////////////////////////
///Singleton�����˵�ʵ�����ģ���ࡣ
///@author	Jacky
///@version 
/////////////////////////////////////////////////////////////////////////

template<class TYPE>
class Singleton : public Cleanup
{
public:
	virtual ~Singleton(void);
protected:
	Singleton(void);
public:
	///���ڻ�ȡTYPE���͵ĵ�ʵ������
	static TYPE *instance ();
protected:
	///TYPE���Ͷ���
	TYPE  m_instance;

	///���൥ʵ������ָ��
	static Singleton<TYPE> * m_singleton;

};

template<class TYPE>
Singleton<TYPE> * Singleton<TYPE>::m_singleton = NULL;

template<class TYPE>
Singleton<TYPE>::Singleton(void)
{
}

template<class TYPE>
Singleton<TYPE>::~Singleton(void)
{
	m_singleton = NULL;
}

template<class TYPE>
TYPE * Singleton<TYPE>::instance ()
{
	if (NULL == m_singleton)
	{
		if (NULL == m_singleton)
		{
			m_singleton = new Singleton();
			m_singleton->registerSingleton();
		}
	}
	return &m_singleton->m_instance;
}


#endif
