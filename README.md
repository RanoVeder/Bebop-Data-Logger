# Bebop Data Logger

**CI State**: [![Build Status](https://travis-ci.com/RanoVeder/Bebop-Data-Logger.svg?branch=master)](https://travis-ci.com/RanoVeder/Bebop-Data-Logger)

The **Bebop data logger** application is a stereo vision image data logger, to be used in conjunction with the [paprazzi tcp-logging module](https://github.com/RanoVeder/paparazzi/commit/d8bd32f5ad4bef2caae8276b046b7a69d12a1c4e).

Full system installation manual can be found [here](https://drive.google.com/open?id=19wJTQTj_YrZ1bTLv1t0einceqsWwRG_O).

# Building and Installing from source

To build **Bebop data logger**, no pre-installed dependencies are required.


After the development files for the previously stated dependencies have been installed/compiled, **Bebop data logger** can be compiled by:

```
$ cd bebop-data-logger
$ git submodule init
$ git submodule sync
$ git submodule update
$ mkdir build
$ cd build
$ cmake ..
$ make -j8
```


License
----

MIT

