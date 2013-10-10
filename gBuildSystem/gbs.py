#!/usr/bin/python3.1
import sys, os, fnmatch, random, re
from threading import Thread


helpTxt = r"""Makefile generator for Linux by Panos (GODlike) Christopoulos-Charitos
Usage: gbs.py [-i input] [-o output]
Options:
-h, -help, --help  Print this text
-i                 Input config file. Default: gen.cfg.py
-o                 Output makefile. Default: Makefile"""


#=======================================================================================================================
# GLOBAL VARS                                                                                                          =
#=======================================================================================================================
sourceFiles = []
sourcePaths = []
precompiledHeaders = []
executableName = "unamed-project"
compiler = ""
linker = ""
compilerFlags = ""
precompiledHeadersFlags = ""
linkerFlags = ""
sourceFileRegExpr = r"^.*\.(c\+\+|cpp|cc|cxx|C|c)$"
includePaths = []



#=======================================================================================================================
# __walkDir                                                                                                            =
#=======================================================================================================================
def __walkDir(path):
	""" Recursive function """
	paths = []
	for f in os.listdir(path):
		newPath = os.path.join(path, f)
		if os.path.isdir(newPath) and not f.startswith("."):
			paths.append(newPath)
			paths.extend(list(__walkDir(newPath)))
	return paths

#=======================================================================================================================
# walkDir                                                                                                              =
#=======================================================================================================================
def walkDir(path, includeThis = False):
	paths = __walkDir(path)
	if includeThis:
		paths.append(path)
	return paths


#=======================================================================================================================
# getCommandOutput                                                                                                     =
#=======================================================================================================================
def getCommandOutput(command):
	child = os.popen(command)
	data = child.read()
	err = child.close()
	if err:
		print("getCommandOutput failed:\n" + command)
		exit(0)
	return data


#=======================================================================================================================
# Threads                                                                                                              =
#=======================================================================================================================
threadList = []

class TargetThread(Thread):
	def __init__(self, tid, range):
		Thread.__init__(self)
		self.tid = tid
		self.range = range
		self.outStr = ""
		
	def run(self):
		for i in self.range:
			print(".", end = "")
			sys.stdout.flush()
			source_file = sourceFileInfos[i]
			self.outStr += getCommandOutput(compiler + " -MM " + compilerFlags + " " + source_file.inFile + " -MT " + source_file.outFile)
			self.outStr += "\t@echo Compiling " + source_file.inFile + "...\n"
			self.outStr += "\t@$(CXX) $(INCPATH) $(CFLAGS) " + source_file.inFile + " -o " + source_file.outFile + "\n\n"
			#print("Im thread %d and I will make depends for %s" %(self.tid, source_file.fname))
			#print("Im thread %d and my i is %d" %(self.tid, i))


#=======================================================================================================================
# SourceFile                                                                                                           =
#=======================================================================================================================
class SourceFile:
	def __init__(self):	
		self.inFile = ""  # eg .cpp or .c
		self.outFile = "" # eg .o or .gch


#=======================================================================================================================
# main                                                                                                                 =
#=======================================================================================================================

# Read the arguments
inputCfgFile = ""
outputMakefile = ""

i = 0
while 1:
	i = i+1
	if i>=len(sys.argv): break

	arg = sys.argv[i]
	if arg == "-h" or arg == "-help" or arg == "--help":
		print(helpTxt)
		exit(0)
	elif arg == "-i":
		inputCfgFile = sys.argv[i+1]
		i = i+1
	elif arg == "-o":
		outputMakefile = sys.argv[i+1]
		i = i+1
	else:
		print("Unrecognized argument " + arg)
	

if outputMakefile == "":
	outputMakefile = "Makefile"
	
if inputCfgFile == "":
	inputCfgFile = "gen.cfg.py"


# Check if cfg exists
if not os.path.exists(inputCfgFile):
	print("File " + inputCfgFile + " doesn't exist")
	exit(0)


# compile the cfg
source = ""
f = open(inputCfgFile, "r")
for line in f.readlines():
	source += line
	
exec(compile(source, inputCfgFile, "exec"))


# find the source files
regexpr = re.compile(sourceFileRegExpr)
allSourceFiles = list(sourceFiles)
for sourceDir in sourcePaths:
	files = os.listdir(sourceDir)
	for f in files:
		if not regexpr.match(f):
			continue
		
		allSourceFiles.append(os.path.join(sourceDir, f))


sourceFileInfos = []
for file_ in allSourceFiles:
	if not os.path.exists(file_):
		raise RuntimeError("File \"" + file_ + "\" doesnt exist")

	sfile = SourceFile()
	
	sfile.inFile = file_
	(fname_wo_ext, ext) = os.path.splitext(os.path.basename(file_))
	sfile.outFile = fname_wo_ext + ".o"
	
	# search all the source files and resolve conflicts in .o
	for sfile1 in sourceFileInfos:
		if sfile1.outFile == sfile.outFile:
			print("There is a naming conflict between \"" + sfile1.inFile + "\" and \"" + sfile.inFile + "\" but dont worry.")
			random.seed()
			sfile.outFile = str(random.randint(1,99)) + "." + sfile.outFile;

	sourceFileInfos.append(sfile)
	

# now the precompiled headers
phFiles = []
for header in precompiledHeaders:
	sFile = SourceFile()
	(fnameWoExt, ext) = os.path.splitext(header)
	sFile.inFile = header
	sFile.outFile = os.path.basename(fnameWoExt) + ".h.gch "
	phFiles.append(sFile)


# build the string
command = ""
for arg in sys.argv:
	command += arg + " "
masterStr = "# Generated by gBuildSystem, command: " + command + "\n\n"

masterStr += "CXX = " + compiler + "\n"
masterStr += "CFLAGS = " + compilerFlags + "\n"
masterStr += "PHFLAGS = " + precompiledHeadersFlags + "\n"
if linker == "":
	linker = compiler
masterStr += "LINKER = " + linker + "\n"	
masterStr += "LFLAGS = " + linkerFlags + "\n"
masterStr += "EXECUTABLE = " + executableName + "\n"

masterStr += "INCPATH = "
for path in includePaths:
	masterStr += "-I" + path + " "
	compilerFlags += " -I" + path + " "
masterStr += "\n"

masterStr += "SOURCES = "
for source_file in sourceFileInfos:
	masterStr += source_file.inFile + " "
masterStr += "\n"

masterStr += "OBJECTS = "
for source_file in sourceFileInfos:
	masterStr += source_file.outFile + " "
masterStr += "\n"

masterStr += "PRECOMPILED_HEADERS = "
for header in phFiles:
	masterStr += header.outFile
masterStr += "\n\n"

masterStr += "all: $(PRECOMPILED_HEADERS) $(SOURCES) $(EXECUTABLE)\n\n"

masterStr += "$(EXECUTABLE): $(OBJECTS)\n"
masterStr += "\t@echo Linking...\n"
masterStr += "\t@$(LINKER) $(OBJECTS) $(LFLAGS) -o $(EXECUTABLE)\n"
masterStr += "\t@echo All Done!\n\n"


for header in phFiles:  
	dependStr = getCommandOutput(compiler + " -MM " + compilerFlags + " " + header.inFile + " -MT " + header.outFile)
	masterStr += dependStr
	masterStr += "\t@echo Pre-compiling header " + header.inFile + "...\n"
	masterStr += "\t@$(CXX) $(INCPATH) $(PHFLAGS) " + header.inFile + " -x c++-header " + header.outFile + "\n\n"


# write source file target
threadsNum = os.sysconf('SC_NPROCESSORS_ONLN') + 1
num = len(sourceFileInfos);
if num < threadsNum:
	threadsNum = num
print("I will invoke %d threads to make the dependencies" % threadsNum, end = "")
itemsPerThread = num // threadsNum;

for i in range(0, threadsNum):
	begin = i*itemsPerThread
	if i == threadsNum-1:
		end = num
	else:
		end = begin + itemsPerThread	
	thread = TargetThread(i, range(int(begin), int(end)))
	thread.start()
	threadList.append(thread)

for thread in threadList:
	thread.join()

for thread in threadList:
	masterStr += thread.outStr
	
print()


masterStr += "clean:\n"
masterStr += "\trm -f *.o\n"
masterStr += "\trm -f *.gch\n"
masterStr += "\trm -f *~\n"
masterStr += "\trm -f $(EXECUTABLE)\n\n"


masterStr += "regen:\n"
masterStr += "\t@echo Re-generating makefile...\n"
masterStr += "\t" + command + "\n"


# write file
f = open(outputMakefile, "w")
f.write(masterStr)
print("File \"" + outputMakefile + "\" created!")
