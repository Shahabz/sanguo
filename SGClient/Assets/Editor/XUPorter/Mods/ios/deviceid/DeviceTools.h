//
//  SvUDIDTools.h
//  SvUDID
//
//  Created by  maple on 8/18/13.
//  Copyright (c) 2013 maple. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

@interface DeviceTools : NSObject


/*
 * @brief obtain Unique Device Identity
 */
+ (NSString*)deviceDesc;
@end
