-- cleanup
DELETE FROM `gossip_menu` where entry = 60001;
DELETE FROM `gossip_menu_option` where box_text = 'Immersive';

-- root menu
INSERT INTO `gossip_menu` (`entry`, `text_id`, `script_id`, `condition_id`)
VALUES (60001, 1, 0, 0);

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60001, 0, 0, 'Help', 18, 16, 0, 1, 0, 0, 0, 'Immersive', 0);

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60001, 1, 0, '+1 Strength', 18, 16, 0, 1, 0, 0, 0, 'Immersive', 0);

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60001, 2, 0, '+1 Agility', 18, 16, 0, 2, 0, 0, 0, 'Immersive', 0);

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60001, 3, 0, '+1 Stamina', 18, 16, 0, 3, 0, 0, 0, 'Immersive', 0);

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60001, 4, 0, '+1 Intellect', 18, 16, 0, 4, 0, 0, 0, 'Immersive', 0);

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60001, 5, 0, '+1 Spirit', 18, 16, 0, 5, 0, 0, 0, 'Immersive', 0);

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60001, 6, 0, 'Unlearn all attributes', 18, 16, 0, 6, 0, 0, 0, 'Immersive', 0);

-- add to trainers
INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`, `box_money`, `box_text`, `condition_id`) 
SELECT menu_id, 61, 0, 'Increase attributes', 1, 16, 60001, 0, 0, 0, 0, 'Immersive', 0 FROM `gossip_menu_option` where `action_menu_id` = 4461;

