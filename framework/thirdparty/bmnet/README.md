# Rust学习

## 生命周期
```
1. 生命周期主要针对函数的引用返回值，只要函数的返回值是引用就需要加入生命周期的注释
2. Rust编译期无法判断引用返回值的生命周期是否是有效的
3. &str赋值给普通变量，会调用copy，这与&String不同，str有实现copy trait, 而String没有
```

## Features, 条件编译
```
1. 禁用默认feature, 使用default-features = false
```

## 问号的使用
```
1. 问号主要用于处理Result, Option的情况
2. 使用问号后如果结果是Err/None，问号会自动return返回，因此函数的返回值也必须是Result/Option枚举
```

## 智能指针
```
智能指针主要针对堆上对象分配
1. Box所有权下的智能指针，类似unique_ptr
2. Rc引用计数智能指针，类似shared_ptr
3. Arc原子化的Rc智能指针，处理多线程并发，类似atomic_shared_ptr
```

## 编译
```
1. IOS模拟器编译，cargo build --target x86_64-apple-ios --release
2. IOS真机编译，cargo build --target aarch64-apple-ios --release
3. Android编译，cargo build --target aarch64-linux-android --release
```