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
import srcYUML2graphVizParser

class Relay(srcYUML2graphVizListener):
	#======================================================Constructer
	def __init__(self, otpt):
		self.output = otpt
	#======================================================Yuml - (node | relationship | NEWLINE)+ EOF
	def enterYuml(self, ctx):
		print("yumlStart")
		##################

		print(dir(ctx))
		####################################
		# self.output.write(ctx.getText()) # THIS IS IT!!!
		####################################
	def exitYuml(self, ctx):
		print("yumlExit")
		#################

	#======================================================Relationship - (node (a|c|r|g) node)
	def enterRelationship(self, ctx):
		print("--relationStart")
		######################

		print("--" + ctx.getText())
		#print("--" + ctx.getRuleContext())

	def exitRelationship(self, ctx):
		print("--relationExit")
		#####################

	#======================================================Node - '[' text ']''
	def enterNode(self, ctx):
		#print("----nodeStart")
		##################

		print(ctx.getText())

	def exitNode(self, ctx):
		#print("----nodeExit")
		#################

	#======================================================Realization - the use of an 
	def enterRealization(self, ctx):                      #interface, the realization
		print("----realizationStart")                         #of an interface
		#########################

		temp = dir(ctx)
		#print(temp)

		print("----" + ctx.getText())

	def exitRealization(self, ctx):
		print("----realizationExit")
		########################

	#=====================================================Aggregation - child can exist 
	def enterAggregation(self, ctx):                     #independently of the parent
		print("----aggregationStart")
		#########################

	def exitAggregation(self, ctx):
		print("----aggregationExit")
		########################

	#=====================================================Composition - without parent child
	def enterComposition(self, ctx):                     #can not exist
		print("----compositionStart")
		#########################

	def exitComposition(self, ctx):
		print("----compositionExit")
		########################

	#====================================================Generalization - the generalization of
	def enterGeneralization(self, ctx):                 #children, children inherit the parent
		print("----generalizationStart")
		############################

	def exitGeneralization(self, ctx):
		print("----generalizationExit")
		############################

	#====================================================Text
	def enterText(self, ctx):
		print("------textStart")
		##################

		print("------" + ctx.getText())

	def exitText(self, ctx):
		print("------textExit")
		#################

	#====================================================
