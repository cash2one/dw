#pragma  once
#include "typedef.h"
#include <mutex>

namespace google
{
	namespace protobuf
	{
		class Message;
	}
}

struct RequestInfost;
#define MSG_TYPE	google::protobuf::Message
typedef std::function<void(uint32_t, uint64_t, MSG_TYPE*)> LoadCallBack;

class Pool
{
public:
	MSG_TYPE* get(uint64_t guid);
	uint32_t load(uint64_t guid, LoadCallBack cb); // 从 db 加载, 返回为request id , 0则请求失败
	bool add(uint64_t guid, MSG_TYPE* msg, bool add_to_db = true);
	bool update(uint64_t guid);
	bool remove(uint64_t guid); // 从 pool 移除


	void update(); // 框架调用

private:
	void cbLoaded(RequestInfost *st);

private:
	std::map<uint64_t, MSG_TYPE*> m_entities;
	uint32_t m_index = 1;
	std::map<uint32_t, LoadCallBack> m_cb;

	std::mutex m_loaded_mutex;
	struct Loadst
	{	
		Loadst(uint32_t a_id, uint64_t a_guid, MSG_TYPE* a_msg)
		{
			id = a_id;
			guid = a_guid;
			msg = a_msg;
		}

		uint32_t id = 0;
		uint64_t guid = 0;
		MSG_TYPE* msg = NULL;
	};
	std::vector<Loadst> m_loaded;
};