### srcYUML
* A tool for efficiently reverse engineering accurate UML class diagrams.

### Cloning
* The project uses srcSAX as a submodule.  The repo most be cloned with the `--recursive` option to clone the submodule.  The following is an example command:
```bash
git clone --recursive https://github.com/srcML/srcYUML.git
```

### Build
The project utilizes CMake for building.
The following provide examples of how to build.

```bash
// in-source build
cmake .

// out-of-source build
mkdir srcyuml_build
cmake path_to_srcYUML_repo

// release mode
cmake -DCMAKE_BUILD_TYPE=Release path_to_srcYUML_repo
```