// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.09

#include "JsEngine.h"

#include <string>

#include "libplatform/libplatform.h"
#include "v8.h"

namespace boymue {
using namespace v8;

class ArrayBufferAllocator : public ArrayBuffer::Allocator {
 public:
  virtual void* Allocate(size_t length) {
    void* data = AllocateUninitialized(length);
    return data == nullptr ? data : memset(data, 0, length);
  }
  virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
  virtual void Free(void* data, size_t) { free(data); }
};
const char* ToCString(const v8::String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

void Print(const FunctionCallbackInfo<v8::Value>& args) {
  bool first = true;
  for (int i = 0; i < args.Length(); i++) {
    HandleScope handle_scope(args.GetIsolate());
    if (first) {
      first = false;
    } else {
      printf(" ");
    }
    String::Utf8Value str(args[i]);
    const char* cstr = ToCString(str);
    printf("V8Core: %s", cstr);
  }
  printf("\n");
  fflush(stdout);
}

class JsRuntimeImpl : public JsRuntime {
 public:
  JsRuntimeImpl() {
    Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = new ArrayBufferAllocator;
    m_isolate = Isolate::New(create_params);

    initRuntime();
  }

  ~JsRuntimeImpl() {
      m_context.Reset();
      m_isolate->Dispose(); 
  }

  virtual void evaluateJs(const std::string& jsSource) {
    // Enter isolate scope
    Isolate::Scope isolateScope(m_isolate);
    // local stack
    HandleScope handle_scope(m_isolate);
    Local<Context> context = Local<Context>::New(m_isolate, m_context);
    // Enter current context
    Context::Scope context_scope(context);

    // Context::Scope context_scope(context);
    // Create a string containing the JavaScript source code.
    Local<String> source =
        String::NewFromUtf8(m_isolate, jsSource.c_str(), NewStringType::kNormal)
            .ToLocalChecked();

    // Compile the source code.
    Local<Script> script = Script::Compile(source);

    // Run the script to get the result.
    Local<Value> result = script->Run();
  }

 private:
  void initRuntime() {
    Isolate::Scope isolateScope(m_isolate);
    HandleScope handleScope(m_isolate);
    // Create a template for the global object.
    Handle<ObjectTemplate> global = ObjectTemplate::New(m_isolate);
    // Bind the global 'print' function to the C++ Print callback.
    global->Set(String::NewFromUtf8(m_isolate, "print", NewStringType::kNormal)
                    .ToLocalChecked(),
                FunctionTemplate::New(m_isolate, Print));

    Handle<Context> context = Context::New(m_isolate, nullptr, global);
    m_context.Reset(m_isolate, context);
  }

  Isolate* m_isolate;
  Persistent<Context> m_context;
};

class JsInitor {
 public:
  JsInitor() {
    // Initialize V8.
    V8::InitializeICU();
    // V8::InitializeExternalStartupData(argv[0]);
    m_platform = V8::InitializeDefaultPlatform();
    // V8::InitializePlatform(m_platform);
    V8::Initialize();
  }

  ~JsInitor() {
    V8::Dispose();
    V8::ShutdownPlatform();
    delete m_platform;
  }

 private:
  Platform* m_platform;
};
JsEngine::JsEngine() : m_initor(new JsInitor()) {}

JsEngine::~JsEngine() { delete m_initor; }

JsRuntime* JsEngine::createRuntime() { return new JsRuntimeImpl(); }
}  // namespace boymue