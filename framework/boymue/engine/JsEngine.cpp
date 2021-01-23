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

// JsApi回调实现
class JsApiCallbackImpl : public JsApiCallback {
 public:
  JsApiCallbackImpl(const FunctionCallbackInfo<v8::Value>& args)
      : m_args(args) {}

  // 回调可能是异步的，因此需要设置Scope
  virtual void callback(const std::string& result) {
    Isolate* isolate = m_args.GetIsolate();
    Isolate::Scope isolateScope(isolate);
    HandleScope handleScope(isolate);
    // 如果result长度不为0，则回调，否则回调undefined
    if (result.length()) {
      Local<String> jsResult = String::NewFromUtf8(isolate, result.c_str());
      m_args.GetReturnValue().Set(jsResult);
    } else {
      m_args.GetReturnValue().SetUndefined();
    }
  }

 private:
  FunctionCallbackInfo<v8::Value> m_args;
};

// JsApiHandler模板函数实现
void JsApiHandlerImpl(const FunctionCallbackInfo<v8::Value>& args) {
  Local<External> data = Local<External>::Cast(args.Data());
  JsApiInterface* jsApi = static_cast<JsApiInterface*>(data->Value());

  HandleScope handleScope(args.GetIsolate());
  //String::Utf8Value str(args[0]);
  if (!jsApi) {
      args.GetReturnValue().SetUndefined();
      return;
  }

  closure task = [jsApi, args] {
      if (jsApi) {
          String::Utf8Value str(args[0]);
          jsApi->execute(*str, new JsApiCallbackImpl(args));
      }
  };
  if (jsApi->executor()) {
      jsApi->executor()->submitTask(task);
  } else {
      task();
  }
}

// 属性对象回调，返回属性对象
static void JsGlobalObjectAccessor(Local<String> property,
                                   const PropertyCallbackInfo<Value>& info) {
  info.GetReturnValue().Set(info.GetIsolate()->GetCurrentContext()->Global());
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

  virtual void registerApi(JsApiInterface* api) {
    // Enter isolate scope
    Isolate::Scope isolateScope(m_isolate);
    // local stack
    HandleScope handleScope(m_isolate);
    // Create local context
    Local<Context> context = Local<Context>::New(m_isolate, m_context);
    // Enter current context
    Context::Scope contextScope(context);

    Handle<Object> object = Local<Object>::New(m_isolate, m_global);

    Local<External> extension = External::New(m_isolate, api);

    object->Set(String::NewFromUtf8(m_isolate, api->name()),
                Function::New(m_isolate, JsApiHandlerImpl, extension));
  }

  virtual void evaluateJs(const std::string& jsSource) {
    // Enter isolate scope
    Isolate::Scope isolateScope(m_isolate);
    // local stack
    HandleScope handleScope(m_isolate);
    Local<Context> context = Local<Context>::New(m_isolate, m_context);
    // Enter current context
    Context::Scope contextScope(context);

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

    // 给Global对象设置一个属性，属性返回global对象自己
    Local<String> globalName = String::NewFromUtf8(m_isolate, "boymue");
    // 设置global对象的名字为boymue
    global->SetAccessor(globalName, JsGlobalObjectAccessor);

    Handle<Context> context = Context::New(m_isolate, nullptr, global);

    m_global.Reset(m_isolate,
                   context->Global()->GetPrototype()->ToObject(m_isolate));
    m_context.Reset(m_isolate, context);
  }

  Isolate* m_isolate;
  Persistent<Context> m_context;
  Persistent<Object> m_global;
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

// unique_ptr can be return for right reference by compiler
JsEngine::JsEngine() : m_initor(make_unique<JsInitor>()) {}

JsEngine::~JsEngine() {}

JsRuntime* JsEngine::createRuntime() { return new JsRuntimeImpl(); }
}  // namespace boymue