# Rust学习

## 生命周期
```
1. 生命周期主要针对函数的引用返回值，只要函数的返回值是引用就需要加入生命周期的注释
2. Rust编译期无法判断引用返回值的生命周期是否是有效的
3. &str赋值给普通变量，会调用copy，这与&String不同，str有实现copy trait, 而String没有
4. rust中trait类似java interface,但又有不同，其又类似OC中catetory类似，catetory能扩展方法而不能扩展属性
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
4. Box使用into_raw转换成原始指针后，Box自身销毁时，就不会同时销毁指向的内存，只能自己销毁原始指针指向的内存
```

## 原始指针
```
1. 强转为原始指针时无需加unsafe
2. 使用原始指针访问数据时即解引用，必须加unsafe
```

## 胖指针
```
1. 切片类胖指针
2. 对象类胖指针
3. 引用实际上就是一个胖指针
```

## 编译
```
1. IOS模拟器编译，cargo build --target x86_64-apple-ios --release
2. IOS真机编译，cargo build --target aarch64-apple-ios --release
3. Android编译，cargo build --target aarch64-linux-android --release
```