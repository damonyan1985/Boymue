// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#include "BoymueApplication.h"

#include "JSTestApi.h"
#include "JsLogApi.h"
#include "JsUIOperationApi.h"
#include "JsXmlToJsonApi.h"
#include "JsRequireModuleApi.h"
#include "JsRequestApi.h"

namespace boymue {

BoymueApplication::BoymueApplication(BoymueAppInfo* info)
    : m_appInfo(info),
      m_uiThread("ui_thread_" + info->appName),
      m_ioThread("io_thread_" + info->appName),
      m_jsThread("js_thread_" + info->appName) {
  m_jsEngine = std::make_unique<JsEngine>();
  
  m_uiThread.start();
  m_ioThread.start();
  m_jsThread.start();

  m_ioExecutor = ThreadExecutor::createSingleTaskExecutor(&m_ioThread);
  getUITaskRunner().postTask(
      [self = this] { self->m_mainView = std::make_unique<BoymueView>(self); });

  getJSTaskRunner().postTask([self = this] {
    self->m_mainRuntime =
        std::unique_ptr<JsRuntime>(self->m_jsEngine->createRuntime());
    self->m_mainRuntime->registerApi(new JsLogApi(self));
    self->m_mainRuntime->registerApi(new JsTestApi(self));
    self->m_mainRuntime->registerApi(new JsUIOperationApi(self));
    self->m_mainRuntime->registerApi(new JsXmlToJsonApi(self));
    self->m_mainRuntime->registerApi(new JsRequireModuleApi(self));
    self->m_mainRuntime->registerApi(new JsRequestApi(self));
    self->m_mainRuntime->setContext(self);
  });
}

ThreadExecutor* BoymueApplication::ioExecutor() const {
    return m_ioExecutor;
}

void BoymueApplication::evaluateJs(const String& jsSource, const String& scriptId) {
  getJSTaskRunner().postTask(
      [=] { this->m_mainRuntime->evaluateJs(jsSource, scriptId); });
}

void BoymueApplication::doRuntimeAction(RuntimeClosure& action) {
    getJSTaskRunner().postTask(
        [=] { this->m_mainRuntime->doAction(action); });
}

JsRuntime* BoymueApplication::runtime() const {
    return m_mainRuntime.get();
}

// 结束当前应用的线程
BoymueApplication::~BoymueApplication() {}

TaskRunner& BoymueApplication::getUITaskRunner() const {
  return m_uiThread.getTaskRunner();
}

TaskRunner& BoymueApplication::getIOTaskRunner() const {
  return m_ioThread.getTaskRunner();
}

TaskRunner& BoymueApplication::getJSTaskRunner() const {
  return m_jsThread.getTaskRunner();
}

const Loader& BoymueApplication::loader() const {
    return m_loader;
}

}  // namespace boymue
