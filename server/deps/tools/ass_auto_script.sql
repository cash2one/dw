DROP TABLE IF EXISTS `Account`;
CREATE TABLE `Account` (
    `guid` bigint(20) unsigned not null default '0' ,
    `user_name` text ,
    `batch_num` int(11) unsigned default '0' ,
    `language_type` int(11) default '0' ,
    `login_type` int(11) unsigned default '0' ,
    `channel_type` int(11) unsigned default '0' ,
    `create_tim` text ,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `RoleServer`;
CREATE TABLE `RoleServer` (
    `guid` bigint(20) unsigned not null default '0' ,
    `area_id` int(11) unsigned default '0' ,
    `gs_id` int(11) unsigned default '0' ,
    `user_id` bigint(20) unsigned not null default '0' ,
    `first_enter_time` text ,
    `last_enter_time` text ,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `ping`;
CREATE TABLE `ping` (
  `pt` tinyint(1) NOT NULL,
  PRIMARY KEY  (`pt`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
INSERT INTO ping SET pt=1;
