using System;
using UnityEngine;
public class NotificationManager : IDisposable
{
	INotification m_notification;

	#region Singleton
	static NotificationManager s_instance;
	public static NotificationManager Instance
	{
		get
		{
			if (s_instance == null)
				s_instance = new NotificationManager();
			return s_instance;
		}
	}
	#endregion

	private NotificationManager()
	{
		m_notification = CreateObj();
	}

	INotification CreateObj()
	{
		INotification obj;
#if UNITY_EDITOR
		obj = new DefaultNotification();
#elif UNITY_ANDROID
		obj = new AndroidNotification();
#elif  UNITY_IPHONE || UNITY_IOS
		obj = new IOSNotification();
#else
		obj = new DefaultNotification();
		Debugger.LogWarning("Local push not support this platform");
#endif
		return obj;
	}

	public void Register(int id, string content, int triggerTime, bool repeat)
	{
		if (string.IsNullOrEmpty(content))
			throw new ArgumentException("content");

		string title = Application.productName;      // 游戏名
		if (string.IsNullOrEmpty(title))
			throw new ArgumentException("title");

		m_notification.Register(id, content, title, content, triggerTime, repeat);
		Debug.Log(string.Format("local push, id: {0}, title: {1},  content: {2},  Time: {3}  repeat: {4}", id, title, content, triggerTime, repeat));
	}

	public void Unregister(int id)
	{
		m_notification.Unregister(id);
	}

	public void ClearAll()
	{
		m_notification.ClearAll();
	}

	public void Dispose()
	{
		m_notification.Dispose();
	}
}
