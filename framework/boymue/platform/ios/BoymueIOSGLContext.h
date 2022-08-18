//
//  BoymueIOSGLContext.h
//  core
//
//  Created by yanbo on 2022/8/17.
//

#ifndef BoymueIOSGLContext_h
#define BoymueIOSGLContext_h

#import <UIKit/UIKit.h>

@interface BoymueIOSGLContext : NSObject

-(void)makeCurrent;
-(void)swapBuffers;
-(void)attachLayer:(CAEAGLLayer*)layer;

@end

#endif /* BoymueIOSGLContext_h */
