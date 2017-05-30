# ofxOSSIA

## Introduction

This openFrameworks addon is a wrapper for the [OSSIA library](https://github.com/OSSIA/libossia) , 
allowing you to share parameters with i-score: the class `ossia::Parameter` 
inheriting the openFrameworks' class `ofParameter` can be controlled both via the UI and [i-score](http://www.i-score.org).

## Dependencies 

* Tested on Linux (Debian/stretch, Antergos), macOS, Windows

* The OSSIA library (libossia) require some boost libs not available in OpenFrameworks. They are installed by the build script.
    
## Installation (OS X, Linux) 

* Compile the OSSIA library: go to the folder `libs/` and run the script `build.sh`
* Include the header file `ofxOssia.h` to use the addon
* compilation of your project must been made through Terminal / make and not through xcode.
* You could also use [ofnode](https://github.com/ofnode/of.git), a CMake build system for openFrameworks 

## Installation (Windows)

* The simplest way is to use one of the releases.
* Once your application is built, do not forget to **copy the relevant DLL** from the libs/ossia/bin folder, to your executable folder.
* For compiling the OSSIA API by hand, refer to the [OSSIA API wiki](https://github.com/OSSIA/API/wiki).

## Troubleshooting

* In case of the following error : `execvp: /bin/sh: Argument list too long`: check that you do not have a boost folder in your example folder.

* If you got : `error: parse error in template argument list` this is likely due to a [GCC bug](https://gcc.gnu.org/ml/gcc-help/2016-01/msg00087.html), try update to gcc 6 or use another compiler.

* It might be a clash with other addon that uses `oscpack` library like `ofxOsc`.

## Main features

* Parameters of various types (int, float, bool, ofVec2f, ofVec3f, ofColor) can be shared with i-score
* Can be exposed in the openFrameworks GUI (modified via slider, button, etc.)
* Can be modified using i-score (automation ...)

## Usage

* Define an attribute of class `ofxOssia` in your app source header (e.g. `ofApp.h`)
* If the constructor for the attribute is not called explicitly, the attribute `ofxOssia` will be initialized with the default values (e.g. "OSCQuery", "ofxOssiaDevice", 3456, 5678)
* The base node is obtained by calling the method `get_root_node()` on the `ofxOssia` instance
* The `ossia::Parameter` is initialized using the method `setup` similar to `ofParameter::setup`. The only difference is the first value which is the parent node of type `ossia::Parameter`
* The same goes for  `ossia::ParameterGroup` (which is similar to `ofParameterGroup`)
