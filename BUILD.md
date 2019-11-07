# Build and Installation Guide #

## Dependencies ##
- [OGDF](https://github.com/ogdf/ogdf)
- libxml2
	- Installed through apt (libxml++2.6-2v5)
- [Boost](https://www.boost.org/)
	- Required Components {program_options, filesystem, system}

## Build ##
The project utilizes CMake for building.
The following provide examples of how to build.  srcuml will be placed in the ./bin directory in all cases.

```bash
# in-source build
cmake .
make

# out-of-source build
mkdir srcuml_build
cd srcuml_build
cmake path_to_srcUML_repo
make

# release mode
mkdir srcuml_build
cd srcuml_build
cmake -DCMAKE_BUILD_TYPE=Release path_to_srcUML_repo
make
```

## Install ##
To install to your system, use
```bash
sudo make install
```
in the build directory.

