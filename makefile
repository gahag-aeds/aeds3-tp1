SrcDir = src

CompilationUnits = $(shell find $(SrcDir) -name '*.c')

OutputFile = kontest


Build    = clang
Standard = -std=c99 -pedantic
Warnings = -Wall -Wextra -Werror
Optimize = -O2 -flto
Libs     = -lm

BuildFlags  = $(Warnings)   \
              $(Standard)   \
              $(Optimize)   \
              $(Libs)       \
              -I $(SrcDir)  \
              -o $(OutputFile)

DebugFlags = -g

ReleaseFlags = -DNDEBUG



all: release-kontest


build-kontest: $(SrcDir)
	@$(Build) $(BuildFlags) $(DebugFlags) $(CompilationUnits)

release-kontest: $(SrcDir)
	@$(Build) $(BuildFlags) $(ReleaseFlags) $(CompilationUnits)


clean:
	rm $(OutputFile)
