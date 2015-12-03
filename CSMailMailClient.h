/*=========================================================================
 This file is part of the Horos Project (www.horosproject.org)
 
 Horos is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation,  version 3 of the License.
 
 Portions of the Horos Project were originally licensed under the GNU GPL license.
 However, all authors of that software have agreed to modify the license to the
 GNU LGPL.
 
 Horos is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY EXPRESS OR IMPLIED, INCLUDING ANY WARRANTY OF
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE OR USE.  See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with Horos.  If not, see http://www.gnu.org/licenses/lgpl.html
 
 Prior versions of this file were published by the OsiriX team pursuant to
 the below notice and licensing protocol.
 ============================================================================
 Program:   OsiriX
  Copyright (c) OsiriX Team
  All rights reserved.
  Distributed under GNU - LGPL
  
  See http://www.osirix-viewer.com/copyright.html for details.
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.
 ============================================================================*/

//
//  CSMailMailClient.h
//  CSMail
//
//  Created by Alastair Houghton on 27/01/2006.
//  Copyright 2006 Coriolis Systems Limited. All rights reserved.
//

#import <Cocoa/Cocoa.h>

enum {
  kCSMCMessageDispatchFeature = 0x0001,
  kCSMCMessageConstructionFeature = 0x0002,
  kCSMCConfigureFeature = 0x0004,
};


@interface CSMailMailClient : NSObject
{
    NSAppleScript *script;
    NSDictionary *defaultSMTPAccount;
    NSString *fromAddress;
}

+ (id) mailClient;
- (NSString *)name;
- (NSString *)version;
- (NSDictionary *) defaultSMTPAccountFromMail;
- (NSString *)applicationName;

- (BOOL)applicationIsInstalled;
- (NSImage *)applicationIcon;

- (int)features;

- (BOOL)deliverMessage:(NSString *)messageBody
	       headers:(NSDictionary *)messageHeaders;
- (BOOL)deliverMessage:(NSString *)messageBody
               headers:(NSDictionary *)messageHeaders
           withMailApp:(BOOL) mailApp;

@end
