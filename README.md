# Nilib
Nilla's common base classes and functions library. Used as a submodule in various projects. 
Not yet complete. Not completely bug free and safe. 

# Includes
- ML library based on computational nodes (layers) that can be chained together
- A simple window class for capturing user input and drawing simple shapes to a window. 
- A more intricit window class for drawing shaded shapes and more complicated objects (text(ures)) to a window, work in progress.
- An argparser, WIP
- A config file parser.
- Running statistics class for calculation of staticstics on the fly. 
- CMake build system including tests, examples and debug and release mode. 
- A logging system that can be tuned with preprocessor directives, different log output targets and color mode. 
- Assert library for debugging and safe code. 
- Some base VRP instances/solution formats.
- A class representing colors HSL, HSV, RGB(a)
- Linear algebra library for simple matrix operations. 
- Templated beam search function.
- Quick and fast custom Random Number Generator. 
- Utilities to detect signals from the operating system. 
- Macro's to profile functions and scope. 
- Circular buffer.
- Binary serialization and deserialization of simple objects. 


# Dependencies
- GLFW, glad and opengl for cross platform window functionality. 
- stb for saving screenshots to file.  
- CMake for building. 
- Gurobi in the future.
