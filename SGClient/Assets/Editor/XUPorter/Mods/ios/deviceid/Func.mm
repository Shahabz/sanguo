//
//  SvUDIDTools.m
//  SvUDID
//
//  Created by  maple on 8/18/13.
//  Copyright (c) 2013 maple. All rights reserved.
//

#import "DeviceTools.h"

extern "C" {
    char *s_szDeviceDescMem = NULL;
    char *getDeviceDesc()
    {
        if( s_szDeviceDescMem == NULL )
            s_szDeviceDescMem = (char*)malloc(256);
        const char *tmpstr = [[DeviceTools deviceDesc] UTF8String];
        if( tmpstr )
        {
            strncpy(s_szDeviceDescMem, tmpstr,256);
            s_szDeviceDescMem[255] = 0;
        }
        else
        {
            s_szDeviceDescMem[0] = 0;
        }
        return s_szDeviceDescMem;
    }
}
