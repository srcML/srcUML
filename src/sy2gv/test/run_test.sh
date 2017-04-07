#!/bin/bash
srcml $1 -o class_file_test.hpp.xml
srcyuml class_file_test.hpp.xml > class_file_test.hpp.yuml
./../srcYUML2graphViz.py class_file_test.hpp.yuml class_file_test.outxt
dot -Tpng class_file_test.outxt -o pic.png