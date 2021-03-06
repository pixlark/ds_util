### ds_util

This is a single header-file library containing useful functions and structures. The implementation is guarded by `DS_UTIL_IMPLEMENTATION` in the spirit of the [stb libraries](https://github.com/nothings/stb).

#### Vector2f and Vector2i

Simple xy point structs. Functions exist for addition, scalar multiplication, and dot products.

#### List\<T>

This is a simple dynamically resizing array with a growth ratio of 2. There is no constructor, you simply initialize by calling `List<T>.alloc()`. Deallocate with `List<T>.dealloc()`. There's no protection against double freeing here, so make sure you don't call `dealloc` when you haven't called `alloc`.  The remaining `List<T>` functions are self-explanatory, just look in the struct definition.

#### String_Builder

`String_Builder` is a basic string builder built around `List<T>`. `alloc` and `dealloc` are the same as with `List<T>`. Use the self-explanatory member functions to modify the string, and call `String_Builder.str()` to get the built string. This doesn't allocate memory, just updates the null terminator and gives you a pointer into the struct's memory.

#### OS-Specific Paths

`auto_convert_path_seperators` takes a buffer and modifies the path seperators in it to conform to the current platform ('\\' for windows and '/' for everything else).

`get_executable_path` returns a path to the executable on Windows using `GetModuleFileName` and on *nix using `readlink`.

### Changelog

*1.3.1* - Small change to HashTable<K,V>

*1.3* - HashTable<K,V>

*1.2.1* - String_Builder final_str for allocating a new copy, append and prepend use strlen by default

*1.2* - load_string_from_file + defer statement

*1.1.1* - Small changes to function names and tests

*1.1* - Vector convenience functions, List insert + insert_many

*1.0* - Initial version based on old utility library
