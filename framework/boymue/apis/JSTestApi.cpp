// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.02.02
#include "JSTestApi.h"

namespace boymue {
const char* JsTestApi::name() const { return "test"; }

JsTestApi::JsTestApi(BoymueApplication* context) : m_context(context) {}

BoymueApplication* JsTestApi::context() const { return m_context; }

void JsTestApi::execute(const std::string& params, JsApiCallback* callback) {
  printf("Boymue: %s\n", params.c_str());
  if (callback) {
    callback->callback("");
  }
}
}  // namespace boymue
