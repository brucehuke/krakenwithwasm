// The entry file of your WebAssembly module.

@external("env", "add_native")
declare function add_native(a: i32,b:i32): i32

export function add(a: i32, b: i32): i32 {
  var ret = a+b;
  return ret;
}

export function addf(a: f64, b: f64): f64 {
  var ret = a+b;
  return ret;
}

export function add2(a: i32, b: i32,c:i32): i32 {
  var ret = a+b+c;
  return ret ;
}
