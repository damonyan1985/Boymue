# Rust学习

## 生命周期
```
1. 生命周期主要针对函数的引用返回值，只要函数的返回值是引用就需要加入生命周期的注释
2. Rust编译期无法判断引用返回值的生命周期是否是有效的
3. &str赋值给普通变量, 会调用copy，这与&String不同，str有实现copy trait, 而String没有
```

## Features, 条件编译
```
1. 禁用默认feature, 使用default-features = false
```