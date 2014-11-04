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

def wglue(txt):
	""" Write glue code to the output """
	global out_file
	global identation_level
	out_file.write("%s%s\n" % ("\t" * identation_level, txt))

def ident(number):
	""" Increase or recrease identation for the wglue """
	global identation_level
	identation_level += number

def type_is_number(type):
	""" Check if a type is number """

	numbers = ["U8", "U16", "U32", "U64", "I8", "I16", "I32", "I64", \
		"U", "I", "PtrSize", "Bool", "Bool8", "F32", "F64", \
		"int", "unsigned", "unsigned int", "short", "unsigned short", "uint", \
		"float", "double"]
		
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
		wglue("%s arg%d(luaL_checknumber(l, %d));" \
			% (type, stack_index, stack_index))
	elif type == "char" or type == "CString":
		wglue("const char* arg%d(luaL_checkstring(l, %d));" \
			% (stack_index, stack_index))
	else:
		wglue("voidp = luaL_checkudata(l, %d, className);" \
			% stack_index)
		wglue("%s* iarg%d = reinterpret_cast<%s*>(ud->m_data);" \
			% (type, stack_index, type))

		deref = ""
		if is_ref:
			deref = "*"

		wglue("%s arg%d(%siarg%d);" \
			% (arg_txt, stack_index, deref, stack_index))

def gen_ret(type_txt):
	(type, is_ref, is_ptr, is_const) = parse_type_decl(type_txt)

	if type_is_number(type):
		wglue("lua_pushnumber(l, ret);")
	elif type == "char" or type == "CString":
		wglue("lua_pushstring(l, ret);")
	else:
	 	wglue("voidp = lua_newuserdata(l, sizeof(UserData));")
		wglue("ud = reinterpret_cast<UserData*>(voidp);")
		wglue("luaL_setmetatable(l, \"%s\");" % type)

		if is_ptr:
			wglue("ud->m_data = reinterpret_cast<%s*>(ret)" % type)
			wglue("ud->m_gc = false;")
		elif is_ref:
			wglue("ud->m_data = reinterpret_cast<%s>(&ret)" % type)
			wglue("ud->m_gc = false;")
		else:
			wglue("ud->m_data = luaAlloc(l, sizeof(%s));" % type)
			wglue("::new(ud->m_data) %s(std::move(ret));" % type)

			wglue("if(d->m_data == nullptr)")
			wglue("{")
			ident(1)
			wglue("lua_pushstring(\"Out of memory\");")
			wglue("lua_error(l);")
			ident(-1)
			wglue("}")

			wglue("ud->m_gc = true;")

	wglue("")

def args(args_el, stack_index):
	""" Write the pop code for argument parsing and return the arg list """

	if args_el is None:
		return ""

	# Do the work
	args_str = ""
	arg_index = 0
	for arg_el in args_el.iter("arg"):
		gen_arg(arg_el.text, stack_index)
		args_str += "arg%d, " % stack_index
		wglue("")
		stack_index += 1

		if len(args_str) > 0:
			args_str = args_str[:-2]

	return args_str

def check_args(args_el, bias):
	if args_el is None:
		wglue("checkArgsCount(l, %d);" % bias)
	else:
		count = 0
		for arg_el in args_el.iter("arg"):
			count += 1

		wglue("checkArgsCount(l, %d);" % (bias + count))

	wglue("")

def method(class_name, meth_el):
	""" Handle a method """

	meth_name = meth_el.get("name")
	meth_alias = meth_el.get("alias")
	if meth_alias is None:
		meth_alias = meth_name

	wglue("// Method %s::%s" % (class_name, meth_name))
	wglue("static int wrap%s%s(lua_State* l)" % (class_name, meth_alias))
	wglue("{")
	ident(1)
	wglue("UserData* ud;")
	wglue("(void)ud;")
	wglue("void* voidp;")
	wglue("(void)voidp;")
	wglue("")

	check_args(meth_el.find("args"), 1)

	wglue("voidp = luaL_checkudata(l, %d, classname%s);" % (1, class_name))
	wglue("ud = reinterpret_cast<UserData*>(voidp);")
	wglue("%s* self = reinterpret_cast<%s*>(ud->m_data);" \
		% (class_name, class_name))
	wglue("")

	args_str = args(meth_el.find("args"), 2)

	# Return value
	ret_txt = None
	ret_el = meth_el.find("return")
	if ret_el is not None:
		ret_txt = ret_el.text

	# Method call
	if ret_txt is None:
		wglue("self->%s(%s);" % (meth_name, args_str))
	else:
		wglue("%s ret = self->%s(%s);" % (ret_txt, meth_name, args_str))

	wglue("")
	if ret_txt is None:
		wglue("return 0;")
	else:
		gen_ret(ret_txt)
		wglue("return 1;")

	ident(-1)
	wglue("}")
	wglue("")

def constructor(constr_el, class_name):
	""" Handle constructor """
	
	wglue("// Constructor for %s" % (class_name))
	wglue("static int wrap%sCtor(lua_State* l)" % class_name)
	wglue("{")
	ident(1)
	wglue("UserData* ud;")
	wglue("(void)ud;")
	wglue("void* voidp;")
	wglue("(void)voidp;")
	wglue("")

	check_args(constr_el.find("args"), 0)

	# Args
	args_str = args(constr_el.find("args"), 1)

	# Create new userdata
	wglue("voidp = lua_newuserdata(l, sizeof(UserData));")
	wglue("luaL_setmetatable(l, classname%s);" % class_name)
	wglue("")

	wglue("void* inst = luaAlloc(l, sizeof(%s));" % class_name)
	wglue("if(inst == nullptr)")
	wglue("{")
	ident(1)
	wglue("lua_pushstring(l, \"Out of memory\");")
	wglue("lua_error(l);")
	ident(-1)
	wglue("}")
	wglue("")
	wglue("::new(inst) %s(%s);" % (class_name, args_str))
	wglue("")

	wglue("ud = reinterpret_cast<UserData*>(voidp);")
	wglue("ud->m_data = inst;")
	wglue("ud->m_gc = true;")
	wglue("")

	wglue("return 1;")
	
	ident(-1)
	wglue("}")
	wglue("")

def destructor(class_name):
	""" Create a destroctor """

	wglue("// Destructor for %s" % (class_name))
	wglue("static int wrap%sDtor(lua_State* l)" % class_name)
	wglue("{")
	ident(1)

	wglue("checkArgsCount(l, 1);")
	wglue("void* voidp = luaL_checkudata(l, 1, classname%s);" % class_name)
	wglue("UserData* ud = reinterpret_cast<UserData*>(voidp);")

	wglue("if(ud->m_gc)")
	wglue("{")
	ident(1)
	wglue("%s* inst = reinterpret_cast<%s*>(ud->m_data);" \
		% (class_name, class_name))
	wglue("inst->~%s();" % class_name)
	wglue("luaFree(l, inst);")
	ident(-1)
	wglue("}")
	wglue("")

	wglue("return 0;")

	ident(-1)
	wglue("}")
	wglue("")

def class_(class_el):
	""" Create a class """

	class_name = class_el.get("name")

	wglue("static const char* classname%s = \"%s\";" \
		% (class_name, class_name))
	wglue("")

	# Constructor
	has_constructor = False
	constructor_el = class_el.find("constructor")
	if constructor_el is not None:
		constructor(constructor_el, class_name)
		has_constructor = True

	# Destructor
	if has_constructor:
		destructor(class_name)

	# Methods LUA C functions
	meth_names_aliases = []
	meths_el = class_el.find("methods")
	if meths_el is not None:
		for meth_el in meths_el.iter("method"):
			method(class_name, meth_el)

			meth_name = meth_el.get("name")
			meth_alias = meth_el.get("alias")
			if meth_alias is None:
				meth_alias = meth_name
			meth_names_aliases.append([meth_name, meth_alias])

	# Write class header
	wglue("static void wrap%s(lua_State* l)" % class_name)
	wglue("{")
	ident(1)
	wglue("createClass(l, classname%s);" % class_name)

	# Register constructor
	if has_constructor:
		wglue("pushLuaCFuncStaticMethod(l, classname%s, \"new\", " \
			"wrap%sCtor);" % (class_name, class_name))

	# Register destructor
	if has_constructor:
		wglue("pushLuaCFuncMethod(l, \"__gc\", wrap%sDtor);" % class_name)

	# Register methods
	if len(meth_names_aliases) > 0:
		for meth_name_alias in meth_names_aliases:
			meth_alias = meth_name_alias[1]
			wglue("pushLuaCFuncMethod(l, \"%s\", wrap%s%s);" \
				% (meth_alias, class_name, meth_alias))

	wglue("lua_settop(l, 0);")

	ident(-1)
	wglue("}")
	wglue("")

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
				class_(cl)
				class_names.append(cl.get("name"))

		wglue("void wrapModule%s(lua_State* l)" % get_base_fname(filename))
		wglue("{")
		ident(1)
		for class_name in class_names:
			wglue("wrap%s(l);" % class_name)
		ident(-1)
		wglue("}")
		wglue("")

		out_file.close()

if __name__ == "__main__":
	main()


