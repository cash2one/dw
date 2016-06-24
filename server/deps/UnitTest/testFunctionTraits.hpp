#pragma  once
#include "Single.hpp"
template<class T>
void printarg(T t)
{
	std::cout << t << std::endl;
}

template<class T, class... ARGS>
void printarg(T t, ARGS... args)
{
	std::cout << t << std::endl;
	printarg(args...);
}

class Test3
{	
public:
	Test3(int a, int b, int c)
	{

	}
};




class TestFunctionTraits
{
public:
	static void run()
	{
		//printarg(1, 2, 3);
		//Singleton<Test3>::Instance(1, 2, 3);

	}
};