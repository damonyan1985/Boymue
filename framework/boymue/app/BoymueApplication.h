// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#ifndef BoymueApplication_h
#define BoymueApplication_h

#include <memory>
#include <string>

#include "BoymueView.h"
#include "TaskThread.h"

namespace boymue {
class BoymueApplication {
 public:
  BoymueApplication();
  TaskRunner& getUITaskRunner() const;
  TaskRunner& getIOTaskRunner() const;
  TaskRunner& getJSTaskRunner() const;

 private:
  static int s_applicationId;
  // BoymueView can only used in ui thread
  std::unique_ptr<BoymueView> m_view;

  // 应用id
  std::string m_appId;
  // ui thread处理UI事件，JS运行等
  TaskThread m_uiThread;
  // io thread处理资源文件加载，外部事件分发
  TaskThread m_ioThread;
  // js thread用来执行js的线程
  TaskThread m_jsThread;
};
}  // namespace boymue
#endif  // !BoymueApplication_h
