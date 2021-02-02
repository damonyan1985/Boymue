// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#include "BoymueApplication.h"

#include "JSTestApi.h"
#include "JsLogApi.h"

namespace boymue {
int BoymueApplication::s_applicationId = 0;

BoymueApplication::BoymueApplication()
    : m_appId(std::to_string(++s_applicationId)),
      m_uiThread("ui_thread_" + m_appId),
      m_ioThread("io_thread_" + m_appId),
      m_jsThread("js_thread_" + m_appId) {
  m_jsEngine = std::make_unique<JsEngine>();

  m_uiThread.start();
  m_ioThread.start();
  m_jsThread.start();

  getUITaskRunner().postTask(
      [self = this] { self->m_mainView = std::make_unique<BoymueView>(self); });

  getJSTaskRunner().postTask([self = this] {
    self->m_mainRuntime =
        std::unique_ptr<JsRuntime>(self->m_jsEngine->createRuntime());
    self->m_mainRuntime->registerApi(new boymue::JsLogApi(self));
    self->m_mainRuntime->registerApi(new boymue::JsTestApi(self));
  });
}

void BoymueApplication::evaluateJs(const std::string& jsSource) {
  getJSTaskRunner().postTask(
      [=] { this->m_mainRuntime->evaluateJs(jsSource); });
}

void BoymueApplication::doRuntimeAction(RuntimeClosure& action) {
    getJSTaskRunner().postTask(
        [=] { this->m_mainRuntime->doAction(action); });
}

// 结束当前应用的线程
BoymueApplication::~BoymueApplication() {
  m_uiThread.terminate();
  m_ioThread.terminate();
  m_jsThread.terminate();
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