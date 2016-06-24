
#ifndef BATTLE_SETTING_HPP__
#define BATTLE_SETTING_HPP__

struct BattleSetting
{
	BattleSetting()
		: ban_timeout(15 * 1000)
		, select_hero_timeout(15 * 1000)
		, buy_skin_timeout(15 * 1000)
		, loading_timeout(30 * 1000)
		, battle_timeout(3600 * 1000)
		, all_offline_battle_timeout(600 * 1000)
		, frame_delta(50 * 1000)
		, pre_act_frames(50 * 1000)
	{}

	int ban_timeout;
	int select_hero_timeout;
	int buy_skin_timeout;
	int loading_timeout;
	int battle_timeout;
	int all_offline_battle_timeout;
	int frame_delta;
	int pre_act_frames;
};

#endif
