// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.09

#include "JsEngine.h"
#include "BoymueApplication.h"
#include "StringUtil.h"

#ifdef ENABLE_BOYMUE_IOS
#include "qjs/include/cutils.h"
#include "qjs/include/quickjs.h"
#include "qjs/include/quickjs-libc.h"
#else
#include "v8.h"
#include "libplatform/libplatform.h"
#endif

namespace boymue {
#if ENABLE_BOYMUE_IOS

#define KB (1024)
#define MB (1024*1024)

// JsApi回调实现
class JsApiCallbackImpl : public JsApiCallback {
public:
    JsApiCallbackImpl(JsApiInterface* api,
                      JSContext* context,
                      JSValue obj,
                      JSValue callback)
        : m_api(api)
        , m_context(context)
        , m_object(JS_DupValue(context, obj))
        , m_callback(JS_DupValue(context, callback)) {
    }

    // 释放引用计数
    ~JsApiCallbackImpl() {
        JS_FreeValue(m_context, m_object);
        JS_FreeValue(m_context, m_callback);
    }

    // 需要保证callback在JS线程中被执行
    virtual void callback(const String& result) {
        // 如果是异步处理
        if (m_api->executor()) {
            RuntimeClosure task = [self = this, result](JsRuntime* runtime) {
                self->callbackImpl(result);
            };
            m_api->context()->doRuntimeAction(task);
        } else {
            callbackImpl(result);
        }
    }
    
private:
    void callbackImpl(const String& result) {
        JSValue jsRet[1] =  {
            JS_NewString(m_context, result.c_str())
        };
        JS_Call(m_context,
                m_callback,
                m_object, 1, jsRet);

        // 使用完之后清除内存
        delete this;
    }
    
    JsApiInterface* m_api;
    // js运行时上下文
    JSContext* m_context;
    // 回调所属的对象
    JSValue m_object;
    // 回调函数
    JSValue m_callback;
};

// API扩展函数
static JSValue JsApiHandlerImpl(JSContext *ctx, JSValueConst this_val,
                int argc, JSValueConst *argv, void *external) {
    //JSValue obj;
    if (argc < 1) {
        return JS_UNDEFINED;
    }
    
    size_t len;
    const char *str = JS_ToCStringLen(ctx, &len, argv[0]);
    JsApiInterface* api = static_cast<JsApiInterface*>(external);
    
    JsApiCallback* cb = nullptr;
    // 第二个参数是回调，需要使用JsApiCallbackImpl进行包装
    if (argc > 1) {
        cb = new JsApiCallbackImpl(api, ctx, this_val, argv[1]);
    }
    
    closure task = [api, str, len, cb] {
      if (api) {
          api->execute(String(str, len), cb);
      }
    };
    if (api->executor()) {
        api->executor()->submitTask(task);
    } else {
        task();
    }

    return JS_UNDEFINED;
}

// 使用qjs
class JsRuntimeImpl : public JsRuntime {
 public:
    JsRuntimeImpl()
        : m_app(nullptr) {
        // 初始化js运行时
        m_runtime = JS_NewRuntime();
        JS_SetMemoryLimit(m_runtime, 8 * MB);
        JS_SetMaxStackSize(m_runtime, 1 * MB);
        js_std_init_handlers(m_runtime);
        // 初始化JS context
        JSContext* ctx = JS_NewContext(m_runtime);
        if (!ctx)
            return;
        // 在此处添加内置对象或者函数
        /* system modules */
        js_init_module_std(ctx, "std");
        js_init_module_os(ctx, "os");
                        
        JS_SetModuleLoaderFunc(m_runtime, NULL, js_module_loader, NULL);
        
        m_context = ctx;
    }
    
    // qjs使用js_call_c_function来执行native function
    virtual void setContext(BoymueApplication* app) {
        m_app = app;
        if (m_apiEntries.size() == 0) {
            return;
        }
        
        m_global = JS_AddGlobalObject(m_context,
                           StringUtil::kAppPrefix.c_str(),
                           m_apiEntries.data(),
                           (int)m_apiEntries.size());
    }
    
    virtual ~JsRuntimeImpl() {
        // 释放全局对象
        js_free(m_context, m_global);
        
        JS_FreeContext(m_context);
        JS_FreeRuntime(m_runtime);
    }

    // 执行的回调也是宏任务
    virtual void doAction(const RuntimeClosure& action) {
        action(this);
        // 宏任务结束处理微任务
        doMicroTask();
    };
    
    // 执行JS代码相当于处理一次宏任务
    virtual void evaluateJs(const String& jsSource, const String& scriptId) {
        JS_Eval(m_context, jsSource.data(), jsSource.length(), scriptId.c_str(), 0);
        // 宏任务结束处理微任务
        doMicroTask();
    };
    
    virtual void registerApi(JsApiInterface* api) {
        // JS_CFUNC_EXTERNAL_DEF中第2个表示参数个数, api中只有两个参数
        JSCFunctionListEntry entry = JS_CFUNC_EXTERNAL_DEF(api->name(), 2, JsApiHandlerImpl, api);
        m_apiEntries.push_back(entry);
    }
    
private:
    // 处理微任务列表中的任务
    // 当异步任务执行完成后会添加一个微任务到微任务列表
    // 在宏任务执行完成后需要处理这些微任务
    void doMicroTask() {
        for (;;) {
            int err = JS_ExecutePendingJob(m_runtime, &m_context);
            if (err <= 0) {
                if (err < 0) {
                    js_std_dump_error(m_context);
                }
                break;
            }
        }
    }
    
    Vector<JSCFunctionListEntry> m_apiEntries;
    JSRuntime* m_runtime;
    JSContext* m_context;
    BoymueApplication* m_app;
    void* m_global;
};

class JsInitor {
 public:
  JsInitor() {
  }

  ~JsInitor() {
  }

 private:
};

// unique_ptr can be return for right reference by compiler
JsEngine::JsEngine() : m_initor(make_unique<JsInitor>()) {}

JsEngine::~JsEngine() {}

JsRuntime* JsEngine::createRuntime() { return new JsRuntimeImpl(); }

#else
  
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
  JsApiCallbackImpl(const FunctionCallbackInfo<v8::Value>& args,
                    BoymueApplication* context)
      : m_args(args), m_context(context) {
    m_receiver.Reset(args.GetIsolate(), args.This());
    if (args.Length() > 1 && args[1]->IsFunction()) {
      m_callback.Reset(args.GetIsolate(), Local<Function>::Cast(args[1]));
    }
  }

  ~JsApiCallbackImpl() {
      m_callback.Reset();
      m_receiver.Reset();
  }

  // 回调可能是异步的，因此需要设置Scope
  // callback中的内容必须回调给JS线程
  virtual void callback(const std::string& result) {
    RuntimeClosure task = [self = this, result](JsRuntime* runtime) {
      // 实现异步回调
      Local<Value> argv[] = {
          String::NewFromUtf8(runtime->getIsolate(), result.c_str())};
      Local<Context> context = runtime->getJsContext();

      Context::Scope contextScope(context);
      // selfm_callback->Call(context, context->Global(), 1, argv);
      Local<Function> jsCallback =
          Local<Function>::New(runtime->getIsolate(), self->m_callback);

      Local<Object> jsReceiver =
          Local<Object>::New(runtime->getIsolate(), self->m_receiver);
      jsCallback->Call(context, jsReceiver, 1, argv);

      delete self;
    };

    if (m_callback.IsEmpty()) {
      if (result.length()) {
        Local<String> jsResult =
            String::NewFromUtf8(m_args.GetIsolate(), result.c_str());
        m_args.GetReturnValue().Set(jsResult);
      } else {
        m_args.GetReturnValue().SetUndefined();
      }

      delete this;
    } else {
      m_context->doRuntimeAction(task);
    }
  }

 public:
  FunctionCallbackInfo<v8::Value> m_args;
  BoymueApplication* m_context;
  Persistent<Function> m_callback;
  Persistent<Object> m_receiver;
};

// JsApiHandler模板函数实现
void JsApiHandlerImpl(const FunctionCallbackInfo<v8::Value>& args) {
  Local<External> data = Local<External>::Cast(args.Data());
  JsApiInterface* jsApi = static_cast<JsApiInterface*>(data->Value());

  HandleScope handleScope(args.GetIsolate());
  // String::Utf8Value str(args[0]);
  if (!jsApi) {
    args.GetReturnValue().SetUndefined();
    return;
  }

  closure task = [jsApi, args] {
    if (jsApi) {
      String::Utf8Value str(args[0]);
      jsApi->execute(*str, new JsApiCallbackImpl(args, jsApi->context()));
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
    m_arrayBufferAllocator = new ArrayBufferAllocator;
    Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = m_arrayBufferAllocator;
    m_isolate = Isolate::New(create_params);

    initRuntime();
  }

  ~JsRuntimeImpl() {
    m_context.Reset();
    m_isolate->Dispose();
    delete m_arrayBufferAllocator;
  }

  v8::Isolate* getIsolate() { return m_isolate; }

  Local<Context> getJsContext() {
    return Local<Context>::New(m_isolate, m_context);
  }    

  void doAction(const RuntimeClosure& action) {
    // Enter isolate scope
    Isolate::Scope isolateScope(m_isolate);
    // local stack
    HandleScope handleScope(m_isolate);
    // Create local context
    // Local<Context> context = Local<Context>::New(m_isolate, m_context);
    // Enter current context
    // Context::Scope contextScope(context);
    // Do what you want
    action(this);
  }

  void setContext(BoymueApplication* app) { m_app = app; }

  virtual void registerApi(JsApiInterface* api) override {
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

  virtual void evaluateJs(const String& jsSource, const String& scriptId) override {
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
  ArrayBufferAllocator* m_arrayBufferAllocator;
  BoymueApplication* m_app;
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
#endif

}  // namespace boymue
