# Boymue

## 引擎原则
```
1. 本引擎目的仅用于学习C++，Rust及V8， skia，以及渲染引擎所用
```

## 主要目的
```
1. 深入探索V8引擎以及Skia 2d绘制引擎原理
2. 自渲染跨端应用开发引擎的深入探索与实验
3. 主要使用C++17来进行调研，开发平台目前仅限于android，windows以及IOS
4. 网络请求等具体功能将采用rust实现，减少跨平台代码编写带来的成本
```
## 相关编译事项
```
1. C++17不支持异常处理，需要添加-Wno-dynamic-exception-spec进行忽略异常处理，IOS中在Other Warning Flags中添加
2. 鉴于目前ios将opengles2标注为过期，使用GLES_SILENCE_DEPRECATION来忽略代码中的警告
3. Windows与Android上将使用V8 JS引擎，IOS上由于不能开启JIT，所以采用包更小的QuickJS
4. ios framework中创建bundle，直接使用target创建
5. ios copy file中需要将framework的destination从File改为frameworks
```

## 相关知识点
### QuickJS
```
1. 执行入口，JS_Eval实际上是使用了__JS_EvalInternal函数进行调用
2. 字符串管理，JSAtom管理字符串，如关键字，标识符，常量字符串
3. 解释执行入口，JS_CallInternal，执行字节码，具体逻辑：
   1）创建local_buf，容量包含函数参数个数arg_count，var_count，以及栈大小stack_size，采用alloca申请栈内存
      函数结束后会自动释放
   2) 字节码定义，字节码存储在opcode_info数组中
4. 微任务
   1）promise调用异步函数执行完后，会调用js_promise_resolve_function_call
   将微任务加入队列，例如JS_EnqueueJob(ctx, promise_reaction_job, 5, args)
```