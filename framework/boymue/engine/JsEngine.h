// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.09

#ifndef JsEngine_h
#define JsEngine_h

#include "JsApiInterface.h"
#include <string>
#include <memory>

using namespace std;

namespace boymue {
class BoymueApplication;
class JsRuntime {
 public:
  virtual void setContext(BoymueApplication* app) = 0;
  virtual ~JsRuntime() {}
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