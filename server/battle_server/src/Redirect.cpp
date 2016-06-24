
#include <vector>
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
#include "Redirect.h"

#ifndef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

using namespace std;

#ifndef WIN32
bool isProcess(string & str_value)
{
	if (str_value.size() == 0)
	{
		return false;
	}

	for (int i = 0; i < str_value.size(); ++i)
	{
		if (str_value[i] < '0' || str_value[i] > '9')
		{
			return false;
		}
	}

	return true;
}

void getAllProcessPid(vector<pid_t> & process_pid_vec)
{
	//std::cout << "get all process" << std::endl;
	boost::filesystem::path process_path("/proc");
	for (boost::filesystem::directory_iterator it(process_path); it != boost::filesystem::directory_iterator(); ++it) 
	{
		const boost::filesystem::path src_path = it->path(); 
		//std::cout << " src path is : " << it->path().string().c_str() << std::endl;
		//if (boost::filesystem::is_directory(src_path))
		{
			string file_name = it->path().leaf().string();
			//std::cout << " file name is : " << file_name.c_str() << std::endl;
			if (isProcess(file_name))
			{
				process_pid_vec.push_back(atoi(file_name.c_str()));
			}
		}
	}
}

string getProcessPath(pid_t process_pid)
{
	int str_len = 200;
	char curr_process_exe[200] = {0};
	char curr_process_name[200] = {0};
	sprintf(curr_process_exe, "/proc/%d/exe", process_pid);
	
	str_len = readlink(curr_process_exe, curr_process_name, str_len - 1);
	if (str_len < 0)
	{
		//std::cout << "failed to get current process name" << std::endl;
		return "";
	}

	return curr_process_name;
}

string getProcessDir(pid_t process_pid)
{
	int str_len = 200;
	char curr_process_exe[200] = {0};
	char curr_process_name[200] = {0};
	sprintf(curr_process_exe, "/proc/%d/exe", process_pid);

	str_len = readlink(curr_process_exe, curr_process_name, str_len - 1);
	if (str_len < 0)
	{
		//std::cout << "failed to get current process name" << std::endl;
		return "";
	}

	int len = strlen(curr_process_name);
	for (int i = len - 1; i >= 0; --i)
	{
		if ('/' == curr_process_name[i])
		{
			curr_process_name[i] = '\0';
			break;
		}
	}

	return curr_process_name;
}

pid_t getRunningServerPid()
{
	string curr_process_name = getProcessPath(getpid());

	//std::cout << "curr process path is : " << curr_process_name.c_str() << std::endl;

	if (curr_process_name.size() == 0)
	{
		std::cout << "process path is : " << curr_process_name << std::endl;
		return 0;
	}

	vector<pid_t> process_pid_vec;
	getAllProcessPid(process_pid_vec);

	//std::cout << "process size is : " << process_pid_vec.size() << std::endl;

	for (auto tmp_pid : process_pid_vec)
	{
		if (getpid() == tmp_pid)
		{
			continue;
		}

		string tmp_process_name = getProcessPath(tmp_pid);
		if (curr_process_name == tmp_process_name)
		{
			return tmp_pid;
		}
	}

	return 0;
}
#endif

void stopServer()
{
#ifdef WIN32
	return;
#else
	pid_t running_svr_pid = getRunningServerPid();
	if (0 == running_svr_pid)
	{
		std::cout << "the server is not running" << std::endl;
		return;
	}

	int ret = kill(running_svr_pid, 9);

	std::cout << "killed the server : " << running_svr_pid << std::endl;
#endif
}

void checkServerStatus()
{
#ifdef WIN32
	return;
#else
	pid_t running_svr_pid = getRunningServerPid();
	if (0 == running_svr_pid)
	{
		std::cout << "the server is not running" << std::endl;
	}
	else
	{
		std::cout << "the server is running" << std::endl;
	}
#endif
}

bool checkInputParam(int argc, char * argv[])
{
#ifdef WIN32
	return true;
#else
	if (argc < 2)
	{
		std::cout << "please input param : start / stop /status" << std::endl;
		return false;
	}

	// change current dir
	string exe_path = getProcessDir(getpid());
	//std::cout << "change working dir : " << exe_path.c_str() << std::endl;
	chdir(exe_path.c_str());

	if (strcmp(argv[1], "start") == 0)
	{
		pid_t running_svr_pid = getRunningServerPid();
		if (0 == running_svr_pid)
		{
			return true;
		}
		else
		{
			std::cout << "the server is already running" << std::endl;
			return false;
		}
	}
	else if (strcmp(argv[1], "stop") == 0)
	{
		stopServer();
		return false;
	}
	else if (strcmp(argv[1], "status") == 0)
	{
		checkServerStatus();
		return false;
	}
	else
	{
		std::cout << "please input param : start / stop /status" << std::endl;
		return false;
	}
#endif
}

Redirect::Redirect()
: m_output_fd(0)
, m_input_fd(0)
, m_error_fd(0)
{
#ifndef WIN32
	boost::filesystem::path path("logs");
	if (!boost::filesystem::exists(path))
	{
		boost::filesystem::create_directory(path);
	}

	m_output_fd = open("./logs/output.txt", O_WRONLY | O_CREAT, 0600);

	m_error_fd = open("./logs/error.txt", O_WRONLY | O_CREAT, 0600);

	m_input_fd = open("./logs/input.txt", O_RDONLY | O_CREAT, 0600);

	close(0);
	close(1);
	close(2);

	dup2(m_input_fd, 0);
	dup2(m_output_fd, 1);
	dup2(m_error_fd, 2);
#endif
}

Redirect::~Redirect()
{
#ifndef WIN32
	close(m_output_fd);
	close(m_input_fd);
	close(m_error_fd);
#endif
}
