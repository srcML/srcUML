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

from antlr4 import*
from srcYUML2graphVizListener import srcYUML2graphVizListener
from kitchen.text.converters import to_bytes
import srcYUML2graphVizParser
import sys

class Relay(srcYUML2graphVizListener):
	#======================================================Constructer
	def __init__(self, otpt):
		self.output = otpt
		self.label2class = {}
		self.labeler = 0 #dictionary, thing["what"] = 10, if "what" in thing
		self.inMethod = False
		self.inRelation = False
		self.inReal = False
		self.inComp = False
		self.inAgg = False
		self.inGen = False
		self.secondRelationPart = False

	#======================================================Yuml - (node | relationship | NEWLINE)+ EOF
	def enterYuml(self, ctx):
		print("----------------yumlStart")
		print(ctx.getText())
		##################

		#print(dir(ctx))
		####################################
		# self.output.write( to_bytes(ctx.getText()) ) # THIS IS IT!!!
		####################################
	def exitYuml(self, ctx):
		print("----------------yumlExit")
		#################
		self.output.write("}\n")

	#======================================================Class
	def enterClassDef(self, ctx):
		print("------------classStart")
		################## 

	def exitClassDef(self, ctx):
		print("------------classExit")
		###################
		if self.inRelation == False :
			self.output.write("}\"]\n")

	#======================================================Relationship - (node (a|c|r|g) node)
	def enterRelationship(self, ctx):
		print("------------relationStart")
		######################
		self.inRelation = True

		#print("--------" + ctx.getText())
		#print("--" + ctx.getRuleContext())

	def exitRelationship(self, ctx):
		print("------------relationExit")
		#####################
		self.inRelation = False
		self.inReal = False
		self.inComp = False
		self.inAgg = False
		self.inGen = False

	#======================================================Realization - the use of an 
	def enterRealization(self, ctx):                      #interface, the realization
		print("--------realizationStart")                         #of an interface
		#########################
		self.inReal = True
		self.output.write("->")
		print("----" + ctx.getText())

	def exitRealization(self, ctx):
		print("--------realizationExit")
		########################
		self.secondRelationPart = True

	#=====================================================Aggregation - child can exist 
	def enterAggregation(self, ctx):                     #independently of the parent
		print("--------aggregationStart")
		#########################
		self.inAgg = True
		self.output.write("->")

	def exitAggregation(self, ctx):
		print("--------aggregationExit")
		########################
		self.secondRelationPart = True

	#=====================================================Composition - without parent child
	def enterComposition(self, ctx):                     #can not exist
		print("--------compositionStart")
		#########################
		self.inComp = True
		self.output.write("->")

	def exitComposition(self, ctx):
		print("--------compositionExit")
		########################
		self.secondRelationPart = True

	#====================================================Generalization - the generalization of
	def enterGeneralization(self, ctx):                 #children, children inherit the parent
		print("--------generalizationStart")
		############################
		self.inGen = True
		self.output.write("->")

	def exitGeneralization(self, ctx):
		print("--------generalizationExit")
		############################
		self.secondRelationPart = True

	#====================================================Text
	def enterText(self, ctx):
		print("----textStart")
		##################

		print(ctx.getText())

	def exitText(self, ctx):
		print("----textExit")
		#################

	#====================================================relationText
	def enterRelationText(self, ctx):
		print("----reltextStart")
		##################


	def exitRelationText(self, ctx):
		print("----reltextExit")
		#################

	#====================================================vmText
	def enterVmText(self, ctx):
		print("----vmTextStart")
		##################
		print(ctx.getText())
		if self.inMethod == True :
			self.output.write( "|" + to_bytes(ctx.getText().replace(";", "\\" + "n")) )
			return
		self.output.write( to_bytes(ctx.getText().replace(";", "\\" + "n")) )

	def exitVmText(self, ctx):
		print("----vmTextExit")
		##################

	#====================================================ClassID
	def enterClassID(self, ctx):
		print("--------IDStart")
		print(ctx.getText())
		##################
		if self.inRelation == True:
			self.output.write(to_bytes(self.label2class[ctx.getText()]))
			if self.secondRelationPart == True:
				if self.inReal == True:
					self.output.write("[arrowhead=\"none\", style=\"dashed\"]\n")
				if self.inGen == True:
					self.output.write("[arrowhead=\"none\"]\n")
				if self.inAgg == True:
					self.output.write("[arrowhead=\"none\", arrowtail=\"odiamond\"]\n")
				if self.inComp == True:
					self.output.write("[arrowhead=\"vee\", arrowtail=\"diamond\"]\n")

				self.secondRelationPart == False
			return
		if ctx.getText() in self.label2class.keys():
			return
		toPlace = ctx.getText()
		if toPlace.find(';') > -1:
			indx = toPlace.find(';')
			toPlace = " " + unichr(171) + "interface" + unichr(187) + "\\" + "n" + toPlace[indx + 1:]
		self.output.write(to_bytes(str(self.labeler) + "[label = \"{" + toPlace + "|"))
		self.label2class[ctx.getText()] = self.labeler # maps the number to the classID for later recall
		self.labeler += 1

	def exitClassID(self, ctx):
		print("--------IDExit")
		###################

	#=====================================================
	def enterMethods(self, ctx):                     
		print("--------methodsStart")
		#########################
		self.inMethod = True

	def exitMethods(self, ctx):
		print("--------methodsExit")
		########################
		self.inMethod = False

	#=====================================================
	def enterVariables(self, ctx):                     
		print("--------variablesStart")
		#########################

	def exitVariables(self, ctx):
		print("--------variablesExit")
		########################
