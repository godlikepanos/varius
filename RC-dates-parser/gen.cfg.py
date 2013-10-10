sourcePaths = ["."]

includePaths = []
includePaths.append("./")
includePaths.extend(list(sourcePaths))

precompiledHeaders = []

executableName = "rc-dates-parser"

compiler = "g++"

compilerFlags = "-c -pedantic-errors -pedantic -ansi -Wall -Wextra -W -pipe -O0 -g3 -pg"

linkerFlags = ""
