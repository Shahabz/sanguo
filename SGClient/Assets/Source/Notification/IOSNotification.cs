#if  UNITY_IPHONE || UNITY_IOS

using System;
using UnityEngine;
public class IOSNotification : INotification
{
	static IOSNotification()
	{
		Clear();
		UnityEngine.iOS.NotificationServices.RegisterForNotifications(UnityEngine.iOS.NotificationType.Alert| UnityEngine.iOS.NotificationType.Badge | UnityEngine.iOS.NotificationType.Sound);
	}

	public void Register(int id, string name, string title, string content, int triggerTime, bool repeat)
	{
		UnityEngine.iOS.LocalNotification iosNotification = new UnityEngine.iOS.LocalNotification()
		{
			alertBody = content,
			hasAction = false,
			applicationIconBadgeNumber = 1,
			fireDate = System.DateTime.Now.AddSeconds( triggerTime ),
			soundName = UnityEngine.iOS.LocalNotification.defaultSoundName,
		};

		if (repeat)
		{
			iosNotification.repeatCalendar = UnityEngine.iOS.CalendarIdentifier.ChineseCalendar;
			iosNotification.repeatInterval = UnityEngine.iOS.CalendarUnit.Day;
		}

		UnityEngine.iOS.NotificationServices.ScheduleLocalNotification(iosNotification);
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
		UnityEngine.iOS.LocalNotification ln = new UnityEngine.iOS.LocalNotification()
		{
			applicationIconBadgeNumber = -1
		};

		UnityEngine.iOS.NotificationServices.PresentLocalNotificationNow(ln);
		UnityEngine.iOS.NotificationServices.CancelAllLocalNotifications();
		UnityEngine.iOS.NotificationServices.ClearLocalNotifications();
	}

	#region INotification
	public void Dispose()
	{

	}
	#endregion
}
#endif
