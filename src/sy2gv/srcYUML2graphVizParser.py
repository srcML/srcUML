# Generated from srcYUML2graphViz.g4 by ANTLR 4.5.1
# encoding: utf-8
from __future__ import print_function
from antlr4 import *
from io import StringIO

def serializedATN():
    with StringIO() as buf:
        buf.write(u"\3\u0430\ud6d1\u8206\uad2d\u4417\uaef1\u8d80\uaadd\3")
        buf.write(u"\25\\\4\2\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7")
        buf.write(u"\4\b\t\b\4\t\t\t\3\2\3\2\3\2\6\2\26\n\2\r\2\16\2\27\3")
        buf.write(u"\2\3\2\3\3\3\3\3\3\3\3\3\3\5\3!\n\3\3\3\3\3\3\4\3\4\3")
        buf.write(u"\4\3\4\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\5\3\6\3\6\3\6\3")
        buf.write(u"\6\3\6\3\6\3\6\3\6\3\6\3\7\3\7\3\7\3\7\3\7\3\7\3\7\3")
        buf.write(u"\7\3\7\3\b\3\b\3\b\3\b\3\b\3\b\3\t\3\t\3\t\3\t\3\t\3")
        buf.write(u"\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\3\t\7\tW\n\t\f\t\16\t")
        buf.write(u"Z\13\t\3\t\2\2\n\2\4\6\b\n\f\16\20\2\2g\2\25\3\2\2\2")
        buf.write(u"\4\33\3\2\2\2\6$\3\2\2\2\b(\3\2\2\2\n\60\3\2\2\2\f9\3")
        buf.write(u"\2\2\2\16B\3\2\2\2\20X\3\2\2\2\22\26\5\6\4\2\23\26\5")
        buf.write(u"\4\3\2\24\26\7\25\2\2\25\22\3\2\2\2\25\23\3\2\2\2\25")
        buf.write(u"\24\3\2\2\2\26\27\3\2\2\2\27\25\3\2\2\2\27\30\3\2\2\2")
        buf.write(u"\30\31\3\2\2\2\31\32\7\2\2\3\32\3\3\2\2\2\33 \5\6\4\2")
        buf.write(u"\34!\5\b\5\2\35!\5\n\6\2\36!\5\f\7\2\37!\5\16\b\2 \34")
        buf.write(u"\3\2\2\2 \35\3\2\2\2 \36\3\2\2\2 \37\3\2\2\2!\"\3\2\2")
        buf.write(u"\2\"#\5\6\4\2#\5\3\2\2\2$%\7\3\2\2%&\5\20\t\2&\'\7\4")
        buf.write(u"\2\2\'\7\3\2\2\2()\5\20\t\2)*\7\5\2\2*+\5\20\t\2+,\7")
        buf.write(u"\6\2\2,-\5\20\t\2-.\7\7\2\2./\5\20\t\2/\t\3\2\2\2\60")
        buf.write(u"\61\5\20\t\2\61\62\7\5\2\2\62\63\5\20\t\2\63\64\7\5\2")
        buf.write(u"\2\64\65\5\20\t\2\65\66\7\6\2\2\66\67\5\20\t\2\678\7")
        buf.write(u"\7\2\28\13\3\2\2\29:\5\20\t\2:;\7\b\2\2;<\5\20\t\2<=")
        buf.write(u"\7\6\2\2=>\5\20\t\2>?\7\t\2\2?@\5\20\t\2@A\7\6\2\2A\r")
        buf.write(u"\3\2\2\2BC\5\20\t\2CD\7\b\2\2DE\5\20\t\2EF\7\6\2\2FG")
        buf.write(u"\5\20\t\2G\17\3\2\2\2HW\7\23\2\2IW\7\24\2\2JW\7\n\2\2")
        buf.write(u"KW\7\6\2\2LW\7\5\2\2MW\7\13\2\2NW\7\f\2\2OW\7\7\2\2P")
        buf.write(u"W\7\r\2\2QW\7\16\2\2RW\7\17\2\2SW\7\20\2\2TW\7\21\2\2")
        buf.write(u"UW\7\22\2\2VH\3\2\2\2VI\3\2\2\2VJ\3\2\2\2VK\3\2\2\2V")
        buf.write(u"L\3\2\2\2VM\3\2\2\2VN\3\2\2\2VO\3\2\2\2VP\3\2\2\2VQ\3")
        buf.write(u"\2\2\2VR\3\2\2\2VS\3\2\2\2VT\3\2\2\2VU\3\2\2\2WZ\3\2")
        buf.write(u"\2\2XV\3\2\2\2XY\3\2\2\2Y\21\3\2\2\2ZX\3\2\2\2\7\25\27")
        buf.write(u" VX")
        return buf.getvalue()


class srcYUML2graphVizParser ( Parser ):

    grammarFileName = "srcYUML2graphViz.g4"

    atn = ATNDeserializer().deserialize(serializedATN())

    decisionsToDFA = [ DFA(ds, i) for i, ds in enumerate(atn.decisionToState) ]

    sharedContextCache = PredictionContextCache()

    literalNames = [ u"<INVALID>", u"'['", u"']'", u"'+'", u"'-'", u"'>'", 
                     u"'^'", u"'.'", u"'|'", u"'#'", u"'<'", u"';'", u"'('", 
                     u"')'", u"':'", u"' '", u"'*'", u"<INVALID>", u"<INVALID>", 
                     u"'\n'" ]

    symbolicNames = [ u"<INVALID>", u"<INVALID>", u"<INVALID>", u"<INVALID>", 
                      u"<INVALID>", u"<INVALID>", u"<INVALID>", u"<INVALID>", 
                      u"<INVALID>", u"<INVALID>", u"<INVALID>", u"<INVALID>", 
                      u"<INVALID>", u"<INVALID>", u"<INVALID>", u"<INVALID>", 
                      u"<INVALID>", u"LETTER", u"NUMBER", u"NEWLINE" ]

    RULE_yuml = 0
    RULE_relationship = 1
    RULE_node = 2
    RULE_aggregation = 3
    RULE_composition = 4
    RULE_realization = 5
    RULE_generalization = 6
    RULE_text = 7

    ruleNames =  [ u"yuml", u"relationship", u"node", u"aggregation", u"composition", 
                   u"realization", u"generalization", u"text" ]

    EOF = Token.EOF
    T__0=1
    T__1=2
    T__2=3
    T__3=4
    T__4=5
    T__5=6
    T__6=7
    T__7=8
    T__8=9
    T__9=10
    T__10=11
    T__11=12
    T__12=13
    T__13=14
    T__14=15
    T__15=16
    LETTER=17
    NUMBER=18
    NEWLINE=19

    def __init__(self, input):
        super(srcYUML2graphVizParser, self).__init__(input)
        self.checkVersion("4.5.1")
        self._interp = ParserATNSimulator(self, self.atn, self.decisionsToDFA, self.sharedContextCache)
        self._predicates = None



    class YumlContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(srcYUML2graphVizParser.YumlContext, self).__init__(parent, invokingState)
            self.parser = parser

        def EOF(self):
            return self.getToken(srcYUML2graphVizParser.EOF, 0)

        def node(self, i=None):
            if i is None:
                return self.getTypedRuleContexts(srcYUML2graphVizParser.NodeContext)
            else:
                return self.getTypedRuleContext(srcYUML2graphVizParser.NodeContext,i)


        def relationship(self, i=None):
            if i is None:
                return self.getTypedRuleContexts(srcYUML2graphVizParser.RelationshipContext)
            else:
                return self.getTypedRuleContext(srcYUML2graphVizParser.RelationshipContext,i)


        def NEWLINE(self, i=None):
            if i is None:
                return self.getTokens(srcYUML2graphVizParser.NEWLINE)
            else:
                return self.getToken(srcYUML2graphVizParser.NEWLINE, i)

        def getRuleIndex(self):
            return srcYUML2graphVizParser.RULE_yuml

        def enterRule(self, listener):
            if hasattr(listener, "enterYuml"):
                listener.enterYuml(self)

        def exitRule(self, listener):
            if hasattr(listener, "exitYuml"):
                listener.exitYuml(self)




    def yuml(self):

        localctx = srcYUML2graphVizParser.YumlContext(self, self._ctx, self.state)
        self.enterRule(localctx, 0, self.RULE_yuml)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 19 
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while True:
                self.state = 19
                la_ = self._interp.adaptivePredict(self._input,0,self._ctx)
                if la_ == 1:
                    self.state = 16
                    self.node()
                    pass

                elif la_ == 2:
                    self.state = 17
                    self.relationship()
                    pass

                elif la_ == 3:
                    self.state = 18
                    self.match(srcYUML2graphVizParser.NEWLINE)
                    pass


                self.state = 21 
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                if not (_la==srcYUML2graphVizParser.T__0 or _la==srcYUML2graphVizParser.NEWLINE):
                    break

            self.state = 23
            self.match(srcYUML2graphVizParser.EOF)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class RelationshipContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(srcYUML2graphVizParser.RelationshipContext, self).__init__(parent, invokingState)
            self.parser = parser

        def node(self, i=None):
            if i is None:
                return self.getTypedRuleContexts(srcYUML2graphVizParser.NodeContext)
            else:
                return self.getTypedRuleContext(srcYUML2graphVizParser.NodeContext,i)


        def aggregation(self):
            return self.getTypedRuleContext(srcYUML2graphVizParser.AggregationContext,0)


        def composition(self):
            return self.getTypedRuleContext(srcYUML2graphVizParser.CompositionContext,0)


        def realization(self):
            return self.getTypedRuleContext(srcYUML2graphVizParser.RealizationContext,0)


        def generalization(self):
            return self.getTypedRuleContext(srcYUML2graphVizParser.GeneralizationContext,0)


        def getRuleIndex(self):
            return srcYUML2graphVizParser.RULE_relationship

        def enterRule(self, listener):
            if hasattr(listener, "enterRelationship"):
                listener.enterRelationship(self)

        def exitRule(self, listener):
            if hasattr(listener, "exitRelationship"):
                listener.exitRelationship(self)




    def relationship(self):

        localctx = srcYUML2graphVizParser.RelationshipContext(self, self._ctx, self.state)
        self.enterRule(localctx, 2, self.RULE_relationship)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 25
            self.node()
            self.state = 30
            la_ = self._interp.adaptivePredict(self._input,2,self._ctx)
            if la_ == 1:
                self.state = 26
                self.aggregation()
                pass

            elif la_ == 2:
                self.state = 27
                self.composition()
                pass

            elif la_ == 3:
                self.state = 28
                self.realization()
                pass

            elif la_ == 4:
                self.state = 29
                self.generalization()
                pass


            self.state = 32
            self.node()
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class NodeContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(srcYUML2graphVizParser.NodeContext, self).__init__(parent, invokingState)
            self.parser = parser

        def text(self):
            return self.getTypedRuleContext(srcYUML2graphVizParser.TextContext,0)


        def getRuleIndex(self):
            return srcYUML2graphVizParser.RULE_node

        def enterRule(self, listener):
            if hasattr(listener, "enterNode"):
                listener.enterNode(self)

        def exitRule(self, listener):
            if hasattr(listener, "exitNode"):
                listener.exitNode(self)




    def node(self):

        localctx = srcYUML2graphVizParser.NodeContext(self, self._ctx, self.state)
        self.enterRule(localctx, 4, self.RULE_node)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 34
            self.match(srcYUML2graphVizParser.T__0)
            self.state = 35
            self.text()
            self.state = 36
            self.match(srcYUML2graphVizParser.T__1)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class AggregationContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(srcYUML2graphVizParser.AggregationContext, self).__init__(parent, invokingState)
            self.parser = parser

        def text(self, i=None):
            if i is None:
                return self.getTypedRuleContexts(srcYUML2graphVizParser.TextContext)
            else:
                return self.getTypedRuleContext(srcYUML2graphVizParser.TextContext,i)


        def getRuleIndex(self):
            return srcYUML2graphVizParser.RULE_aggregation

        def enterRule(self, listener):
            if hasattr(listener, "enterAggregation"):
                listener.enterAggregation(self)

        def exitRule(self, listener):
            if hasattr(listener, "exitAggregation"):
                listener.exitAggregation(self)




    def aggregation(self):

        localctx = srcYUML2graphVizParser.AggregationContext(self, self._ctx, self.state)
        self.enterRule(localctx, 6, self.RULE_aggregation)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 38
            self.text()
            self.state = 39
            self.match(srcYUML2graphVizParser.T__2)
            self.state = 40
            self.text()
            self.state = 41
            self.match(srcYUML2graphVizParser.T__3)
            self.state = 42
            self.text()
            self.state = 43
            self.match(srcYUML2graphVizParser.T__4)
            self.state = 44
            self.text()
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class CompositionContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(srcYUML2graphVizParser.CompositionContext, self).__init__(parent, invokingState)
            self.parser = parser

        def text(self, i=None):
            if i is None:
                return self.getTypedRuleContexts(srcYUML2graphVizParser.TextContext)
            else:
                return self.getTypedRuleContext(srcYUML2graphVizParser.TextContext,i)


        def getRuleIndex(self):
            return srcYUML2graphVizParser.RULE_composition

        def enterRule(self, listener):
            if hasattr(listener, "enterComposition"):
                listener.enterComposition(self)

        def exitRule(self, listener):
            if hasattr(listener, "exitComposition"):
                listener.exitComposition(self)




    def composition(self):

        localctx = srcYUML2graphVizParser.CompositionContext(self, self._ctx, self.state)
        self.enterRule(localctx, 8, self.RULE_composition)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 46
            self.text()
            self.state = 47
            self.match(srcYUML2graphVizParser.T__2)
            self.state = 48
            self.text()
            self.state = 49
            self.match(srcYUML2graphVizParser.T__2)
            self.state = 50
            self.text()
            self.state = 51
            self.match(srcYUML2graphVizParser.T__3)
            self.state = 52
            self.text()
            self.state = 53
            self.match(srcYUML2graphVizParser.T__4)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class RealizationContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(srcYUML2graphVizParser.RealizationContext, self).__init__(parent, invokingState)
            self.parser = parser

        def text(self, i=None):
            if i is None:
                return self.getTypedRuleContexts(srcYUML2graphVizParser.TextContext)
            else:
                return self.getTypedRuleContext(srcYUML2graphVizParser.TextContext,i)


        def getRuleIndex(self):
            return srcYUML2graphVizParser.RULE_realization

        def enterRule(self, listener):
            if hasattr(listener, "enterRealization"):
                listener.enterRealization(self)

        def exitRule(self, listener):
            if hasattr(listener, "exitRealization"):
                listener.exitRealization(self)




    def realization(self):

        localctx = srcYUML2graphVizParser.RealizationContext(self, self._ctx, self.state)
        self.enterRule(localctx, 10, self.RULE_realization)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 55
            self.text()
            self.state = 56
            self.match(srcYUML2graphVizParser.T__5)
            self.state = 57
            self.text()
            self.state = 58
            self.match(srcYUML2graphVizParser.T__3)
            self.state = 59
            self.text()
            self.state = 60
            self.match(srcYUML2graphVizParser.T__6)
            self.state = 61
            self.text()
            self.state = 62
            self.match(srcYUML2graphVizParser.T__3)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class GeneralizationContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(srcYUML2graphVizParser.GeneralizationContext, self).__init__(parent, invokingState)
            self.parser = parser

        def text(self, i=None):
            if i is None:
                return self.getTypedRuleContexts(srcYUML2graphVizParser.TextContext)
            else:
                return self.getTypedRuleContext(srcYUML2graphVizParser.TextContext,i)


        def getRuleIndex(self):
            return srcYUML2graphVizParser.RULE_generalization

        def enterRule(self, listener):
            if hasattr(listener, "enterGeneralization"):
                listener.enterGeneralization(self)

        def exitRule(self, listener):
            if hasattr(listener, "exitGeneralization"):
                listener.exitGeneralization(self)




    def generalization(self):

        localctx = srcYUML2graphVizParser.GeneralizationContext(self, self._ctx, self.state)
        self.enterRule(localctx, 12, self.RULE_generalization)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 64
            self.text()
            self.state = 65
            self.match(srcYUML2graphVizParser.T__5)
            self.state = 66
            self.text()
            self.state = 67
            self.match(srcYUML2graphVizParser.T__3)
            self.state = 68
            self.text()
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx

    class TextContext(ParserRuleContext):

        def __init__(self, parser, parent=None, invokingState=-1):
            super(srcYUML2graphVizParser.TextContext, self).__init__(parent, invokingState)
            self.parser = parser

        def LETTER(self, i=None):
            if i is None:
                return self.getTokens(srcYUML2graphVizParser.LETTER)
            else:
                return self.getToken(srcYUML2graphVizParser.LETTER, i)

        def NUMBER(self, i=None):
            if i is None:
                return self.getTokens(srcYUML2graphVizParser.NUMBER)
            else:
                return self.getToken(srcYUML2graphVizParser.NUMBER, i)

        def getRuleIndex(self):
            return srcYUML2graphVizParser.RULE_text

        def enterRule(self, listener):
            if hasattr(listener, "enterText"):
                listener.enterText(self)

        def exitRule(self, listener):
            if hasattr(listener, "exitText"):
                listener.exitText(self)




    def text(self):

        localctx = srcYUML2graphVizParser.TextContext(self, self._ctx, self.state)
        self.enterRule(localctx, 14, self.RULE_text)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 86
            self._errHandler.sync(self)
            _alt = self._interp.adaptivePredict(self._input,4,self._ctx)
            while _alt!=2 and _alt!=ATN.INVALID_ALT_NUMBER:
                if _alt==1:
                    self.state = 84
                    token = self._input.LA(1)
                    if token in [srcYUML2graphVizParser.LETTER]:
                        self.state = 70
                        self.match(srcYUML2graphVizParser.LETTER)

                    elif token in [srcYUML2graphVizParser.NUMBER]:
                        self.state = 71
                        self.match(srcYUML2graphVizParser.NUMBER)

                    elif token in [srcYUML2graphVizParser.T__7]:
                        self.state = 72
                        self.match(srcYUML2graphVizParser.T__7)

                    elif token in [srcYUML2graphVizParser.T__3]:
                        self.state = 73
                        self.match(srcYUML2graphVizParser.T__3)

                    elif token in [srcYUML2graphVizParser.T__2]:
                        self.state = 74
                        self.match(srcYUML2graphVizParser.T__2)

                    elif token in [srcYUML2graphVizParser.T__8]:
                        self.state = 75
                        self.match(srcYUML2graphVizParser.T__8)

                    elif token in [srcYUML2graphVizParser.T__9]:
                        self.state = 76
                        self.match(srcYUML2graphVizParser.T__9)

                    elif token in [srcYUML2graphVizParser.T__4]:
                        self.state = 77
                        self.match(srcYUML2graphVizParser.T__4)

                    elif token in [srcYUML2graphVizParser.T__10]:
                        self.state = 78
                        self.match(srcYUML2graphVizParser.T__10)

                    elif token in [srcYUML2graphVizParser.T__11]:
                        self.state = 79
                        self.match(srcYUML2graphVizParser.T__11)

                    elif token in [srcYUML2graphVizParser.T__12]:
                        self.state = 80
                        self.match(srcYUML2graphVizParser.T__12)

                    elif token in [srcYUML2graphVizParser.T__13]:
                        self.state = 81
                        self.match(srcYUML2graphVizParser.T__13)

                    elif token in [srcYUML2graphVizParser.T__14]:
                        self.state = 82
                        self.match(srcYUML2graphVizParser.T__14)

                    elif token in [srcYUML2graphVizParser.T__15]:
                        self.state = 83
                        self.match(srcYUML2graphVizParser.T__15)

                    else:
                        raise NoViableAltException(self)
             
                self.state = 88
                self._errHandler.sync(self)
                _alt = self._interp.adaptivePredict(self._input,4,self._ctx)

        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx





