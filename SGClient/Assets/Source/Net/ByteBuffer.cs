using UnityEngine;
using System.Collections;
using System.IO;
using System.Text;
using System;

// 修改过的字节流
public class ByteBuffer {
    MemoryStream stream = null;
    BinaryWriter writer = null;
    BinaryReader reader = null;

    public ByteBuffer() {
        stream = new MemoryStream();
        writer = new BinaryWriter(stream);
    }

    public ByteBuffer(byte[] data) {
        if (data != null) {
            stream = new MemoryStream(data);
            reader = new BinaryReader(stream);
        } else {
            stream = new MemoryStream();
            writer = new BinaryWriter(stream);
        }
    }

    public void Close() {
        if (writer != null) 
            writer.Close();
        if (reader != null) 
            reader.Close();
        if (stream != null)
            stream.Close();
        writer = null;
        reader = null;
        stream = null;
    }

    public void WriteSByte( int v )
    {
        if ( writer == null )
            return;
        writer.Write((sbyte)v);
    }

    public void WriteByte (int v) 
    {
        if ( writer == null )
            return;
        writer.Write((byte)v);
    }

    public void WriteInt(int v) 
    {
        if ( writer == null )
            return;
        writer.Write((int)v);
    }

    public void WriteUInt( uint v )
    {
        if ( writer == null )
            return;
        writer.Write((uint)v);
    }

    public void WriteShort(short v) 
    {
        if ( writer == null )
            return;
        writer.Write((short)v);
    }

    public void WriteUShort( ushort v )
    {
        if ( writer == null )
            return;
        writer.Write((ushort)v);
    }

    public void WriteLong(long v) 
    {
        if ( writer == null )
            return;
        writer.Write((long)v);
    }

    public void WriteFloat(float v) 
    {
        if ( writer == null )
            return;
        byte[] temp = BitConverter.GetBytes(v);
        Array.Reverse(temp);
        writer.Write(BitConverter.ToSingle(temp, 0));
    }

    public void WriteDouble(double v) 
    {
        if ( writer == null )
            return;
        byte[] temp = BitConverter.GetBytes(v);
        Array.Reverse(temp);
        writer.Write(BitConverter.ToDouble(temp, 0));
    }

    public void WriteString(string v) 
    {
        if ( writer == null )
            return;
        byte[] bytes = Encoding.UTF8.GetBytes(v);
        writer.Write((ushort)bytes.Length);
        writer.Write(bytes);
    }
    
    public void WriteStringWithLength(string v) 
    {
        if ( writer == null )
            return;
        byte[] bytes = Encoding.UTF8.GetBytes(v);
        writer.Write(bytes);
    }

    public void WriteBytes(byte[] v)
    {
        if ( writer == null )
            return;
        writer.Write((ushort)v.Length);
        writer.Write(v);
    }
    
    public void WriteBytesWithLength(byte[] v) 
    {
        if ( writer == null )
            return;
        writer.Write(v);
    }

    public int ReadByte () 
    {
        int value = 0;
        try {
            value = reader.ReadByte(); 
        }
        catch( EndOfStreamException e ){
            Debug.LogError( e.Message );
            value = 0;
        }
        return value; 
    }

    public int ReadSByte()
    {
        int value = 0;
        try{
            value = (int)reader.ReadSByte();
        }
        catch ( EndOfStreamException e ){
            Debug.LogError( e.Message );
            value = 0;
        }
        return value;
    }

    public int ReadInt() 
    {
        int value = 0;
        try{
            value = (int)reader.ReadInt32();
        }
        catch ( EndOfStreamException e ){
            Debug.LogError( e.Message );
            value = 0;
        }
        return value;
    }
    public uint ReadUInt()
    {
        uint value = 0;
        try{
            value = (uint)reader.ReadUInt32();
        }
        catch ( EndOfStreamException e ){
            Debug.LogError( e.Message );
            value = 0;
        }
        return value;
    }

    public short ReadShort() 
    {
        short value = 0;
        try{
            value = (short)reader.ReadInt16();
        }
        catch ( EndOfStreamException e ){
            Debug.LogError( e.Message );
            value = 0;
        }
        return value;
    }

    public ushort ReadUShort()
    {
        ushort value = 0;
        try{
            value = (ushort)reader.ReadUInt16();
        }
        catch ( EndOfStreamException e ){
            Debug.LogError( e.Message );
            value = 0;
        }
        return value;
    }

    public long ReadLong() 
    {
        long value = 0;
        try{
            value = (long)reader.ReadInt64();
        }
        catch ( EndOfStreamException e ){
            Debug.LogError( e.Message );
            value = 0;
        }
        return value;
    }

    public float ReadFloat() {
        byte[] temp = BitConverter.GetBytes(reader.ReadSingle());
        Array.Reverse(temp);
        return BitConverter.ToSingle(temp, 0);
    }

    public double ReadDouble()
    {
        byte[] temp = BitConverter.GetBytes(reader.ReadDouble());
        Array.Reverse(temp);
        return BitConverter.ToDouble(temp, 0);
    }

    public string ReadString() 
    {
        try
        {
            ushort len = (ushort)ReadShort();
            byte[] buffer = new byte[len];
            buffer = reader.ReadBytes( len );
            return Encoding.UTF8.GetString( buffer );
        }
        catch ( EndOfStreamException e )
        {
            Debug.LogError( e.Message );
            return "";
        }
    }
    
    public string ReadStringWithLen( ushort len ) 
    {
        try
        {
            byte[] buffer = new byte[len];
            buffer = reader.ReadBytes( len );
            return Encoding.UTF8.GetString( buffer );
        }
        catch ( EndOfStreamException e )
        {
            Debug.LogError( e.Message );
            return "";
        }
    }

    public byte[] ReadBytes() 
    {
        try
        {
            ushort len = (ushort)ReadShort();
            return reader.ReadBytes( len );
        }
        catch ( EndOfStreamException e )
        {
            Debug.LogError( e.Message );
            return new byte[1];
        }
    }
    
    public byte[] ReadBytesWithLen( ushort len ) 
    {
        try
        {
            return reader.ReadBytes(len);
        }
        catch ( EndOfStreamException e )
        {
            Debug.LogError( e.Message );
            return new byte[1];
        }
    }

    public byte[] ToBytes() {
        if ( writer != null )
            writer.Flush();
        return stream.ToArray();
    }

    public void Flush() {
        if ( writer == null )
            return;
        writer.Flush();
    }

	public UInt32 Len() {
		return (UInt32)stream.Length;
	}
}
