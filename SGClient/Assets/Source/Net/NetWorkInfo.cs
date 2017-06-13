using UnityEngine;
using System.Collections;
using System.Net.NetworkInformation;
using System;

public  class NetWorkInfo 
{
    public static string GetMacAddress()
    {
        string physicalAddress = "";

        NetworkInterface[] nice = NetworkInterface.GetAllNetworkInterfaces();

        foreach(NetworkInterface adaper in nice)
        {
			//Debug.Log( "adaper.Description:"+adaper.Description );

            if (adaper.Description == "en0")
            {
                physicalAddress = adaper.GetPhysicalAddress().ToString();
                break;
            }
            else
            {
                physicalAddress = adaper.GetPhysicalAddress().ToString();

                if (physicalAddress != "") 
                {
                    break;
                };
            }
        }

        //Debug.Log("physicalAddress : " + physicalAddress);

        return physicalAddress;
    }
}
