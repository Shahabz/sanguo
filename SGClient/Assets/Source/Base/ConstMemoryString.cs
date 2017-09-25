using System.Text;
using System.Reflection;

public class ConstMemoryString
{
    private string _baseStr;
    private StringBuilder _builder;
    private char[] _intAry = new char[11];

    public ConstMemoryString(int capacity)
    {
        _builder = new StringBuilder(capacity, capacity);

        FieldInfo fi = _builder.GetType().GetField(
            "_str",
            BindingFlags.NonPublic |
            BindingFlags.Instance);

        _baseStr = (string)fi.GetValue(_builder);
    }

    public string content
    {
        get
        {
            if (_builder.Length == 0)
                return string.Empty;

            int delta = _builder.Capacity - _builder.Length;
            if (delta > 0)
            {
                for (int i = 0; i < delta; ++i)
                    _builder.Append(" ");
            }

            return _baseStr;
        }
    }

    public void Clear()
    {
        _builder.Length = 0;
    }

    public void Append(string value)
    {
        _builder.Append(value);
    }

    public void Append(int value)
    {
        int count = 0;

        if (value >= 0)
        {
            count = ToCharArray((uint)value, _intAry, 0);
        }
        else
        {
            _intAry[0] = '-';
            count = ToCharArray((uint)-value, _intAry, 1) + 1;
        }
        for (int i = 0; i < count; i++)
        {
            _builder.Append(_intAry[i]);
        }
    }

    public void Set(string v)
    {
        _builder.Length = 0;
        Append(v);
    }

    public void Set(int v)
    {
        _builder.Length = 0;
        Append(v);
    }

    private static int ToCharArray(uint value, char[] buffer, int bufferIndex)
    {
        if (value == 0)
        {
            buffer[bufferIndex] = '0';
            return 1;
        }
        int len = 1;
        for (uint rem = value / 10; rem > 0; rem /= 10)
        {
            len++;
        }
        for (int i = len - 1; i >= 0; i--)
        {
            buffer[bufferIndex + i] = (char)('0' + (value % 10));
            value /= 10;
        }
        return len;
    }
}