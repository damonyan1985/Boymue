// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#include "BoymueApplication.h"

namespace boymue {
int BoymueApplication::s_applicationId = 0;

BoymueApplication::BoymueApplication()
    : m_appId(std::to_string(++s_applicationId)),
      m_uiThread("ui_thread_" + m_appId),
      m_ioThread("io_thread_" + m_appId),
      m_jsThread("js_thread_" + m_appId) {
  m_uiThread.start();
  m_ioThread.start();
  m_jsThread.start();

  getUITaskRunner().postTask(
      [self = this] { self->m_view = std::make_unique<BoymueView>(); });
}

TaskRunner& BoymueApplication::getUITaskRunner() const {
  return m_uiThread.getTaskRunner();
}

TaskRunner& BoymueApplication::getIOTaskRunner() const {
  return m_ioThread.getTaskRunner();
}

TaskRunner& BoymueApplication::getJSTaskRunner() const {
  return m_jsThread.getTaskRunner();
}
}  // namespace boymue