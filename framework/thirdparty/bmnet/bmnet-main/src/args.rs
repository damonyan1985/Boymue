use std::os::raw::c_char;

#[repr(C)]
pub struct Argumemnt {
    pub arg_type: u8,
    pub arg_value: ArgumemntValue,
}

#[repr(C)]
pub union ArgumemntValue {
    pub i32_value: i32,
    pub usize_value: usize,
    pub str_value: *const c_char,
}

#[repr(C)]
pub struct ArgumemntList {
    pub arg_count: i32,
    pub arg_list: *mut Argumemnt,
}

pub type FnCallback = unsafe extern "C" fn(args: ArgumemntList);