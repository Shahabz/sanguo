using UnityEngine;
using System.Collections;
using UnityEditor;  
[CustomPropertyDrawer( typeof( MyRangeAttribute ) )]  
public class MyRangeAttributeDrawer : PropertyDrawer
{
    // 重写OnGUI的方法（坐标，SerializedProperty 序列化属性，显示的文字）  
    public override void OnGUI( Rect position, SerializedProperty property, GUIContent lable )
    {
        // attribute 是PropertyAttribute类中的一个属性  
        // 调用MyRangeAttribute中的最大和最小值还有文字信息，用于绘制时候的显示  
        MyRangeAttribute range = attribute as MyRangeAttribute;
        // 判断传进来的值类型  
        if ( property.propertyType == SerializedPropertyType.Float )
        {
            EditorGUI.Slider( position, property, range.min, range.max, range.label );
        }
        else if ( property.propertyType == SerializedPropertyType.Integer )
        {
            EditorGUI.IntSlider( position, property, (int)range.min, (int)range.max, range.label );
        }
    }

}