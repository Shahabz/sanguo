using UnityEngine;  
using System.Collections;  
using UnityEditor;  

public class CreateBMFont : ScriptableWizard  
{
	public Font m_myFont;  
	public TextAsset m_data;  
	private BMFont mbFont = new BMFont();
	
	/// <summary>  
	/// 创建编辑窗口（注意，这个函数名可以随意取）  
	/// </summary>
	/// 
	[MenuItem("Tools/Create BMFont")]
	static void CreateWindow()  
	{
		ScriptableWizard.DisplayWizard<CreateBMFont>( "Create BMFont", "Create" ); 
	}
	
	/// <summary>  
	/// 窗口创建或窗口内容更改时调用  
	/// </summary>  
	void OnWizardUpdate()  
	{  
		helpString = "This Wizard help to create a BMFont!";  
		
		if( m_myFont == null )  
		{  
			errorString = "Choose A Custom Font!";  
			isValid = false;  
		}
		else if( m_data == null )
		{
			errorString = "Choose A .fnt file";  
			isValid = false; 
		}
		else  
		{  
			errorString = "";  
			isValid = true;  
		}  
	}  
	
	/// <summary>  
	/// 点击Add按钮（即Create按钮）调用  
	/// </summary>  
	void OnWizardCreate()  
	{  
		
		BMFontReader.Load(mbFont, m_data.name, m_data.bytes);  // 借用NGUI封装的读取类
		CharacterInfo[] characterInfo = new CharacterInfo[mbFont.glyphs.Count];  
		for (int i = 0; i < mbFont.glyphs.Count; i++)  
		{  
			BMGlyph bmInfo = mbFont.glyphs[i];  
			CharacterInfo info = new CharacterInfo();  
			info.index = bmInfo.index;  
			info.uv.x = (float)bmInfo.x / (float)mbFont.texWidth;  
			info.uv.y = 1-(float)bmInfo.y / (float)mbFont.texHeight;  
			info.uv.width = (float)bmInfo.width / (float)mbFont.texWidth;  
			info.uv.height = -1f * (float)bmInfo.height / (float)mbFont.texHeight;  
			info.vert.x = (float)bmInfo.offsetX;  
			//info.vert.y = (float)bmInfo.offsetY;  
			info.vert.y = 0f;//自定义字库UV从下往上，所以这里不需要偏移，填0即可。 
			info.vert.width = (float)bmInfo.width;  
			info.vert.height = (float)bmInfo.height;  
			info.width = (float)bmInfo.advance;  
			characterInfo[i] = info;  
		}  
		m_myFont.characterInfo = characterInfo; 
	}
}  