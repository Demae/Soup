// Compile with: rustc soup.rs -L .
// Assuming soup.dll is in the same directory.

#[link(name = "Soup", kind = "dylib")]
extern {
	fn InquiryLang_execute(a1: *const std::os::raw::c_char) -> *const std::os::raw::c_void;
	fn InquiryLang_formatResultLine(a1: *const std::os::raw::c_void) -> *const std::os::raw::c_char;
}

fn main() {
	// This code was mostly generated by ChatGPT, but I can believe that this is about as compact as it gets.
	unsafe {
		let input = std::ffi::CString::new("base64_encode Hello").unwrap();
		let m = InquiryLang_execute(input.as_ptr());
		let result = std::ffi::CStr::from_ptr(InquiryLang_formatResultLine(m)).to_str().unwrap();
		println!("{}", result); // SGVsbG8=
	}
}
