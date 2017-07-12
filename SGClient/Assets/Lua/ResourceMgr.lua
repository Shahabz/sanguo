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
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 英雄半身像
function HeroFaceSprite( kind )
	local name = string.format("herohead_%d_face", kind );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 英雄兵种icon
function CorpsSprite( corps )
	local name = string.format("ui_icon_corps%d", corps );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 英雄颜色框
function HeroColorSprite( color )
	local name = string.format("ui_icon_herocolor%d", color );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 守卫头像
function GuardSprite( shape )
	local name = string.format("guardhead_%d", shape );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
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

-- 道具形象
function ItemSprite( kind )
	local name = string.format("item_icon_%d", kind );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 装备形象
function EquipSprite( kind )
	local name = string.format("equip_icon_%d", kind );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 道具颜色框
function ItemColorSprite( color )
	local name = string.format("ui_icon_itemcolor%d", color );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end


