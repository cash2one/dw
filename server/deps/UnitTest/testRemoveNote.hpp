#pragma  once
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>
#include <fstream>
char g_buf[5000] = { 0 };

class TestRemoveNote
{
public :
	void static  run(const std::string dir)
	{	
		try
		{
			rundir(dir);
		}
		catch (boost::filesystem::filesystem_error& err)
		{
			std::cout << err.what() << std::endl;
		}

	}

	void static rundir(const std::string dir)
	{
		boost::filesystem::path fsPath(dir);
		boost::filesystem::directory_iterator beg_iter(fsPath);
		boost::filesystem::directory_iterator end_iter;
		for (; beg_iter != end_iter; ++beg_iter)
		{
			boost::filesystem::file_status fs = boost::filesystem::status(*beg_iter);
			std::string str_name = beg_iter->path().filename().generic_string();
			if (boost::filesystem::regular_file == fs.type())
			{
				doOneFile(dir + "\\" + str_name);
			}
			else if (boost::filesystem::directory_file == fs.type())
			{
				rundir(dir + "\\" + str_name);
			}

			
		}
	}


	void static doOneFile(const std::string file_path)
	{
		std::fstream f(file_path);
		std::string output;
		size_t size = 0;
		std::vector<std::string> temp;
		bool needChange = false;

		for (int i = 0; i < 7; i++)
		{
			f.getline(g_buf, 5000);
			if (0 != strncmp(g_buf, "//", 2))
				return;

		}

		while (!f.eof())
		{
			f.getline(g_buf, 5000);
			temp.push_back(g_buf);
		}

		f.close();
		
		std::fstream out(file_path, std::ios::out | std::ios::trunc);
		for (auto& it : temp)
		{
			out << it << "\n";
		}

		out.close();
	}
};