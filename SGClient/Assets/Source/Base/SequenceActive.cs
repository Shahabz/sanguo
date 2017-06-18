using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SequenceActive : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	protected void OnEnable()
	{
		for (int i = 0; i < transform.childCount; i++) {
			transform.GetChild (i).gameObject.SetActive(false);
		}
		StartCoroutine( Execute() );
	}

	protected void OnDisable()
	{
		StopCoroutine( Execute() );
	}

	IEnumerator Execute()
	{
		for ( int i = 0; i < transform.childCount; i++ )
		{
			transform.GetChild (i).gameObject.SetActive(true);
			yield return 0;
		}
	}
}
