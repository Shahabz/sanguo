using UnityEngine;
using System.Collections;

public class BirdEmitter : MonoBehaviour {
	private ParticleSystem birdparticleSys;
	public int minsec = 80;
	public int maxsec = 160;
	// Use this for initialization
	void Start () {
		birdparticleSys = GetComponent<ParticleSystem>();
		Invoke("EmitterBirds", Random.Range(minsec, maxsec));
	}
	// Update is called once per frame
	void EmitterBirds () {
		if(birdparticleSys.gameObject.active)
		{
			birdparticleSys.Play();
		}
		Invoke("EmitterBirds", Random.Range(minsec, maxsec));
	}
}
