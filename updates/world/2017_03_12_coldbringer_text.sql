DELETE FROM `script_texts` WHERE `entry` = -1129004;
DELETE FROM `script_texts` WHERE `entry` = -1129003;
DELETE FROM `script_texts` WHERE `entry` = -1129002;
DELETE FROM `script_texts` WHERE `entry` = -1129001;
DELETE FROM `script_texts` WHERE `entry` = -1129000;

INSERT INTO `script_texts` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `sound`, `type`, `language`, `emote`, `comment`) VALUES 
(-1129004, 'Too...easy!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 5826, 1, 0, 0, 'amnennar SAY_KILL'),
(-1129002, 'Come, spirits, attend your master!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 5829, 1, 0, 0, 'amnennar SAY_SUMMON30'),
(-1129003, 'I am the hand of the Lich King!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 5827, 1, 0, 0, 'amnennar SAY_HP'),
(-1129001, 'To me, my servants!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 5828, 1, 0, 0, 'amnennar SAY_SUMMON60'),
(-1129000, 'You\'ll never leave this place... alive.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 5825, 1, 0, 0, 'amnennar SAY_AGGRO');
