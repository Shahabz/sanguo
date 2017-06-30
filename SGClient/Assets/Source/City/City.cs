using UnityEngine;
using System.Collections;

public class City : MonoBehaviour 
{
	public Transform	_buildings;
    public Camera2D     _camera;
    
	void Start()
	{
		
	}

	public void SelectBuildingLand( Transform building )
	{
		LuaFun.buildinglandselect.Call( building );
	} 

	public void SelectBuilding( Transform building )
	{
        LuaFun.buildingselect.Call( building );
    }  
}
