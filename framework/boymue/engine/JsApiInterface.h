// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.21

#ifndef JsApiInterface_h
#define JsApiInterface_h

#include "StringUtil.h"
#include "ThreadExecutor.h"

namespace boymue {
class BoymueApplication;
// Api回调
class JsApiCallback {
 public:
  virtual ~JsApiCallback() {}
  // JS回调
  virtual void callback(const String& result) = 0;
};
// Native端Api接口
class JsApiInterface {
 public:
  JsApiInterface(BoymueApplication* app)
    : m_app(app) {}
    
  virtual ~JsApiInterface() {};
  virtual const char* name() const = 0;
  // 获取JS应用上下文
  virtual BoymueApplication* context() const { return m_app; }
  // 线程池接口
  virtual ThreadExecutor* executor() { return nullptr; }
  // JS接口执行方法
  virtual void execute(const String& params, JsApiCallback* callback) = 0;
    
private:
  BoymueApplication* m_app;
};
}  // namespace boymue

#endif  // !JsApiInterface_h
