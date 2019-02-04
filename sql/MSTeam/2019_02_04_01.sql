UPDATE creature_template SET scriptname='npc_hunter_talua', `gossip_menu_id`='19868' WHERE entry=108868;
UPDATE `creature_template` SET npcflag=0, vehicleid='4839', scriptname='npc_great_eagle', inhabittype=4 WHERE `entry`='108853';
DELETE FROM `gossip_menu_option` WHERE menuid=19868;
INSERT INTO `gossip_menu_option` (`menuid`, `OptionIndex`, `optionicon`, `optiontext`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcFlag`) VALUES
(19868, 0, 0, 'I need to fly to the Trueshat Lodge.', 0, 1, 1);