//! `Argument.arg_type` 取值，与 `bmnet_main.h` 中 `BmnetArgType` / `BMNET_ARG_*` 一致。

pub mod args_type {
    pub const ARG_TYPE_I32: u8 = 1;
    pub const ARG_TYPE_STRING: u8 = 2;
    pub const ARG_TYPE_USIZE: u8 = 3;
}
