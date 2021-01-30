#include "JsLogApi.h"

namespace boymue {
const char* JsLogApi::name() const { return "log"; }

JsLogApi::JsLogApi(BoymueApplication* context) : m_context(context) {}

BoymueApplication* JsLogApi::context() const { return m_context; }

void JsLogApi::execute(const std::string& params, JsApiCallback* callback) {
  printf("Boymue: %s", params.c_str());
  if (callback) {
    callback->callback("");
  }
}
}  // namespace boymue