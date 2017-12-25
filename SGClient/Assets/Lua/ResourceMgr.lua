-- 资源管理函数
-- 载入Sprite
function LoadSprite( name )
    return ResourceManager.LoadSprite( name );
end

-- 载入Prefab
function LoadPrefab( name )
    return ResourceManager.LoadPrefab( name );
end

-- 异步载入Sprite
function LoadSpriteAsyn( name, callback )
	eye.resourceManager:LoadSpriteAsyn( name, callback );
end

-- 异步载入Prefab
function LoadPrefabAsyn( name, callback )
	eye.resourceManager:LoadPrefabAsyn( name, callback );
end

-- 玩家头像
function PlayerHeadSprite( shape )
	local name = string.format("playerhead_%02d", shape+1 );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 玩家半身像
function PlayerFaceSprite( shape )
	local name = string.format("playerhead_%02d_face", shape+1 );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 英雄头像
function HeroHeadSprite( kind )
	local name = string.format("herohead_%d", kind );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "herohead_1" );
	end
	return sprite;
end

-- 英雄半身像
function HeroFaceSprite( kind )
	local name = string.format("heroface_%d", kind );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "heroface_1" );
	end
	return sprite;
end

-- 英雄兵种icon
function CorpsSprite( corps )
	local name = "ui_icon_corps"..corps;
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 兵种半身像
function CorpsFaceSprite( corps )
	local name = string.format("ui_traindlg_corps_%d", corps+1 );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "ui_traindlg_corps_1" );
	end
	return sprite;
end

-- 兵种半身像
function CorpsFaceSpriteEx( buildingkind )
	local corps = 0;
	if buildingkind == BUILDING_Infantry or buildingkind == BUILDING_Militiaman_Infantry then
		corps = 0
	elseif buildingkind == BUILDING_Cavalry or buildingkind == BUILDING_Militiaman_Cavalry then
		corps = 1
	elseif buildingkind == BUILDING_Archer or buildingkind == BUILDING_Militiaman_Archer then
		corps = 2
	end
	return CorpsFaceSprite( corps );
end

-- 英雄颜色底板(带ab读取)
function HeroColorSprite( color )
	local name = "ui_static_colorpanel_"..color;
	ResourceManager.LoadAssetBundle( "_ab_ui_static_colorpanel" );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "ui_static_colorpanel_0" );
	end
	return sprite;
end

-- 英雄颜色底板卸载
function HeroColorSpriteUnload()
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_colorpanel" );
end

-- 守卫头像
function GuardSprite( shape )
	local name = string.format("guardhead_%d", shape );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "guardhead_1" );
	end
	return sprite;
end

-- 建筑形象
function BuildingSprite( kind )
	local name = string.format("building_kind%d", kind );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 建筑图标形象
function BuildingIconSprite( kind )
	local name = string.format("ui_main_building_%d", kind );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 道具形象
function ItemSprite( kind )
	local name = string.format("item_icon_%d", kind );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "item_icon_1" );
	end
	return sprite;
end

-- 装备形象
function EquipSprite( kind )
	local name = string.format("equip_icon_%d", kind );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "equip_icon_1" );
	end
	return sprite;
end

-- 国器形象
function NationEquipSprite( kind )
	local name = string.format("nation_equip_icon_%d", kind );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "nation_equip_icon_1" );
	end
	return sprite;
end


-- 装备缺省图标
function EquipNormalSprite( type )
	local name = string.format("ui_icon_back_equip%d", type );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "equip_icon_1" );
	end
	return sprite;
end

-- 装备洗练图标
function EquipWashSprite( ability )
	local name = string.format("ui_icon_wash_ability_%d", ability );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 道具颜色框
function ItemColorSprite( color )
	local name = string.format("ui_color_%d", color );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "ui_color_0" );
	end
	return sprite;
end

-- 国家图标
function NationSprite( nation )
	local name = string.format("ui_icon_nation%d", nation );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

--国家旗子
function NationFlagSprite( nation )
	local name = string.format("ui_icon_nationflag_%d", nation);
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 科技形象
function TechSprite( kind )
	local name = string.format("tech_icon_%d", kind );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "tech_icon_1" );
	end
	return sprite;
end

-- 官员形象
function OfSprite( shape )
	local name = string.format("ofhead_%d", shape );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "herohead_1" );
	end
	return sprite;
end

-- 敌军形象
function EnemyHeadSprite( shape )
	local name = ""
	if shape < 1000 then
		name = string.format("enemyhead_%d", shape ); -- 使用敌人专用头像
	elseif shape < 2000 then
		name = string.format("guardhead_%d", shape-1000 ); -- 使用守卫专用头像
	elseif shape < 3000 then
		name = string.format("herohead_%d", shape-2000 ); -- 使用英雄专用头像
	end
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "enemyhead_1" );
	end
	return sprite;
end

-- 资源图标通过类型
function ResIcon( type )
	local list = { "ui_icon_res_silver", "ui_icon_res_wood", "ui_icon_res_food", "ui_icon_res_iron", "ui_icon_res_token" }
	local sprite = LoadSprite( list[type] );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 资源图标通过道具kind获取
function ResKindIcon( kind )
	local list = { 
	[120]="ui_icon_res_silver", 
	[121]="ui_icon_res_wood", 
	[122]="ui_icon_res_food", 
	[123]="ui_icon_res_iron", 
	[124]="ui_icon_res_token" }
	local name = list[kind]
	if name == nil then
		name = "Char_Default"
	end
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 资源图标通过类型
function ResItemIcon( type )
	local list = { "item_icon_120", "item_icon_121", "item_icon_122", "item_icon_123", "item_icon_124" }
	local sprite = LoadSprite( list[type] );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 流寇形象
function EnemySprite( shape, frames )
	local name = string.format("map_enemy_%d_%d", shape, frames );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 爵位形象
function PlaceSprite( place )
	local name = string.format("p%d", place+1 );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 爵位形象
function PlaceRichText( place )
	local name = string.format("<icon=p%d>", place+1 );
	return name;
end