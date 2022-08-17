//
//  BoymueViewController.m
//  core
//
//  Created by yanbo on 2022/8/17.
//

#include "PaintContextIOS.h"

#import "BoymueViewController.h"
#import "BoymueView.h"
#import <OpenGLES/ES2/gl.h>

@interface BoymueViewController()

@property (nonatomic, strong) BoymueView* boymueView;


@property (nonatomic, strong) BoymueIOSGLContext* glContext;
@property (nonatomic, assign) GLuint framebuffer;
@property (nonatomic, assign) GLuint colorRenderbuffer;
@property (nonatomic, assign) GLint framebufferWidth;
@property (nonatomic, assign) GLint framebufferHeight;

@end

@implementation BoymueViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.boymueView = [[BoymueView alloc] initWithFrame:self.view.bounds];
    self.boymueView.eaLayer.frame = self.view.bounds;
    self.boymueView.eaLayer.drawableProperties = @{kEAGLDrawablePropertyRetainedBacking:@NO, kEAGLDrawablePropertyColorFormat: kEAGLColorFormatRGBA8};
    self.boymueView.eaLayer.opaque = YES;
    self.view = self.boymueView;
    //self.view.backgroundColor = [UIColor whiteColor];
    NSLog(@"hello boymue");
    [self drawTest];
}

-(void)drawTest {
//    boymue::PaintContextIOS* painter = new boymue::PaintContextIOS();
//    painter->initContext(self.boymueView.layer, self.view.bounds.size.width, self.view.bounds.size.height);
//
//    SkCanvas* canvas = painter->canvas();
//    canvas->drawColor(SK_ColorWHITE);
//
//    painter->submit();
    self.glContext = [BoymueIOSGLContext new];
    [self.glContext makeCurrent:self.boymueView.layer];
    
    //set up color render buffer
    glGenRenderbuffers(1, &_colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);
    [self.glContext attachLayer:self.boymueView.layer];
    
    //set up frame buffer
        glGenFramebuffers(1, &_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderbuffer);
        //[self.glContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:self.glLayer];
    
        
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_framebufferWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_framebufferHeight);

        //check success
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            NSLog(@"Failed to make complete framebuffer object: %i", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        }
    
    
    
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glViewport(0, 0, _framebufferWidth, _framebufferHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);
    [self.glContext swapBuffers];
    
    GLenum error = glGetError();
    
}

@end


