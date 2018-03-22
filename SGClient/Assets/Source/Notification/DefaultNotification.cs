#if  UNITY_EDITOR
using System;
class DefaultNotification : INotification
{
	public void Register(int id, string name, string title, string content, int triggerTime, bool repeat)
	{

	}

	public void Unregister(int id)
	{

	}

	public void ClearAll()
	{

	}

	public void Dispose()
	{

	}
}
#endif