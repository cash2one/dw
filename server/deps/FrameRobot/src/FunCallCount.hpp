#pragma once

class FunCallCount
{
public:
	void reset()
	{
		search_friend_count = 0;
		add_friend_ack_count = 0;
		confirm_add_friend_count = 0;
		add_cancel_friend_count = 0;
		del_friend_count = 0;
	}

	void addSearchFriendCount()
	{
		search_friend_count++;
	}
	void addAddFriendAckCount()
	{
		add_friend_ack_count++;
	}
	void addConfirmAddFriendCount()
	{
		confirm_add_friend_count++;
	}
	void addAddCancelFriendCount()
	{
		add_cancel_friend_count++;
	}
	void addDelFriendCount()
	{
		del_friend_count++;
	}

	FunCallCount()
	{
	}

	~FunCallCount()
	{
	}

private:
	int search_friend_count = 0;
	int add_friend_ack_count = 0;
	int confirm_add_friend_count = 0;
	int add_cancel_friend_count = 0;
	int del_friend_count = 0;
};
