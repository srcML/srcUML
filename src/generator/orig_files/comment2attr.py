#!/usr/bin/python3

from xml.sax import *
import sys

class myHandler(ContentHandler):

	cur_element = None

	def startDocument(self):
		print("Doc Started\n")
		
	def startElement(self, name, attrs):
		if name == "name":
			self.cur_element = "name"
			print( attrs.getNames()[0] + '\t' + attrs.getValue( attrs.getNames()[0] ) )
		else:
			self.cur_element = None

	def characters(self, content):
		if self.cur_element == "comment" && content == "String":
			



xml_file = open(sys.argv[1])
handler = myHandler()
#handler.startDocument()

parse(xml_file, handler)
