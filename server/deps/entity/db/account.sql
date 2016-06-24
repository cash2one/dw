DROP TABLE IF EXISTS `account`;
CREATE TABLE `account` (
    `guid` bigint(20) unsigned not null default '0' ,
    `user_name` text ,
    `batch_num` int(11) unsigned default '0' ,
    `language_type` int(11) default '0' ,
    `login_type` int(11) unsigned default '0' ,
    `channel_type` int(11) unsigned default '0' ,
    `create_tim` text ,
    `sum_id` bigint(20) unsigned not null default '0' ,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `role_server`;
CREATE TABLE `role_server` (
    `guid` bigint(20) unsigned not null default '0' ,
    `area_id` int(11) unsigned default '0' ,
    `gs_id` int(11) unsigned default '0' ,
    `user_id` bigint(20) unsigned not null default '0' ,
    `first_enter_time` text ,
    `last_enter_time` text ,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `summoner`;
CREATE TABLE `summoner` (
    `guid` bigint(20) unsigned not null default '0' ,
    `account_id` bigint(20) unsigned not null default '0' ,
    `sum_name` text ,
    `match_status` bigint(20) unsigned not null default '0' ,
    `gate_guid` bigint(20) unsigned not null default '0' ,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `friend_player_info`;
CREATE TABLE `friend_player_info` (
    `guid` bigint(20) unsigned not null default '0' ,
    `player_name` text ,
    `vr_id` int(11) unsigned default '0' ,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `friend_player_relation`;
CREATE TABLE `friend_player_relation` (
    `guid` bigint(20) unsigned not null default '0' ,
    `apply_list` blob ,
    `black_list` blob ,
    `friend_list` blob ,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `ping`;
CREATE TABLE `ping` (
  `pt` tinyint(1) NOT NULL,
  PRIMARY KEY  (`pt`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
INSERT INTO ping SET pt=1;
