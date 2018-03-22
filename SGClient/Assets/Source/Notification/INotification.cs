using System;

public interface INotification : IDisposable
{
	/// <summary>
	/// 注册一个推送
	/// </summary>
	/// <param name="id">消息唯一标识</param>
	/// <param name="name">消息弹出一时在手机屏幕最上方显示的文字，对苹果无效</param>
	/// <param name="title">通知栏中消息的标题</param>
	/// <param name="content">通知栏中消息的正文</param>
	/// <param name="triggerTime">触发的时间</param>
	/// <param name="repeat">是否要每日重复触发</param>
	void Register(int id, string name, string title, string content, int triggerTime, bool repeat);

	/// <summary>
	/// 取消一个推送
	/// </summary>
	/// <param name="id">消息唯一标识</param>
	void Unregister(int id);

	/// <summary>
	/// 取消所有推送
	/// </summary>
	void ClearAll();
}
