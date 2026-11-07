# winc
Compiler and stuff for Winc (Winc Is Not C), a programming language based on C.
The compiler is based on chibicc.

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

Here is an example:
```wc
// person.wc

#include <stdlib.h>
#include <string.h>

pub struct person {
    char name[25];
    unsigned int age;
};

pub struct person *create_person(const char *name, unsigned int age) {
    struct person *p = calloc(1, sizeof(*p));
    if (!p)
        return NULL;

    strncpy(p->name, name, strlen(name));
    p->age = age;
    return p;
}

pub void kill_person(struct person *p) {
    free(p);
}
```
```wc
// main.wc

#include <stdio.h>

import "person";

int main(void) {
    struct person *p = create_person("John Doe", 42);
    printf("Hi! My name is %s and I am %u years old!\n", p->name, p->age);
    kill_person(p);

    return 0;
}
```
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