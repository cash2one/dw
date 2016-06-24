//
//#ifndef MESSAGE_PUMP_IMP_HPP__
//#define MESSAGE_PUMP_IMP_HPP__
//
//#include <unordered_map>
//#include "SessionHandle.h"
//#include "SessionServer.h"
//#include "MessagePump.h"
//
//using namespace std;
//
//template<class _PackType>
//class MessageServerPumpImp : public SessionHandle, public MessageServerPump<_PackType>
//{
//public:
//	MessageServerPumpImp();
//	~MessageServerPumpImp();
//public:
//	virtual void handleNewConnection(SessionId session_id);
//	virtual void handleNewConnectionFailed();
//	virtual void handleCloseConnection(SessionId session_id, int error_code);
//	virtual size_t handleInputStream(SessionId session_id, char * buffer, size_t buf_len);
//
//public:
//	virtual int init(MessagePumpHandle * msg_pump_handle);
//	virtual int listen(const string & ip_addr, int port);
//	virtual int connect(const string & ip_addr, int port);
//	virtual int addMsgHandle(int msg_id, PacketHandleBase<_PackType> * msg_handle);
//	virtual int update(size_t & dispatch_pack_num, size_t & dispatch_pack_bytes);
//	virtual int handleOutputStream(SessionId session_id, char * buffer, size_t buf_len);
//
//protected:
//private:
//	typedef unordered_map<int, PacketHandleBase *> PacketHandleMap_t;
//
//	SessionServer * m_server_session;
//
//	MessagePumpHandle * m_msg_pump_handle;
//
//	PacketHandleMap_t m_msg_handle_map;
//
//	InterPacketVec_t m_input_packet;
//
//	boost::mutex m_input_packet_mutex;
//
//};
//
//template<class _PackType>
//MessageServerPumpImp<_PackType>::MessageServerPumpImp()
//{
//
//}
//
//template<class _PackType>
//MessageServerPumpImp<_PackType>::~MessageServerPumpImp()
//{
//
//}
//
//
//template<class _PackType>
//void MessageServerPumpImp<_PackType>::handleNewConnection(SessionId session_id)
//{
//
//}
//
//template<class _PackType>
//void MessageServerPumpImp<_PackType>::handleNewConnectionFailed()
//{
//
//}
//
//template<class _PackType>
//void MessageServerPumpImp<_PackType>::handleCloseConnection(SessionId session_id, int error_code)
//{
//
//}
//
//template<class _PackType>
//size_t MessageServerPumpImp<_PackType>::handleInputStream(SessionId session_id, char * buffer, size_t buf_len)
//{
//	return 0;
//}
//
//template<class _PackType>
//int MessageServerPumpImp<_PackType>::init(MessagePumpHandle * msg_pump_handle)
//{
//	return 0;
//}
//
//template<class _PackType>
//int MessageServerPumpImp<_PackType>::listen(const string & ip_addr, int port)
//{
//	return 0;
//}
//
//template<class _PackType>
//int MessageServerPumpImp<_PackType>::connect(const string & ip_addr, int port)
//{
//	return 0;
//}
//
//template<class _PackType>
//int MessageServerPumpImp<_PackType>::addMsgHandle(int msg_id, PacketHandleBase<_PackType> * msg_handle)
//{
//	return 0;
//}
//
//template<class _PackType>
//int MessageServerPumpImp<_PackType>::update(size_t & dispatch_pack_num, size_t & dispatch_pack_bytes)
//{
//	return 0;
//
//}
//
//template<class _PackType>
//int MessageServerPumpImp<_PackType>::handleOutputStream(SessionId session_id, char * buffer, size_t buf_len)
//{
//	return 0;
//}
//
//#endif
