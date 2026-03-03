fn main() {
    // 确保 bmnet-macros 被编译
    // 过程宏会在编译时自动处理，这里只是确保依赖被包含
    println!("cargo:rerun-if-changed=./bmnet-macros/src/lib.rs");
    println!("cargo:rerun-if-changed=./bmnet-macros/Cargo.toml");
    
    // 如果 bmnet-macros 中的文件发生变化，重新编译
    println!("cargo:rerun-if-changed=./bmnet-macros/src");
}

