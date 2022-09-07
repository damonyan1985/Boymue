// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.09

#ifndef BoymueView_h
#define BoymueView_h

#include "Layout.h"
#include "Document.h"
#include "StringUtil.h"
#include "BoymuePage.h"
#include <memory>

// 应用UI
namespace boymue {
class BoymueApplication;
class BoymueView {
 public:
  BoymueView(BoymueApplication* context);

 private:
  // 应用上下文
  BoymueApplication* m_context;
  // 栈式管理页面
  Stack<BoymuePage*> m_stack;
};
}  // namespace boymue
#endif
