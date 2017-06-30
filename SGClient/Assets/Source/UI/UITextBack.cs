using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UITextBack : MonoBehaviour {

	public InlieText m_uiText;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		Refresh ();
	}

	public void Refresh()
	{
		if (m_uiText.preferredWidth + 20.0f < 105.0f)
		{
			gameObject.GetComponent<RectTransform>().sizeDelta = new Vector2(105.0f, m_uiText.preferredHeight + 50.0f);
		}
		else if (m_uiText.preferredWidth + 20.0f > m_uiText.rectTransform.sizeDelta.x)
		{
			gameObject.GetComponent<RectTransform>().sizeDelta = new Vector2(m_uiText.rectTransform.sizeDelta.x + 20.0f, m_uiText.preferredHeight + 50.0f);
		}
		else
		{
			gameObject.GetComponent<RectTransform>().sizeDelta = new Vector2(m_uiText.preferredWidth + 20.0f, m_uiText.preferredHeight + 50.0f);
		}

		//gameObject.SetActive(true);
		//m_uiText.SetVerticesDirty();

		//gameObject.GetComponent<RectTransform>().sizeDelta = new Vector2(m_uiText.preferredWidth, m_uiText.preferredHeight + 50.0f);
	}
}
