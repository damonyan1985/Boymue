// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#ifndef BoymueApplication_h
#define BoymueApplication_h

#include <memory>
#include <string>

#include "BoymueView.h"
#include "TaskThread.h"
#include "JsEngine.h"
#include "BoymueAppInfo.h"
#include "Loader.h"
#include "ThreadExecutor.h"

namespace boymue {
class BoymueApplication {
 public:
  BoymueApplication(BoymueAppInfo* info);
  ~BoymueApplication();
  TaskRunner& getUITaskRunner() const;
  TaskRunner& getIOTaskRunner() const;
  TaskRunner& getJSTaskRunner() const;

  void evaluateJs(const String& jsSource, const String& scriptId);
  void doRuntimeAction(RuntimeClosure& action);
    
  JsRuntime* runtime() const;
  const Loader& loader() const;

  ThreadExecutor* ioExecutor() const;

 private:
  OwnerPtr<JsEngine> m_jsEngine;
  // BoymueView can only used in ui thread
  OwnerPtr<BoymueView> m_mainView;
  OwnerPtr<JsRuntime> m_mainRuntime;

  // 应用信息
  BoymueAppInfo* m_appInfo;
  // ui thread处理UI事件，JS运行等
  TaskThread m_uiThread;
  // io thread处理资源文件加载，外部事件分发
  TaskThread m_ioThread;
  // js thread用来执行js的线程
  TaskThread m_jsThread;

  OwnerPtr<ThreadExecutor> m_ioExecutor;
    
  Loader m_loader;
};
}  // namespace boymue
#endif  // !BoymueApplication_h
