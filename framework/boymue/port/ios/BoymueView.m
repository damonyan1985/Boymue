//
//  BoymueView.m
//  core
//
//  Created by yanbo on 2022/8/17.
//

#import <Foundation/Foundation.h>
#include "BoymueView.h"

@interface BoymueView()

@end


@implementation BoymueView

// 利用CAEAGLLayer类来创建CAEAGLLayer对象
+(Class)layerClass {
    return [CAEAGLLayer class];
}

// 返回CAMetalLayer对象
-(CAEAGLLayer*)eaLayer {
    return (CAEAGLLayer*)self.layer;
}

@end
