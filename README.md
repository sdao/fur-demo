gldemo
======

OpenGL Core Profile 3.3 shell app using GLFW and GLEW.

This project is a template for OpenGL apps wanting to run on OpenGL 3.3 Core Profile.
It's only been tested on Mac OS X 10.9 Lion, but OpenGL 3.3 should be available on most recent operating systems.
The Core Profile means that the fixed-function pipeline is unavailable, and everything must be done through shaders.
GLSL 3.3 (`#version 330`) uses the new-style syntax with `in` and `out` instead of `attribute` and `varying`.
The provided shaders show off simple texture loading from PNGs using libpng.

To compile, you'll need GLFW, GLEW, GLM (OpenGL Mathematics), and libpng.

Unlicense
=========
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org>
