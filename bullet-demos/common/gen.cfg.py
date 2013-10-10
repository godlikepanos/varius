bulletPath_ = "../../../bullet/install/"

sourcePaths = [".", "../common"]

includePaths = []
includePaths.append("./")
includePaths.extend(list(sourcePaths))
includePaths.extend([bulletPath_ + "include/"])

precompiledHeaders = []

executableName = "test"

compiler = "g++"

defines__ = ""

precompiledHeadersFlags = defines__ + " -c -pedantic-errors -pedantic -ansi -Wall -Wextra -W -pipe -O0 -g3 -pg"

compilerFlags = precompiledHeadersFlags + " -fsingle-precision-constant"

linkerFlags = "-rdynamic -L" + bulletPath_ + "lib -Wl,-Bstatic -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath -Wl,-Bdynamic -lGL -lglut -lGLU -lSDL -lpthread"
