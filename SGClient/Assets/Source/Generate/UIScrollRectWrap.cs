﻿//this source code was auto-generated by tolua#, do not modify it
using System;
using LuaInterface;

public class UIScrollRectWrap
{
	public static void Register(LuaState L)
	{
		L.BeginClass(typeof(UIScrollRect), typeof(UnityEngine.UI.ScrollRect));
		L.RegFunction("OnBeginDrag", OnBeginDrag);
		L.RegFunction("OnDrag", OnDrag);
		L.RegFunction("OnEndDrag", OnEndDrag);
		L.RegFunction("ResetScroll", ResetScroll);
		L.RegFunction("SetContentPosition", SetContentPosition);
		L.RegFunction("ScrollToBottom", ScrollToBottom);
		L.RegFunction("ScrollToTop", ScrollToTop);
		L.RegFunction("clearChildrenPos", clearChildrenPos);
		L.RegFunction("CenterOnItem", CenterOnItem);
		L.RegFunction("ShowLoading", ShowLoading);
		L.RegFunction("HideLoading", HideLoading);
		L.RegFunction("IsShowLoading", IsShowLoading);
		L.RegFunction("__eq", op_Equality);
		L.RegFunction("__tostring", ToLua.op_ToString);
		L.RegVar("controlID", get_controlID, set_controlID);
		L.RegVar("uiMod", get_uiMod, set_uiMod);
		L.RegVar("enableOnDragEvent", get_enableOnDragEvent, set_enableOnDragEvent);
		L.RegVar("enableDrag", get_enableDrag, set_enableDrag);
		L.EndClass();
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int OnBeginDrag(IntPtr L)
	{
		try
		{
			ToLua.CheckArgsCount(L, 2);
			UIScrollRect obj = (UIScrollRect)ToLua.CheckObject(L, 1, typeof(UIScrollRect));
			UnityEngine.EventSystems.PointerEventData arg0 = (UnityEngine.EventSystems.PointerEventData)ToLua.CheckObject(L, 2, typeof(UnityEngine.EventSystems.PointerEventData));
			obj.OnBeginDrag(arg0);
			return 0;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int OnDrag(IntPtr L)
	{
		try
		{
			ToLua.CheckArgsCount(L, 2);
			UIScrollRect obj = (UIScrollRect)ToLua.CheckObject(L, 1, typeof(UIScrollRect));
			UnityEngine.EventSystems.PointerEventData arg0 = (UnityEngine.EventSystems.PointerEventData)ToLua.CheckObject(L, 2, typeof(UnityEngine.EventSystems.PointerEventData));
			obj.OnDrag(arg0);
			return 0;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int OnEndDrag(IntPtr L)
	{
		try
		{
			ToLua.CheckArgsCount(L, 2);
			UIScrollRect obj = (UIScrollRect)ToLua.CheckObject(L, 1, typeof(UIScrollRect));
			UnityEngine.EventSystems.PointerEventData arg0 = (UnityEngine.EventSystems.PointerEventData)ToLua.CheckObject(L, 2, typeof(UnityEngine.EventSystems.PointerEventData));
			obj.OnEndDrag(arg0);
			return 0;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int ResetScroll(IntPtr L)
	{
		try
		{
			ToLua.CheckArgsCount(L, 1);
			UIScrollRect obj = (UIScrollRect)ToLua.CheckObject(L, 1, typeof(UIScrollRect));
			obj.ResetScroll();
			return 0;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int SetContentPosition(IntPtr L)
	{
		try
		{
			ToLua.CheckArgsCount(L, 2);
			UIScrollRect obj = (UIScrollRect)ToLua.CheckObject(L, 1, typeof(UIScrollRect));
			UnityEngine.Vector2 arg0 = ToLua.ToVector2(L, 2);
			obj.SetContentPosition(arg0);
			return 0;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int ScrollToBottom(IntPtr L)
	{
		try
		{
			int count = LuaDLL.lua_gettop(L);

			if (count == 1 && TypeChecker.CheckTypes(L, 1, typeof(UIScrollRect)))
			{
				UIScrollRect obj = (UIScrollRect)ToLua.ToObject(L, 1);
				obj.ScrollToBottom();
				return 0;
			}
			else if (count == 2 && TypeChecker.CheckTypes(L, 1, typeof(UIScrollRect), typeof(int)))
			{
				UIScrollRect obj = (UIScrollRect)ToLua.ToObject(L, 1);
				int arg0 = (int)LuaDLL.lua_tonumber(L, 2);
				obj.ScrollToBottom(arg0);
				return 0;
			}
			else
			{
				return LuaDLL.luaL_throw(L, "invalid arguments to method: UIScrollRect.ScrollToBottom");
			}
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int ScrollToTop(IntPtr L)
	{
		try
		{
			int count = LuaDLL.lua_gettop(L);

			if (count == 1 && TypeChecker.CheckTypes(L, 1, typeof(UIScrollRect)))
			{
				UIScrollRect obj = (UIScrollRect)ToLua.ToObject(L, 1);
				obj.ScrollToTop();
				return 0;
			}
			else if (count == 2 && TypeChecker.CheckTypes(L, 1, typeof(UIScrollRect), typeof(int)))
			{
				UIScrollRect obj = (UIScrollRect)ToLua.ToObject(L, 1);
				int arg0 = (int)LuaDLL.lua_tonumber(L, 2);
				obj.ScrollToTop(arg0);
				return 0;
			}
			else
			{
				return LuaDLL.luaL_throw(L, "invalid arguments to method: UIScrollRect.ScrollToTop");
			}
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int clearChildrenPos(IntPtr L)
	{
		try
		{
			ToLua.CheckArgsCount(L, 1);
			UIScrollRect obj = (UIScrollRect)ToLua.CheckObject(L, 1, typeof(UIScrollRect));
			obj.clearChildrenPos();
			return 0;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int CenterOnItem(IntPtr L)
	{
		try
		{
			ToLua.CheckArgsCount(L, 3);
			UIScrollRect obj = (UIScrollRect)ToLua.CheckObject(L, 1, typeof(UIScrollRect));
			int arg0 = (int)LuaDLL.luaL_checknumber(L, 2);
			UnityEngine.GameObject arg1 = (UnityEngine.GameObject)ToLua.CheckUnityObject(L, 3, typeof(UnityEngine.GameObject));
			obj.CenterOnItem(arg0, arg1);
			return 0;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int ShowLoading(IntPtr L)
	{
		try
		{
			ToLua.CheckArgsCount(L, 3);
			UIScrollRect obj = (UIScrollRect)ToLua.CheckObject(L, 1, typeof(UIScrollRect));
			bool arg0 = LuaDLL.luaL_checkboolean(L, 2);
			float arg1 = (float)LuaDLL.luaL_checknumber(L, 3);
			obj.ShowLoading(arg0, arg1);
			return 0;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int HideLoading(IntPtr L)
	{
		try
		{
			ToLua.CheckArgsCount(L, 1);
			UIScrollRect obj = (UIScrollRect)ToLua.CheckObject(L, 1, typeof(UIScrollRect));
			obj.HideLoading();
			return 0;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int IsShowLoading(IntPtr L)
	{
		try
		{
			ToLua.CheckArgsCount(L, 1);
			UIScrollRect obj = (UIScrollRect)ToLua.CheckObject(L, 1, typeof(UIScrollRect));
			bool o = obj.IsShowLoading();
			LuaDLL.lua_pushboolean(L, o);
			return 1;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int op_Equality(IntPtr L)
	{
		try
		{
			ToLua.CheckArgsCount(L, 2);
			UnityEngine.Object arg0 = (UnityEngine.Object)ToLua.ToObject(L, 1);
			UnityEngine.Object arg1 = (UnityEngine.Object)ToLua.ToObject(L, 2);
			bool o = arg0 == arg1;
			LuaDLL.lua_pushboolean(L, o);
			return 1;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int get_controlID(IntPtr L)
	{
		object o = null;

		try
		{
			o = ToLua.ToObject(L, 1);
			UIScrollRect obj = (UIScrollRect)o;
			int ret = obj.controlID;
			LuaDLL.lua_pushinteger(L, ret);
			return 1;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e, o == null ? "attempt to index controlID on a nil value" : e.Message);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int get_uiMod(IntPtr L)
	{
		object o = null;

		try
		{
			o = ToLua.ToObject(L, 1);
			UIScrollRect obj = (UIScrollRect)o;
			UIMod ret = obj.uiMod;
			ToLua.Push(L, ret);
			return 1;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e, o == null ? "attempt to index uiMod on a nil value" : e.Message);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int get_enableOnDragEvent(IntPtr L)
	{
		object o = null;

		try
		{
			o = ToLua.ToObject(L, 1);
			UIScrollRect obj = (UIScrollRect)o;
			bool ret = obj.enableOnDragEvent;
			LuaDLL.lua_pushboolean(L, ret);
			return 1;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e, o == null ? "attempt to index enableOnDragEvent on a nil value" : e.Message);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int get_enableDrag(IntPtr L)
	{
		object o = null;

		try
		{
			o = ToLua.ToObject(L, 1);
			UIScrollRect obj = (UIScrollRect)o;
			bool ret = obj.enableDrag;
			LuaDLL.lua_pushboolean(L, ret);
			return 1;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e, o == null ? "attempt to index enableDrag on a nil value" : e.Message);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int set_controlID(IntPtr L)
	{
		object o = null;

		try
		{
			o = ToLua.ToObject(L, 1);
			UIScrollRect obj = (UIScrollRect)o;
			int arg0 = (int)LuaDLL.luaL_checknumber(L, 2);
			obj.controlID = arg0;
			return 0;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e, o == null ? "attempt to index controlID on a nil value" : e.Message);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int set_uiMod(IntPtr L)
	{
		object o = null;

		try
		{
			o = ToLua.ToObject(L, 1);
			UIScrollRect obj = (UIScrollRect)o;
			UIMod arg0 = (UIMod)ToLua.CheckUnityObject(L, 2, typeof(UIMod));
			obj.uiMod = arg0;
			return 0;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e, o == null ? "attempt to index uiMod on a nil value" : e.Message);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int set_enableOnDragEvent(IntPtr L)
	{
		object o = null;

		try
		{
			o = ToLua.ToObject(L, 1);
			UIScrollRect obj = (UIScrollRect)o;
			bool arg0 = LuaDLL.luaL_checkboolean(L, 2);
			obj.enableOnDragEvent = arg0;
			return 0;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e, o == null ? "attempt to index enableOnDragEvent on a nil value" : e.Message);
		}
	}

	[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int set_enableDrag(IntPtr L)
	{
		object o = null;

		try
		{
			o = ToLua.ToObject(L, 1);
			UIScrollRect obj = (UIScrollRect)o;
			bool arg0 = LuaDLL.luaL_checkboolean(L, 2);
			obj.enableDrag = arg0;
			return 0;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e, o == null ? "attempt to index enableDrag on a nil value" : e.Message);
		}
	}
}

