#!/usr/bin/python

import os
import optparse
import xml.etree.ElementTree as et

# Globals
identation_level = 0
out_file = None

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

def get_base_fname(path):
	""" From path/to/a/file.ext return the "file" """
	return os.path.splitext(os.path.basename(path))[0]

def write_glue(txt):
	""" Write glue code to the output """
	global out_file
	global identation_level
	out_file.write("%s%s\n" % ("\t" * identation_level, txt))

def ident(number):
	""" Increase or recrease identation for the write_glue """
	global identation_level
	identation_level += number

def type_is_number(type):
	""" Check if a type is number """

	numbers = ["U8", "U16", "U32", "U64", "I8", "I16", "I32", "I64", \
		"U", "I", "PtrSize", "Bool", "Bool8", "F32", "F64" \
		"int", "unsigned", "unsigned int", "short", "unsigned short", "uint"]

	it_is = False
	for num in numbers:
		if num == type:
			it_is = True
			break

	return it_is

def parse_type_decl(arg_txt):
	""" Parse an arg text """

	tokens = arg_txt.split(" ")
	tokens_size = len(tokens)

	type = tokens[tokens_size - 1]
	type_len = len(type)
	is_ptr = False
	is_ref = False
	if type[type_len - 1] == "&":
		is_ref = True
	elif type[type_len - 1] == "*":
		is_ptr = True

	if is_ref or is_ptr:
		type = type[:-1]

	is_const = False
	if tokens[0] == "const":
		is_const = True

	return (type, is_ref, is_ptr, is_const)

def gen_arg(arg_txt, stack_index):
	""" XXX """

	(type, is_ref, is_ptr, is_const) = parse_type_decl(arg_txt)

	if type_is_number(type):
		write_glue("%s arg%d(luaL_checknumber(l, %d));" \
			% (type, stack_index, stack_index))
	elif type == "char" or type == "CString":
		write_glue("const char* arg%d(luaL_checkstring(l, %d));" \
			% (stack_index, stack_index))
	else:
		write_glue("voidp = luaL_checkudata(l, %d, className);" \
			% stack_index)
		write_glue("%s* iarg%d = reinterpret_cast<%s*>(ud->m_data);" \
			% (type, stack_index, type))

		deref = ""
		if is_ref:
			deref = "*"

		write_glue("%s arg%d(%siarg%d);" \
			% (arg_txt, stack_index, deref, stack_index))

def gen_ret(type_txt):
	(type, is_ref, is_ptr, is_const) = parse_type_decl(type_txt)

	if type_is_number(type):
		write_glue("lua_pushnumber(l, ret);")
	elif type == "char" or type == "CString":
		write_glue("lua_pushstring(l, ret);")
	else:
	 	write_glue("voidp = lua_newuserdata(l, sizeof(UserData));")
		write_glue("ud = reinterpret_cast<UserData*>(voidp);")
		write_glue("luaL_setmetatable(l, \"%s\");" % type)

		if is_ptr:
			write_glue("ud->m_data = reinterpret_cast<%s*>(ret)" % type)
			write_glue("ud->m_gc = false;")
		elif is_ref:
			write_glue("ud->m_data = reinterpret_cast<%s>(&ret)" % type)
			write_glue("ud->m_gc = false;")
		else:
			"""write_glue("LuaBinder* binder = "\
				"reinterpret_cast<LuaBinder*>(lua_getuserdata(l));")
			write_glue("ANKI_ASSERT(binder);")
			write_glue("LuaBinder::Allocator<U8> alloc = " \
				"binder->_getAllocator();")
			write_glue("d->m_data = " \
				"alloc.newInstance<%s>(std::move(ret));" % type)"""

			write_glue("d->m_data = " \
				"new %s(std::move(ret));" % type)

			write_glue("if(d->m_data == nullptr)")
			write_glue("{")
			ident(1)
			write_glue("lua_pushstring(\"Out of memory\");")
			write_glue("lua_error(l);")
			ident(-1)
			write_glue("}")

			write_glue("ud->m_gc = true;")

	write_glue("")

def gen_method(class_name, meth_el):
	""" Handle a method """

	meth_name = meth_el.find("name").text

	write_glue("// Method %s::%s" % (class_name, meth_name))
	write_glue("static int wrap%s%s(lua_State* l)" % (class_name, meth_name))
	write_glue("{")
	ident(1)
	write_glue("UserData* ud;")
	write_glue("(void)ud;")
	write_glue("void* voidp;")
	write_glue("(void)voidp;")
	write_glue("")
	write_glue("voidp = luaL_checkudata(l, %d, classname%s);" % (1, class_name))
	write_glue("ud = reinterpret_cast<UserData*>(voidp);")
	write_glue("%s* self = reinterpret_cast<%s*>(ud->m_data);" \
		% (class_name, class_name))
	write_glue("")

	args_el = meth_el.find("args")

	args_str = ""
	if args_el is not None:
		stack_index = 1
		arg_index = 0
		for arg_el in args_el.iter("arg"):
			gen_arg(arg_el.text, stack_index)
			args_str += "arg%d, " % stack_index
			write_glue("")
			stack_index += 1

		if len(args_str) > 0:
			args_str = args_str[:-2]

	# Return value
	ret_txt = None
	ret_el = meth_el.find("return")
	if ret_el is not None:
		ret_txt = ret_el.text

	# Method call
	if ret_txt is None:
		write_glue("self->%s(%s);" % (meth_name, args_str))
	else:
		write_glue("%s ret = self->%s(%s);" % (ret_txt, meth_name, args_str))

	write_glue("")
	if ret_txt is None:
		write_glue("return 0;")
	else:
		gen_ret(ret_txt)
		write_glue("return 1;")

	ident(-1)
	write_glue("}")
	write_glue("")

def gen_constructor(constr_el, class_name):
	""" Handle constructor """
	
	write_glue("// Constructor for %s" % (class_name))
	write_glue("static int wrap%sCtor(lua_State* l)" % class_name)
	write_glue("{")
	ident(1)

	write_glue("void* ptr = lua_newuserdata(l, sizeof(UserData));")
	write_glue("luaL_setmetatable(l, classname%s);" % class_name)
	write_glue("")

	write_glue("%s* inst = new %s();" % (class_name, class_name))
	write_glue("if(inst == nullptr)")
	write_glue("{")
	ident(1)
	write_glue("lua_pushstring(l, \"Out of memory\");")
	write_glue("lua_error(l);")
	ident(-1)
	write_glue("}")
	write_glue("")

	write_glue("UserData* ud = reinterpret_cast<UserData*>(ptr);")
	write_glue("ud->m_data = inst;")
	write_glue("ud->m_gc = true;")
	write_glue("")

	write_glue("return 1;")
	
	ident(-1)
	write_glue("}")
	write_glue("")

def gen_class(class_el):
	class_name = class_el.find("name").text
	class_alias = class_el.find("alias")
	if class_alias is not None:
		class_alias = class_alias.text

	write_glue("static const char* classname%s = \"%s\";" \
		% (class_name, class_name))
	write_glue("")

	# Constructor
	has_constructor = False
	constructor_el = class_el.find("constructor")
	if constructor_el is not None:
		gen_constructor(constructor_el, class_name)
		has_constructor = True

	# Methods LUA C functions
	meth_names = []
	meths_el = class_el.find("methods")
	if meths_el is not None:
		for meth_el in meths_el.iter("method"):
			meth_names.append(meth_el.find("name").text)
			gen_method(class_name, meth_el)

	# Write class header
	write_glue("static void wrap%s(lua_State* l)" % class_name)
	write_glue("{")
	ident(1)
	write_glue("createClass(l, \"%s\");" % class_name)

	# Register constructor
	if has_constructor:
		write_glue("pushLuaCFuncStaticMethod(l, classname%s, \"new\", " \
			"wrap%sCtor);" % (class_name, class_name))

	# Register methods
	if len(meth_names) > 0:
		for meth_name in meth_names:
			write_glue("pushLuaCFuncMethod(l, \"%s\", wrap%s%s);" \
				% (meth_name, class_name, meth_name))

	write_glue("lua_settop(l, 0);")

	ident(-1)
	write_glue("}")
	write_glue("")

def main():
	global out_file
	filenames = parse_commandline()

	for filename in filenames:
		out_filename = get_base_fname(filename) + "Autogen.h"
		out_file = open(out_filename, "w")

		tree = et.parse(filename)
		root = tree.getroot()

		class_names = []
		for cls in root.iter("classes"):
			for cl in cls.iter("class"):
				gen_class(cl)
				class_names.append(cl.find("name").text)

		write_glue("void wrapModule%s(lua_State* l)" % get_base_fname(filename))
		write_glue("{")
		ident(1)
		for class_name in class_names:
			write_glue("wrap%s(l);" % class_name)
		ident(-1)
		write_glue("}")
		write_glue("")

		out_file.close()

if __name__ == "__main__":
	main()


