from antlr4 import *
from antlr4.tree.Trees import Trees
from srcYUML2graphVizLexer import srcYUML2graphVizLexer
from srcYUML2graphVizParser import srcYUML2graphVizParser

def main(argv):
    input = FileStream(argv[1])
    lexer = srcYUML2graphVizLexer(input)
    stream = CommonTokenStream(lexer)
    parser = srcYUML2graphVizParser(stream)
    tree = parser.yuml()
    print(Trees.toStringTree(tree, None, parser))

if __name__ == '__main__':
    import sys
    main(sys.argv)