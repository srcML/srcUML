#!/usr/bin/python2

import sys, os, time
import subprocess as sp

def main(argv):

	if argv == 1:
		print "Usage: ./autopy.py [input_file]"

	if argv > 1:
		sp.call(['srcml', argv[1], '-o', 'class_file_test.hpp.xml'])
		print "[=	Finished srcml..."
		sp.call(['srcyuml', 'class_file_test.hpp.xml', 'class_file_test.hpp.yuml'])
		print "[=	Finished srcyuml..."
		sp.call(['./../srcYUML2graphViz.py', 'class_file_test.hpp.yuml', 'class_file_test.outxt'])
		print "[=	Finished conversion..."
		sp.call(['dot', '-Tpng', 'class_file_test.outxt', '-o', 'pic.png'])
		print "[=	Finished picture..."
		sp.call(['rm', 'class_file_test.hpp.xml', 'class_file_test.hpp.yuml', 'class_file_test.outxt'])
		print "[=	Removed extra files..."
		print "[=	Completed!"

if __name__ == '__main__':
	main(sys.argv)
