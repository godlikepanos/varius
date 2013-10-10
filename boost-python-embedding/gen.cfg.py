sourcePaths = ["."]

includePaths = [".", "/usr/include/python2.6"]


executableName = "test"

compiler = "g++"

compilerFlags = "-c -pedantic-errors -pedantic -ansi -Wall -Wextra -W -Wno-long-long -pipe -O0 -g3 -pg -fsingle-precision-constant"

linkerFlags = "-rdynamic -Wl,-Bstatic -Wl,-Bdynamic -lpython2.6 -lboost_python"
