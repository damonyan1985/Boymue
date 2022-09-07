//
//  BoymueViewController.m
//  core
//
//  Created by yanbo on 2022/8/17.
//

#include "PaintContextIOS.h"

#import "BoymueViewController.h"
#import "BoymueAppView.h"
#include "SkTypeface.h"
#include "BoymueBridge.h"
#include "BoymueApplication.h"
#include "FileUtil.h"
#include "Document.h"

#import <GLKit/GLKit.h>
#import <OpenGLES/ES2/gl.h>
#include <fstream>

static boymue::BoymueApplication* s_app;

@interface BoymueViewController()

@property (nonatomic, strong) BoymueAppView* boymueView;

@property (nonatomic, strong) BoymueIOSGLContext* glContext;
@property (nonatomic, assign) GLuint framebuffer;
@property (nonatomic, assign) GLuint colorRenderbuffer;
@property (nonatomic, assign) GLint framebufferWidth;
@property (nonatomic, assign) GLint framebufferHeight;

@end

@implementation BoymueViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.boymueView = [[BoymueAppView alloc] initWithFrame:self.view.bounds];
    self.boymueView.eaLayer.frame = self.view.bounds;
    self.boymueView.eaLayer.drawableProperties = @{kEAGLDrawablePropertyRetainedBacking:@NO, kEAGLDrawablePropertyColorFormat: kEAGLColorFormatRGBA8};
    self.boymueView.eaLayer.opaque = YES;
    self.view = self.boymueView;
    //self.view.backgroundColor = [UIColor whiteColor];
    NSLog(@"hello boymue");
    [self drawTest];
    
    
    boymue::String path = std::move(boymue::BoymueBridge::getSourcePath("/example/test.js"));
    boymue::String source = std::move(boymue::FileUtil::readFile(path));
    
    s_app = new boymue::BoymueApplication();
    s_app->evaluateJs(source.c_str());
    
    
    boymue::String uiPath = std::move(boymue::BoymueBridge::getSourcePath("/example/test.xml"));
    boymue::Document dom;
    dom.initDocument(boymue::FileUtil::readFile(uiPath));
    
}

-(void)drawTest {
    boymue::PaintContextIOS* painter = new boymue::PaintContextIOS();
    painter->initContext(self.boymueView.layer, self.view.bounds.size.width, self.view.bounds.size.height);

    SkCanvas* canvas = painter->canvas();
    canvas->drawColor(SK_ColorBLUE);
    
    SkTypeface* typeface = SkTypeface::CreateFromName("Heiti SC", SkTypeface::kNormal);
    SkPaint paint;
    if (typeface) {
        paint.setTypeface(typeface);
    }
    paint.setColor(SK_ColorRED);  // This is a solid black color for our text
    paint.setTextSize(SkIntToScalar(30));  // Sets the text size to 30 pixels
    paint.setAntiAlias(true);  // We turn on anti-aliasing so that the text to looks good.

    // Draw some text
    SkString text("Skia Test你好");
    SkScalar fontHeight = paint.getFontSpacing();
    canvas->drawText(text.c_str(), text.size(),  // text's data and length
                   10, 100,  // X and Y coordinates to place the text
                   paint);
    painter->submit();
    
#if 0
    self.glContext = [BoymueIOSGLContext new];
    [self.glContext makeCurrent];
    
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
    //glViewport(0, 0, _framebufferWidth, _framebufferHeight);
    //[self.effect prepareToDraw];
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

        //set up vertices
//        GLfloat vertices[] = {
//            -0.5f, -0.5f, -1.0f, 0.0f, 0.5f, -1.0f, 0.5f, -0.5f, -1.0f,
//        };
//
//        //set up colors
//        GLfloat colors[] = {
//            0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//        };
//
//        //draw triangle
//        glEnableVertexAttribArray(GLKVertexAttribPosition);
//        glEnableVertexAttribArray(GLKVertexAttribColor);
//        glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 0, vertices);
//        glVertexAttribPointer(GLKVertexAttribColor,4, GL_FLOAT, GL_FALSE, 0, colors);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);
    [self.glContext swapBuffers];
    
    GLenum error = glGetError();
#endif
}

@end


