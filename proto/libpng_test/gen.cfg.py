sourcePaths = ["."]

includePaths = []
includePaths.append("./")
includePaths.extend(list(sourcePaths))

precompiledHeaders = []

executableName = "libpng-test"

compiler = "g++"

compilerFlags = "-c -pedantic-errors -pedantic -ansi -Wall -Wextra -W -pipe -O0 -g3 -pg"

linkerFlags = "-lpng -lGL -lGLU -lglut"
