# winc
Compiler and stuff for Winc (Winc Is Not C), a programming language based on C.
The compiler is based on chibicc.

## Basic example
Here is a basic program which uses the [standard library](#standard-library). Save it in a file, for example `hello.wc`.
```wc
import "std.io";
import "std.status";

int main() {
    println("Hello, world!");
    return SUCCESS;
}
```
To compile and execute it, run:
```sh
wincc --project . hello.wc -o hello -L"$HOME/.winc" -lwcstd
./hello
```

Check out the [examples/](examples) directory if you want to see more examples.

## Project organization
Organize your project however you want, with Makefiles and stuff.  
The conventional extension for a Winc file is `.wc`. Headers are not needed unless you wish to use C code.

# Features
All C features implemented by chibicc are currently available.  
However, I wont hesitate to remove, replace or modify an existing feature if I need to. C is a base, not a prison.  
You can find a list of custom features below.

### Modules
Even though `#include` is still available for C headers, modules are a cleaner way to organize the code.  
They are evaluated at compile-time and there is no need to split the interface and the implementation in several files. Also there is no need for header guards or `#pragma once`: a module is never evaluated twice.
In a module, all symbols are private by default. To export a symbol, place `pub` in front of its definition. This only works with global symbols.

You can crawl directories using `.`:
```wc
import "lib.net.utils.ipv4";
```
To compile the code, you need to tell the compiler about your project location.
```sh
wincc --project pathname # other arguments
```

### New types
* Boolean types: `bool`, with the keywords `true` and `false`
* Unsigned sized types: `u8`, `u16`, `u32`, `u64`
* Signed sized types: `s8`, `s16`, `s32`, `s64`
* Size types: `usize`, `ssize`

### Attributes
Winc provides a clean syntax for using attributes. To use them, follow the following pattern: `[[attr, ...]]`. You can also give arguments to attributes using `()` like you would do in a function call.

### Standard library
Once of Winc's goals is to provide a standard-library which provides more modern structures than the libc. The standard library is divided in two parts:
* `core`: everything in `core` does not require allocators nor filesystems nor tasks management.
* `std`: built on top of `core`, `std` provides more structures but relies on an operating system to work.

To install the standard library, run the following commands:
```sh
make -C libraries
cd libraries
chmod +x install.sh
./install.sh
```
It should be installed in `~/.winc`.

Since `std` uses separated assembly source files to implement some functions, you would need to link your executable with the corresponding objects.