#pragma once

template <class T>
class Single
{
public:
	template<typename... Args>
	static T* Instance(Args... args)
	{
		if (m_pInstance == nullptr)
			m_pInstance = new T(args...);

		return m_pInstance;
	}

	static T* GetInstance()
	{
		if (m_pInstance == nullptr)
			throw std::logic_error("the instance is not init, please initialize the instance first");

		return m_pInstance;
	}

    static void DestroyInstance()
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }

private:
	Single(void);
	virtual ~Single(void);
	Single(const Single&);
	Single& operator=(const Single&);
private:
    static T* m_pInstance;
};

template <class T> T*  Single<T>::m_pInstance = nullptr;

