#!/usr/bin/python

import optparse
import xml.etree.ElementTree as et

def parse_commandline():
	""" Parse the command line arguments """

	parser = optparse.OptionParser(usage = "usage: %prog [options]", \
		description = "Create LUA bindings using XML")	

	parser.add_option("-i", "--input", dest = "inp",
			type = "string", help = "specify the XML files to parse. " \
			"Seperate with :")

	(options, args) = parser.parse_args()

	if not options.inp:
		parser.error("argument is missing")

	return options.inp.split(":")


def gen_method(meth_el):
	args_el = meth_el.find("args")

	if args_el is not None:
		for arg_el in args_el.iter("arg"):
			print("arg %s" % arg_el.text)

def gen_class(class_el):
	class_name = class_el.find("name").text
	
	# Constructor
	constructor_el = class_el.find("constructor")
	if constructor_el is not None:
		print("has constructor")

	# Methods
	meths_el = class_el.find("methods")
	if meths_el is not None:
		for meth_el in meths_el.iter("method"):
			gen_method(meth_el)

def main():
	filenames = parse_commandline()

	for filename in filenames:
		tree = et.parse(filename)
		root = tree.getroot()

		for cls in root.iter("classes"):
			for cl in cls.iter("class"):
				gen_class(cl)

if __name__ == "__main__":
	main()


