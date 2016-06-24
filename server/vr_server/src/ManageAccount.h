
#ifndef MANAGE_ACCOUNT_HPP__
#define MANAGE_ACCOUNT_HPP__

#ifdef WIN32
#include <WinSock2.h>
#endif // WIN32

#include <string>
#include "Packet_.h"
#include "Account.h"
#include "login_generated.h"
#include "VrConfig.h"
#include <mysql++/mysql++.h>

using namespace std;


typedef struct _VERIFYING_ACCOUNT_INFO
{
	_VERIFYING_ACCOUNT_INFO() :account_id(0) {};
	MyGuid_t account_id;		//账号id
	string account_name;		//账号名字
	string session_code;		//验证码
	// long long llExpireTime;	// 失效时间 to do list
}VERIFYING_ACCOUNT_INFO;

class ManageAccount
{
public:
	typedef map<MyGuid_t, VERIFYING_ACCOUNT_INFO> MAP_VERIFYING_ACCOUNT;

public:
	ManageAccount();
	~ManageAccount();
public:
	
	static ManageAccount * GetInstance();

	//vrpublic 账号验证信息
	bool checkSessionCode(MyGuid_t account_id, string session_code);
	bool newSessionCode(MyGuid_t account_id, string account_name, string session_code);

	uint64_t querySumIdByAccountId(uint64_t account_id);

	// account处理
	Account * addAccount(uint64_t client_id, Account *account);
	void delAccount(uint64_t client_id);
	Account *findAccount(uint64_t client_id);
	Account *getAccount(uint64_t account_id);

protected:
public:
	static ManageAccount * m_instance;

private:
	MAP_VERIFYING_ACCOUNT map_verifying_account;    // vrpublic发给vr需要验证的账号信息，只有account_name, account_id, session_code 

	map<uint64_t, Account *> map_account;
};

#endif
