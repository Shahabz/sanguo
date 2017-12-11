using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public class AudioManager : MonoBehaviour
{
    public delegate void SwitchChanged(AudioType type, bool isEnable);

    public static SwitchChanged OnSwitchChanged;

    public enum AudioType
    {
        UI = 1,
        BGM = 2,
        EFFECT = 3,
        EVENT = 4,
    }
    /// <summary>
    /// 配置数据
    /// </summary>
    struct AudioConfig
    {
        // 类别
        public AudioType Type;
        // 资源名称
        public string ResName;
        // 是否叠加执行
        public bool IsOverlay;
    }

    /// <summary>
    /// 整体音量
    /// 0-1
    /// </summary>
    public float MainVoice
    {
        set
        {
            _MainVoice = value;
        }
        get
        {
            return _MainVoice;
        }

    }
    private float _MainVoice = 1.0f;

    public Dictionary<AudioType, bool> AudioSwitches
    {
        get
        {
            return _AudioSwitches;
        }
    }
    private Dictionary<AudioType, bool> _AudioSwitches = new Dictionary<AudioType, bool>();
    private Dictionary<AudioType, AudioSourceAgent> _AudioSources = new Dictionary<AudioType, AudioSourceAgent>();

    // 缓存数据
    Dictionary<int, AudioClip> _Caches = new Dictionary<int, AudioClip>();
    // 配置
    Dictionary<int, AudioConfig> _Configs = new Dictionary<int, AudioConfig>();

    public Transform AudioRoot
    {
        get
        {
            return _AudioRoot;
        }
    }
    private Transform _AudioRoot;

    void Awake()
    {
        Init();
    }

    public void OnDestroy()
    {
        OnSwitchChanged = null;
    }


    /// <summary>
    /// 初始化
    /// </summary>
    private void Init()
    {

        for (int i = 1; i <= 4; i++)
        {
            _AudioSwitches.Add((AudioType)i, true);
            _AudioSources.Add((AudioType)i, null);
        }

        _AudioRoot = GameObject.Find("Audio").transform;
        _AudioSources[AudioType.UI] = _AudioRoot.Find("Audio_UI").GetComponent<AudioSourceAgent>();
        _AudioSources[AudioType.BGM] = _AudioRoot.Find("Audio_BGM").GetComponent<AudioSourceAgent>();
		_AudioSources[AudioType.EFFECT] = _AudioRoot.Find("Audio_Effect").GetComponent<AudioSourceAgent>();
		_AudioSources[AudioType.EVENT] = _AudioRoot.Find("Audio_Event").GetComponent<AudioSourceAgent>();

        /*GameObject go = new GameObject("Audio_Effect");
        go.transform.SetParent(_AudioRoot);
        _AudioSources[AudioType.EFFECT] = go.AddComponent<AudioSourceAgent>();
        _AudioSources[AudioType.EFFECT].audioType = AudioType.EFFECT;
        _AudioSources[AudioType.EFFECT].AudioSource.volume = 0.5f;

        go = new GameObject("Audio_Event");
        go.transform.SetParent(_AudioRoot);
        _AudioSources[AudioType.EVENT] = go.AddComponent<AudioSourceAgent>();
        _AudioSources[AudioType.EVENT].audioType = AudioType.EVENT;
        _AudioSources[AudioType.EVENT].AudioSource.volume = 0.5f;*/

        // 读取配置
        TableUtil config = new TableUtil();
        config.OpenFromData("audio.txt");

        for (int i = 0; i < config.GetRecordsNum(); i++)
        {
            int id = Convert.ToInt32(config.GetValue(i, 0));
            AudioConfig ac = new AudioConfig();
            ac.Type = (AudioType)Convert.ToInt32(config.GetValue(i, 1));
            ac.ResName = config.GetValue(i, 2);
            ac.IsOverlay = config.GetValue(i, 4) == "1" ? true : false;
            _Configs.Add(id, ac);
        }
    }

    /// <summary>
    /// 播放
    /// </summary>
    /// <param name="type"></param>
    /// <param name="path"></param>
    public string Play(int id)
    {
        if (_Configs.ContainsKey(id))
        {
            AudioConfig config = _Configs[id];
            AudioClip clip = GetAudioClip(id);
            if (clip == null)
                return "";

            return PlayAudio(config.Type, clip, config.IsOverlay);
        }
        return "";
    }

	/// <summary>
	/// 停止播放
	/// </summary>
	/// <param name="type"></param>
	/// <param name="path"></param>
	public string Stop(int id)
	{
		if (_Configs.ContainsKey(id))
		{
			AudioConfig config = _Configs[id];
			AudioClip clip = GetAudioClip(id);
			if (clip == null)
				return "";

			return StopAudio(config.Type, clip, config.IsOverlay);
		}
		return "";
	}

    /// <summary>
    /// 设置音乐开关
    /// </summary>
    /// <param name="swich"></param>
    public void SetAudioSwitch(int type, bool isEnable)
    {
        AudioType enumType = (AudioType)type;
        SetAudioSwitch(enumType, isEnable);
    }

    public void SetAudioSwitch(AudioType type, bool isEnable)
    {
        _AudioSwitches[type] = isEnable;
        if (OnSwitchChanged != null)
        {
            OnSwitchChanged(type, isEnable);
        }
    }

    /// <summary>
    /// 播放
    /// </summary>
    /// <param name="type"></param>
    /// <param name="audioClip"></param>
    public string PlayAudio(AudioType type, AudioClip audioClip)
    {
        return PlayAudio(type, audioClip, false);
    }

    /// <summary>
    /// 播放
    /// </summary>
    /// <param name="type"></param>
    /// <param name="audioClip"></param>
    public string PlayAudio(AudioType type, AudioClip audioClip, bool isOverlay)
    {
        if (_AudioSources[type] == null)
            return "";
        _AudioSources[type].clip = audioClip;
        if (_AudioSwitches[type])
        {
            if (isOverlay)
            {
                AudioSourceAgent asa = GameObject.Instantiate<AudioSourceAgent>(_AudioSources[type]);
                asa.name = "Audio" + DateTime.Now.Ticks;
                asa.transform.SetParent(this._AudioRoot);
                asa.clip = audioClip;
                asa.Play();
                // 非循环播放的播放结束后删除
                if (asa.AudioSource.loop == false)
                    GameObject.Destroy(asa.gameObject, audioClip.length);
                return asa.name;
            }
            else
            {
                _AudioSources[type].Play();
                return _AudioSources[type].name;
            }
        }
        else
            return "";
    }

	/// <summary>
	/// 停止播放
	/// </summary>
	/// <param name="type"></param>
	/// <param name="audioClip"></param>
	public string StopAudio(AudioType type, AudioClip audioClip, bool isOverlay)
	{
		if (_AudioSources[type] == null)
			return "";
		if ( !isOverlay ) 
		{
			_AudioSources [type].Stop ();
		}
		return "";
	}

    // 获取到音频内容
    public AudioClip GetAudioClip(int id)
    {
        if (_Caches.ContainsKey(id))
        {
            return _Caches[id];
        }
        if (_Configs.ContainsKey(id))
        {
            AudioConfig config = _Configs[id];
            AudioClip clip = ResourceManager.LoadObject(config.ResName) as AudioClip;
            _Caches.Add(id, clip);
            return clip;
        }
        return null;
    }
    // 获取播放音乐的对象(如果有的话)
    public GameObject GetAudioObject(string name)
    {
        if( name == null )
            return null;
        Transform trans = this._AudioRoot.Find(name);
        if (trans != null)
            return trans.gameObject;
        else
            return null;
    }
    // 设置频道音量
    public void SetChannelVolume(int type, float volume)
    {
        SetChannelVolume((AudioType)type, volume);
    }
    /// <summary>
    /// 设置音频音量
    /// </summary>
    /// <param name="type"></param>
    /// <param name="volume"></param>
    public void SetChannelVolume(AudioType type, float volume)
    {
        if (_AudioSources[type] == null)
            return;
        _AudioSources[type].volume = _MainVoice * volume;
    }
}
