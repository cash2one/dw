
#ifndef REDIRECT_HPP
#define REDIRECT_HPP

#include <string>
#include <iostream>

#ifndef WIN32
#include <signal.h>
#include <unistd.h>
#endif

using namespace std;

bool checkInputParam(int argc, char * argv[]);

#ifndef WIN32
string getProcessDir(pid_t process_pid);
#endif

#ifdef WIN32
#define CHECK_INPUT_PARAM(argc, argv)
#define START_DAEMONIZE_CMD() 

#define IGNORE_LINUX_SINGLE()
#else
#define CHECK_INPUT_PARAM(argc, argv)	\
	if (!checkInputParam(argc, argv))	\
					{	\
		return 0;	\
					}

#define START_DAEMONIZE_CMD()	\
		daemon(1, 1);	\
		Redirect * redirect = new Redirect();	\
		string exe_path = getProcessDir(getpid());	\
		std::cout << "exe path is : " << exe_path.c_str() << std::endl;	\
		int ch_ret = chdir(exe_path.c_str());	\
		std::cout << " chdir result is " << ch_ret << std::endl;	\
		char curr_dir[255] = {0};	\
		getcwd((char *)curr_dir, 255);	\
		std::cout << "curr dir is : " << curr_dir << std::endl;


	#define IGNORE_LINUX_SINGLE()	\
		signal(SIGQUIT, SIG_IGN);	\
		signal(SIGPIPE, SIG_IGN);
#endif // !WIN32

class Redirect
{
public:
	Redirect();
	~Redirect();
protected:
private:
	int m_output_fd;
	int m_input_fd;
	int m_error_fd;
};

#endif
