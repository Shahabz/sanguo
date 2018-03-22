#if UNITY_ANDROID
using System;
using UnityEngine;

public class AndroidNotification : INotification
{
	AndroidJavaObject m_javaObj = new AndroidJavaObject(DeviceHelper.AndroidPackageName+".AlarmReceiver");

	public void Register(int id, string name, string title, string content, int triggerTime, bool repeat)
	{
		m_javaObj.CallStatic("Register", new object[6]{
			id,
			name,
			title,
			content,
			triggerTime,
			repeat
		});
	}

	public void Unregister(int id)
	{
		m_javaObj.CallStatic("Unregister", id);
	}

	public void ClearAll()
	{
		/*var types = Enum.GetValues(typeof(NotificationType));
		for (int i = 0; i < types.Length * 100; i++)
			Unregister(i);*/
	}

	#region IDisposable
	public void Dispose()
	{
		Dispose(true);
		GC.SuppressFinalize(this);
	}

	protected virtual void Dispose(bool disposing)
	{
		if (disposing)
		{
			m_javaObj.Dispose();
			m_javaObj = null;
		}
	}

	~AndroidNotification()
	{
		Dispose(false);
	}
	#endregion
}

#endif