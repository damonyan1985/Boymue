use std::os::raw::c_char;

#[derive(Copy, Clone)]
#[repr(C)]
pub struct ArgumentString {
    pub str_value: *const c_char,
    pub str_len: usize,
}

impl ArgumentString {
    /// `str_len` 为有效字节数；`str_value` 为空或 `str_len == 0` 时视为空串。
    pub unsafe fn to_string_lossy(&self) -> String {
        if self.str_len == 0 {
            return String::new();
        }
        if self.str_value.is_null() {
            return String::new();
        }
        let slice = std::slice::from_raw_parts(self.str_value as *const u8, self.str_len);
        String::from_utf8_lossy(slice).into_owned()
    }
}

#[repr(C)]
pub struct Argument {
    pub arg_type: u8,
    pub arg_value: ArgumentValue,
}

#[repr(C)]
pub union ArgumentValue {
    pub i32_value: i32,
    pub usize_value: usize,
    pub str: ArgumentString,
}

#[repr(C)]
pub struct ArgumentList {
    pub arg_count: i32,
    pub arg_list: *mut Argument,
}

pub type FnCallback = unsafe extern "C" fn(args: ArgumentList);