# Boymue

## 引擎原则
```
1. 本引擎目的仅用于学习C++及V8, skia，以及渲染引擎所用
```

## 主要目的
```
1. 深入探索V8引擎以及Skia 2d绘制引擎原理
2. 自渲染跨端应用开发引擎的深入探索与实验
3. 主要使用C++17来进行调研，开发平台目前仅限于android, windows以及IOS
```
## 相关编译事项
```
1. C++17不支持异常处理，需要添加-Wno-dynamic-exception-spec进行忽略异常处理, IOS中在Other Warning Flags中添加
2. 鉴于目前ios将opengles2标注为过期，使用GLES_SILENCE_DEPRECATION来忽略代码中的警告
3. Windows与Android上将使用V8 JS引擎，IOS上由于不能开启JIT，所以采用包更小的QuickJS
4. ios framework中创建bundle，直接使用target创建
```