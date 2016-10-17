# ofxOSSIA

**Introduction**

This openFrameworks addon allows you to share parameters with i-score: the class `ossia::Parameter` inheriting the openFrameworks' class `ofParameter` can be controlled both via the UI and i-score.

**Dependencies**

* Tested on Linux (debian/stretch, antergos) and OSX


**Installation**

* Compile the OSSIA library: go to the folder `libs/` and run the script `build.sh`
* Copy the three dynamic libraries created in `libs/ossia/libs/(OS_name)` to your app's `bin/` folder
* Include the header file `ofxOssia.hpp` to use the addon

**Main features**

* Parameters of various types (int, float, bool, ofVec2f, ofVec3f, ofColor) can be shared with i-score
* Can be exposed in the openFrameworks GUI (modified via slider, button, etc.)
* Can be modified using i-score (automation ...)

**Usage**

* Define an attribute of class `Network` in your app source header (e.g. `ofApp.h`)
* N.B. if the constructor for the attribute is not called explicitly, the attribute `Network` will be initialized with the default values (e.g. "newDevice", "i-score", "127.0.0.1", 13579, 9998)
* The `ossia::Parameter` is initialized using the method `setup` similar to `ofParameter::setup`. The only difference is the first value which is the base node obtained calling the method `getSceneNode()` on the `Network` instance
