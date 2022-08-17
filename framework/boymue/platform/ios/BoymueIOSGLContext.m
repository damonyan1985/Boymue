//
//  BoymueIOSGLContext.m
//  core
//
//  Created by yanbo on 2022/8/17.
//

#import <Foundation/Foundation.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import "BoymueIOSGLContext.h"

@interface BoymueIOSGLContext()

@property (nonatomic, strong) EAGLContext* glContext;

@end

@implementation BoymueIOSGLContext

-(instancetype)init {
    self = [super init];
    if (self) {
        self.glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    }
    
    return self;
}

-(void)makeCurrent:(CAEAGLLayer*)layer {
    [EAGLContext setCurrentContext:self.glContext];
    //[self.glContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
}

-(void)attachLayer:(CAEAGLLayer*)layer {
    [self.glContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
}

-(void)swapBuffers {
    [self.glContext presentRenderbuffer:GL_RENDERBUFFER];
}

@end
