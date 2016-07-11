# Generated from srcYUML2graphViz.g4 by ANTLR 4.5.1
# encoding: utf-8
from __future__ import print_function
from antlr4 import *
from io import StringIO


def serializedATN():
    with StringIO() as buf:
        buf.write(u"\3\u0430\ud6d1\u8206\uad2d\u4417\uaef1\u8d80\uaadd\2")
        buf.write(u"\31_\b\1\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7")
        buf.write(u"\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t\13\4\f\t\f\4\r\t")
        buf.write(u"\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22")
        buf.write(u"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4")
        buf.write(u"\30\t\30\3\2\3\2\3\3\3\3\3\4\3\4\3\5\3\5\3\6\3\6\3\7")
        buf.write(u"\3\7\3\b\3\b\3\t\3\t\3\n\3\n\3\13\3\13\3\f\3\f\3\r\3")
        buf.write(u"\r\3\16\3\16\3\17\3\17\3\20\3\20\3\21\3\21\3\22\3\22")
        buf.write(u"\3\23\3\23\3\24\3\24\3\25\3\25\3\26\3\26\3\27\3\27\3")
        buf.write(u"\30\3\30\2\2\31\3\3\5\4\7\5\t\6\13\7\r\b\17\t\21\n\23")
        buf.write(u"\13\25\f\27\r\31\16\33\17\35\20\37\21!\22#\23%\24\'\25")
        buf.write(u")\26+\27-\30/\31\3\2\4\4\2C\\c|\3\2\62;^\2\3\3\2\2\2")
        buf.write(u"\2\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2\2\2\2\13\3\2\2\2\2\r")
        buf.write(u"\3\2\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2\25")
        buf.write(u"\3\2\2\2\2\27\3\2\2\2\2\31\3\2\2\2\2\33\3\2\2\2\2\35")
        buf.write(u"\3\2\2\2\2\37\3\2\2\2\2!\3\2\2\2\2#\3\2\2\2\2%\3\2\2")
        buf.write(u"\2\2\'\3\2\2\2\2)\3\2\2\2\2+\3\2\2\2\2-\3\2\2\2\2/\3")
        buf.write(u"\2\2\2\3\61\3\2\2\2\5\63\3\2\2\2\7\65\3\2\2\2\t\67\3")
        buf.write(u"\2\2\2\139\3\2\2\2\r;\3\2\2\2\17=\3\2\2\2\21?\3\2\2\2")
        buf.write(u"\23A\3\2\2\2\25C\3\2\2\2\27E\3\2\2\2\31G\3\2\2\2\33I")
        buf.write(u"\3\2\2\2\35K\3\2\2\2\37M\3\2\2\2!O\3\2\2\2#Q\3\2\2\2")
        buf.write(u"%S\3\2\2\2\'U\3\2\2\2)W\3\2\2\2+Y\3\2\2\2-[\3\2\2\2/")
        buf.write(u"]\3\2\2\2\61\62\7]\2\2\62\4\3\2\2\2\63\64\7~\2\2\64\6")
        buf.write(u"\3\2\2\2\65\66\7_\2\2\66\b\3\2\2\2\678\7-\2\28\n\3\2")
        buf.write(u"\2\29:\7/\2\2:\f\3\2\2\2;<\7@\2\2<\16\3\2\2\2=>\7`\2")
        buf.write(u"\2>\20\3\2\2\2?@\7\60\2\2@\22\3\2\2\2AB\7=\2\2B\24\3")
        buf.write(u"\2\2\2CD\7%\2\2D\26\3\2\2\2EF\7>\2\2F\30\3\2\2\2GH\7")
        buf.write(u"*\2\2H\32\3\2\2\2IJ\7+\2\2J\34\3\2\2\2KL\7<\2\2L\36\3")
        buf.write(u"\2\2\2MN\7\"\2\2N \3\2\2\2OP\7,\2\2P\"\3\2\2\2QR\7\uff3d")
        buf.write(u"\2\2R$\3\2\2\2ST\7\uff3f\2\2T&\3\2\2\2UV\7\u00ad\2\2")
        buf.write(u"V(\3\2\2\2WX\7\u00bd\2\2X*\3\2\2\2YZ\t\2\2\2Z,\3\2\2")
        buf.write(u"\2[\\\t\3\2\2\\.\3\2\2\2]^\7\f\2\2^\60\3\2\2\2\3\2\2")
        return buf.getvalue()


class srcYUML2graphVizLexer(Lexer):

    atn = ATNDeserializer().deserialize(serializedATN())

    decisionsToDFA = [ DFA(ds, i) for i, ds in enumerate(atn.decisionToState) ]


    T__0 = 1
    T__1 = 2
    T__2 = 3
    T__3 = 4
    T__4 = 5
    T__5 = 6
    T__6 = 7
    T__7 = 8
    T__8 = 9
    T__9 = 10
    T__10 = 11
    T__11 = 12
    T__12 = 13
    T__13 = 14
    T__14 = 15
    T__15 = 16
    T__16 = 17
    T__17 = 18
    T__18 = 19
    T__19 = 20
    LETTER = 21
    NUMBER = 22
    NEWLINE = 23

    modeNames = [ u"DEFAULT_MODE" ]

    literalNames = [ u"<INVALID>",
            u"'['", u"'|'", u"']'", u"'+'", u"'-'", u"'>'", u"'^'", u"'.'", 
            u"';'", u"'#'", u"'<'", u"'('", u"')'", u"':'", u"' '", u"'*'", 
            u"'［'", u"'］'", u"'«'", u"'»'", u"'\n'" ]

    symbolicNames = [ u"<INVALID>",
            u"LETTER", u"NUMBER", u"NEWLINE" ]

    ruleNames = [ u"T__0", u"T__1", u"T__2", u"T__3", u"T__4", u"T__5", 
                  u"T__6", u"T__7", u"T__8", u"T__9", u"T__10", u"T__11", 
                  u"T__12", u"T__13", u"T__14", u"T__15", u"T__16", u"T__17", 
                  u"T__18", u"T__19", u"LETTER", u"NUMBER", u"NEWLINE" ]

    grammarFileName = u"srcYUML2graphViz.g4"

    def __init__(self, input=None):
        super(srcYUML2graphVizLexer, self).__init__(input)
        self.checkVersion("4.5.1")
        self._interp = LexerATNSimulator(self, self.atn, self.decisionsToDFA, PredictionContextCache())
        self._actions = None
        self._predicates = None


