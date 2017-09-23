SrcDir = src

CompilationUnits = $(shell find $(SrcDir) -name '*.c')

OutputFile = kontest


Build    = gcc
Standard = c99 -pedantic
Warnings = -Wall -Wextra # -Werror
Optimize = -O2 -flto

BuildFlags  = $(Warnings)       \
              -std=$(Standard)  \
              $(Optimize)       \
              -I $(SrcDir)      \
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
