MainCity = {};
MainCity.m_Camera = nil;
MainCity.m_LastSelect = nil;

-- 初始化
function MainCity.Init()
	
end

-- 选择建筑
function MainCity.BuildingSelect( transform )
	if MainCity.m_LastSelect ~= nil then
		-- 点击相同对象直接返回
		if MainCity.m_LastSelect == transform then
			return;
		end
		-- 关闭之前渐变动画
		MainCity.m_LastSelect:GetComponent("UITweenColor"):Kill(true);
		MainCity.m_LastSelect = nil;
	end
	
	local building = nil;	
	if transform == nil then
		
		return;
	else

		building = transform:GetComponent("CityBuilding");
		MainCity.m_LastSelect = transform;
		MainCity.m_LastSelect:GetComponent("UITweenColor"):Play(true);
	end
	
	
end