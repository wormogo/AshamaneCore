DELETE FROM gameobject WHERE guid = 184384 AND id = 142075;
UPDATE creature_addon SET path_id = 1802240, emote = 0 WHERE guid = 180224;
UPDATE creature SET position_x = -9462.29, position_y = 87.8151, position_z = 58.4211, orientation = 0.733038, spawndist = 0, MovementType = 2 WHERE guid = 180224;
UPDATE creature_template SET AIName = "SmartAI" WHERE entry = 2046;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 2046 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (2046*100, 2046*100+1) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(2046, 0, 0, 0, 34, 0, 100, 0, 2, 1, 0, 0, 80, 2046*100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Andrew Krighton - On WP 1 Reached - Start Script"),
(2046, 0, 1, 0, 34, 0, 100, 0, 2, 3, 0, 0, 80, 2046*100+1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Andrew Krighton - On WP 3 Reached - Start Script"),
(2046*100, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 2.37365, "Andrew Krighton - On Script - Set Orientation (2.37365)"),
(2046*100, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 17, 233, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Andrew Krighton - On Script - Set Emote State (233)"),
(2046*100, 9, 2, 0, 0, 0, 100, 0, 17000, 17000, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Andrew Krighton - On Script - Set Emote State (0)"),
(2046*100+1, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0.733038, "Andrew Krighton - On Script - Set Orientation (0.733038)");
DELETE FROM waypoint_data WHERE id = 1802240;
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z, delay) VALUES
(1802240, 1, -9464.05957, 88.933998, 58.3437, 0),
(1802240, 2, -9464.05957, 88.933998, 58.3437, 20000),
(1802240, 3, -9462.29, 87.8151, 58.4211, 0),
(1802240, 4, -9462.29, 87.8151, 58.4211, 60000);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid = 177313 AND id = 46982;
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274684, 274851, 274366, 274636, 274555, 274809, 274883, 274849, 274686, 274374, 274884, 274846, 274691, 274378, 274377);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274770, 274431, 274816, 274815, 274808, 274381, 274380, 274383);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274735, 274369, 274848, 274364, 274885, 274631, 274553, 274633, 274632, 274688, 274733, 274556);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274635, 274685, 274728, 274813, 274371, 274807, 274806, 274774, 274845);
DELETE FROM creature WHERE guid = 318123 AND id = 20075;
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274692, 274850, 274768, 274557, 274854, 274637, 274847, 274810, 274730, 274689);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274551, 274771, 274769, 274729, 274552, 274772, 274773, 274844, 274687);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE guid IN (274640, 274367, 274736, 274554, 274690, 274365, 274638, 274811, 274732, 274370, 274734, 274775, 274778, 274639, 274812);
UPDATE creature_addon SET path_id = 1776330, emote = 0 WHERE guid = 177633;
UPDATE creature SET position_x = -9464.87, position_y = 93.9896, position_z = 58.5271, orientation = 0, spawndist = 0, MovementType = 2 WHERE guid = 177633;
UPDATE creature_template SET AIName = "SmartAI" WHERE entry = 54;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 54 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (54*100, 54*100+1) AND `source_type` = 9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(54, 0, 0, 0, 34, 0, 100, 0, 2, 1, 0, 0, 80, 54*100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On WP 1 Reached - Start Script"),
(54, 0, 1, 0, 34, 0, 100, 0, 2, 3, 0, 0, 80, 54*100+1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On WP 3 Reached - Start Script"),
(54*100, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0.811598, "Corina Steele - On Script - Set Orientation (0.811598)"),
(54*100, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 17, 233, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On Script - Set Emote State (233)"),
(54*100, 9, 2, 0, 0, 0, 100, 0, 27000, 27000, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On Script - Set Emote State (0)"),
(54*100+1, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 3.124597, "Corina Steele - On Script - Set Orientation (3.124597)"),
(54*100+1, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 17, 69, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On Script - Set Emote State (69)"),
(54*100+1, 9, 2, 0, 0, 0, 100, 0, 16000, 16000, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Corina Steele - On Script - Set Emote State (0)"),
(54*100+1, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0.00000, "Corina Steele - On Script - Set Orientation (0.00000)");
DELETE FROM waypoint_data WHERE id = 1776330;
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z, delay) VALUES
(1776330, 1, -9460.31543, 101.147774, 58.344334, 0),
(1776330, 2, -9460.31543, 101.147774, 58.344334, 30000),
(1776330, 3, -9464.87, 93.9896, 58.5271, 0),
(1776330, 4, -9464.87, 93.9896, 58.5271, 80000);
