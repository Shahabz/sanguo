using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
///  帧动画组件
/// </summary>
public class SpriteAnimation : MonoBehaviour
{
	private SpriteRenderer _spriteRenderer;

	private int mCurFrame = 0;

	private float mDelta = 0;

	public float FPS = 0.1f;
	public float MinFPS = 0.0f;
	public float MaxFPS = 0.0f;

	public List<Sprite> SpriteFrames;

	public bool IsPlaying = false;

	public bool Foward = true;

	public bool AutoPlay = false;

	public bool Loop = false;

	public int FrameCount {
		get {
			return SpriteFrames.Count;
		}
	}

	void Awake ()
	{
		if ( MinFPS > 0.0 && MaxFPS > 0 )
		{
			FPS = Random.Range(MinFPS, MaxFPS);
		}
		_spriteRenderer = GetComponent<SpriteRenderer> ();
	}

	void Start ()
	{
		if (AutoPlay) {
			Play ();
		} else {
			IsPlaying = false;
		}
	}

	private void SetSprite (int idx)
	{
		_spriteRenderer.sprite = SpriteFrames [idx];
	}

	public void Play ()
	{
		IsPlaying = true;
		Foward = true;

	}

	public void PlayReverse ()
	{
		IsPlaying = true;
		Foward = false;
	}

	void Update ()
	{
		if (!IsPlaying || 0 == FrameCount) {
			return;
		}

		mDelta += Time.deltaTime;

		if (mDelta >  FPS) {
			mDelta = 0;
			if (Foward) {
				mCurFrame++;
			} else {
				mCurFrame--;
			}

			if (mCurFrame >= FrameCount) {
				if (Loop) {
					mCurFrame = 0;
				} else {
					IsPlaying = false;
					return;
				}
			} else if (mCurFrame < 0) {

				if (Loop) {

					mCurFrame = FrameCount - 1;

				} else {

					IsPlaying = false;

					return;

				}       

			}

			SetSprite (mCurFrame);

		}

	}

	public void Pause ()
	{

		IsPlaying = false;

	}

	public void Resume ()
	{

		if (!IsPlaying) {

			IsPlaying = true;

		}

	}

	public void Stop ()
	{

		mCurFrame = 0;

		SetSprite (mCurFrame);

		IsPlaying = false;

	}

	public void Rewind ()
	{

		mCurFrame = 0;

		SetSprite (mCurFrame);

		Play ();

	}

}
