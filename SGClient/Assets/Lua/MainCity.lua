MainCity = {};

function MainCity.BuildingSelect( obj )
	local ShareData = obj:GetComponent("ShareData");
	if ShareData == nil then
		return
	end
	gamelog( ShareData.stringValue[0] )
	--print( ShareData.stringValue[0] )
	
end