//
//  PaintContextIOS.h
//  core
//
//  Created by yanbo on 2022/8/17.
//

#ifndef PaintContextIOS_h
#define PaintContextIOS_h

#include "PaintContext.h"
#include "GrContext.h"
#include "GrContextFactory.h"
#include "SkSurface.h"

#import "BoymueIOSGLContext.h"
#import <UIKit/UIKit.h>

namespace boymue {
class PaintContextIOS : public PaintContext {
public:
  PaintContextIOS();
  void initContext(CAEAGLLayer* layer, int width, int height);
  virtual void reset();
  virtual SkCanvas* canvas();
  virtual void submit();
    
private:
  BoymueIOSGLContext* m_glContext;
  GrContext* m_context;
  SkSurface* m_surface;
};
}


#endif /* PaintContextIOS_h */
