using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UITextTimeCountdownSec : UITextTimeCountdown {

	protected override void ShowText(int leftTime)
	{

		if (!string.IsNullOrEmpty (formatText)) {
			text = string.Format (formatText, leftTime);
		} else {
			text = leftTime.ToString();
		}
	}
}
