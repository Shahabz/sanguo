#if  UNITY_IPHONE

using System;
using UnityEngine;

public class IOSNotification : INotification
{
	static IOSNotification()
	{
		Clear();
		NotificationServices.RegisterForLocalNotificationTypes(LocalNotificationType.Alert);
	}

	public void Register(int id, string name, string title, string content, int triggerTime, bool repeat)
	{
		var iosNotification = new LocalNotification()
		{
			alertBody = content,
			hasAction = false,
			applicationIconBadgeNumber = 1,
			fireDate = System.DateTime.Now.AddSeconds( triggerTime ),
			soundName = LocalNotification.defaultSoundName,
		};

		if (repeat)
		{
			iosNotification.repeatCalendar = CalendarIdentifier.ChineseCalendar;
			iosNotification.repeatInterval = CalendarUnit.Day;
		}

		NotificationServices.ScheduleLocalNotification(iosNotification);
	}

	public void Unregister(int id)
	{

	}

	public void ClearAll()
	{
		Clear();
	}

	static void Clear()
	{
		var ln = new LocalNotification()
		{
			applicationIconBadgeNumber = -1
		};

		NotificationServices.PresentLocalNotificationNow(ln);
		NotificationServices.CancelAllLocalNotifications();
		NotificationServices.ClearLocalNotifications();
	}

	#region INotification
	public void Dispose()
	{

	}
	#endregion
}
#endif
