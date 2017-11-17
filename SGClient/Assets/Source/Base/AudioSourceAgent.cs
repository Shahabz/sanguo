using UnityEngine;
using System.Collections;

[RequireComponent(typeof(AudioSource))]
public class AudioSourceAgent : MonoBehaviour
{
    public bool playOnEnable = false;
    public AudioManager.AudioType audioType = AudioManager.AudioType.EFFECT;

    public enum PlayMode
    {
        Noraml,
        Fade
    }

    public PlayMode mode;

    public float fadeTime = 1.0f;

    public AudioClip clip;
    public float volume
    {
        set
        {
            _AudioSource.volume = value;
        }
        get
        {
            return _AudioSource.volume;
        }
    }

    public AudioSource AudioSource
    {
        get
        {
            return _AudioSource;
        }
    }
    AudioSource _AudioSource;


    void Start()
    {
        _AudioSource = GetComponent<AudioSource>();
        AudioManager.OnSwitchChanged += OnSwitchChanged;
        if (_AudioSource.playOnAwake)
        {
            _AudioSource.playOnAwake = false;
            Stop();
            Play();
        }
    }

    public void OnDestroy()
    {
        AudioManager.OnSwitchChanged -= OnSwitchChanged;
    }

    void OnEnable()
    {
        if (playOnEnable)
            Play();
    }

    void OnDisable()
    {
        Stop();
    }

    public void Play()
    {
        if (eye.audioManager == null)
            return;
		var switches = eye.audioManager.AudioSwitches;
        if (switches.ContainsKey(audioType) && switches[audioType])
        {
            //if (mode == PlayMode.Noraml)
            //{
            _AudioSource.clip = clip;
            _AudioSource.Play();
            //}
            //else
            //PlayFade(clip, fadeTime);
        }
    }

    public void Stop()
    {
        _AudioSource.Stop();
    }

    public void OnSwitchChanged(AudioManager.AudioType type, bool isEnable)
    {
        if (type == this.audioType)
        {
            if (isEnable == false)
            {
                Stop();
            }
            else
            {
                if (type == AudioManager.AudioType.BGM)
                {
                    Play();
                }
            }
        }
    }

    /// <summary>
    /// 淡隐模式播放
    /// </summary>
    /// <param name="clip"></param>
    //private void PlayFade(AudioClip clip, float fadeTime)
    //{
    //    StartCoroutine(PlayFadeAsync(clip, fadeTime));
    //}

    //private IEnumerator PlayFadeAsync(AudioClip clip, float fadeTime)
    //{
    //    float step = 0.01f;
    //    float curVolume = volume;
    //    while (true)
    //    {
    //        _AudioSource.volume = curVolume * (fadeTime - step) / fadeTime;
    //        step += 0.01f;
    //        yield return new WaitForEndOfFrame();
    //        if (step > fadeTime)
    //            break;
    //    }
    //    yield return null;
    //    _AudioSource.volume = volume;
    //    _AudioSource.clip = clip;
    //    _AudioSource.Play();
    //}
}
