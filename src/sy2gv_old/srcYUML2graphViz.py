##
# srcYUML2graphViz.py
#
# Copyright (C) 2015-2016 srcML, LLC. (www.srcML.org)
#
# This file is part of srcYUML.
#
# srcYUML is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# srcYUML is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with srcYUML.  If not, see <http://www.gnu.org/licenses/>.

from antlr4 import *
from antlr4.tree.Trees import Trees
from srcYUML2graphVizLexer import srcYUML2graphVizLexer
from srcYUML2graphVizParser import srcYUML2graphVizParser
from kitchen.text.converters import to_unicode

def main(argv):
    file = open(argv[1], "rb") #rb is R-read and B-binary
    input_str = to_unicode(file.read())
    file.close()
    input = InputStream(input_str)
    lexer = srcYUML2graphVizLexer(input)
    stream = CommonTokenStream(lexer)
    parser = srcYUML2graphVizParser(stream)
    tree = parser.yuml()
    print(Trees.toStringTree(tree, None, parser))

if __name__ == '__main__':
    import sys
    main(sys.argv)
