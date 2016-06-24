#include <string>
#include <vector>
#include <flatbuffers/flatbuffers.h>#include "friend_generated.h"
#include "Packaet_.h"

using namespace std;
using namespace MobaGo::FlatBuffer;
using namespace flatbuffers;


InterPacket * makeFriendInfo(int msg_id, uint64_t guid, uint64_t friend_id, uint32_t head_id, uint8_t status);

InterPacket * makeMsgAddFriendReq(int msg_id, uint64_t guid, uint64_t friend_id);

InterPacket * makeMsgAddFriendAck(int msg_id, uint64_t guid, uint8_t add_friend_result);

InterPacket * makeMsgDelFriendReq(int msg_id, uint64_t guid, uint64_t friend_id);

InterPacket * makeMsgDelFriendAck(int msg_id, uint64_t guid, uint8_t del_friend_result);

InterPacket * makeMsgConfirmAddFriendReq(int msg_id, uint64_t guid, uint64_t friend_id);

InterPacket * makeMsgAddCancelFriendReq(int msg_id, uint64_t guid, uint64_t friend_id);

InterPacket * makeMsgAddCancelFriendAck(int msg_id, uint64_t guid, uint8_t confirm_cancel_result);

InterPacket * makeMsgSearchFriendReq(int msg_id, uint64_t guid, const string & friend_name);

InterPacket * makeMsgVRFriendOperateReq(int msg_id, uint64_t guid, uint64_t friend_id, uint8_t operate_type);

InterPacket * makeMsgFriendDetailInfoReq(int msg_id, uint64_t guid, uint64_t friend_id);

InterPacket * makeFriendStatus(int msg_id, uint64_t guid, uint64_t friend_id, int8_t status);


