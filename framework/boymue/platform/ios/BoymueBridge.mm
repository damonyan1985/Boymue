//
//  BoymueBridge.m
//  core
//
//  Created by yanbo on 2022/9/1.
//

#include "BoymueBridge.h"
#import <Foundation/Foundation.h>

#define STR_TO_OCSTR(str) ([[NSString alloc] initWithBytes:str.data() length:str.size() encoding:NSUTF8StringEncoding])
#define OCSTR_TO_STR(str) (std::string([str UTF8String]));

namespace boymue {
String BoymueBridge::getAppRoot() {
    NSBundle* coreBundle = [NSBundle bundleWithIdentifier:@"com.boymue.core"];
    NSString* appDir = [coreBundle pathForResource:@"assets" ofType:@"bundle"];
    NSBundle* appBundle = [NSBundle bundleWithPath:appDir];
    return OCSTR_TO_STR(appBundle.bundlePath);
}

String BoymueBridge::getSourcePath(const String& path) {
    NSBundle* coreBundle = [NSBundle bundleWithIdentifier:@"com.boymue.core"];
    NSString* appDir = [coreBundle pathForResource:@"assets" ofType:@"bundle"];
    NSBundle* appBundle = [NSBundle bundleWithPath:appDir];
    NSString* sourcePath = [appBundle.bundlePath stringByAppendingString:STR_TO_OCSTR(path)];
    return OCSTR_TO_STR(sourcePath);
}
}
