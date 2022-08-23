// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.09

#ifndef JsEngine_h
#define JsEngine_h

#include <memory>
#include <string>

#include "JsApiInterface.h"

using namespace std;

namespace boymue {
class JsRuntime;
// 可以使用JSRuntime提供的lambda规则来运行异步任务
using RuntimeClosure = std::function<void(JsRuntime*)>;

class BoymueApplication;
class JsRuntime {
 public:
  virtual void setContext(BoymueApplication* app) = 0;
  virtual ~JsRuntime() {}

  virtual void doAction(const RuntimeClosure& action) = 0;
  virtual void evaluateJs(const std::string& jsSource) = 0;
  virtual void registerApi(JsApiInterface* api) = 0;
};

class JsInitor;
class JsEngine {
 public:
  JsEngine();
  ~JsEngine();

  JsRuntime* createRuntime();

 private:
  unique_ptr<JsInitor> m_initor;
};
}  // namespace boymue
#endif