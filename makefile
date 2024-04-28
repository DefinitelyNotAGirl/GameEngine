ENGINEDIR=Engine
CLASSDIR=Classes

SOURCE_asm_asm=$(wildcard src/*.asm)
SOURCE_asm_asm+=$(wildcard src/$(ENGINEDIR)/*.asm)
OBJECTS_asm_asm=$(patsubst src/%.asm,build/%.o,$(SOURCE_asm_asm))
DEPFILES_asm_asm=$(patsubst src/%.asm,build/%.d,$(SOURCE_asm_asm))
SOURCE_asm_s=$(wildcard src/*.s)
SOURCE_asm_s+=$(wildcard src/$(ENGINEDIR)/*.s)
OBJECTS_asm_s=$(patsubst src/%.s,build/%.o,$(SOURCE_asm_s))
DEPFILES_asm_s=$(patsubst src/%.s,build/%.d,$(SOURCE_asm_s))
SOURCE_cpp_cpp=$(wildcard src/*.cpp)
SOURCE_cpp_cpp+=$(wildcard src/$(ENGINEDIR)/*.cpp)
SOURCE_cpp_cpp+=$(wildcard src/$(CLASSDIR)/*.cpp)
OBJECTS_cpp_cpp=$(patsubst src/%.cpp,build/%.o,$(SOURCE_cpp_cpp))
DEPFILES_cpp_cpp=$(patsubst src/%.cpp,build/%.d,$(SOURCE_cpp_cpp))
SOURCE_cpp_cxx=$(wildcard src/*.cxx)
SOURCE_cpp_cxx+=$(wildcard src/$(ENGINEDIR)/*.cxx)
SOURCE_cpp_cxx+=$(wildcard src/$(CLASSDIR)/*.cxx)
OBJECTS_cpp_cxx=$(patsubst src/%.cxx,build/%.o,$(SOURCE_cpp_cxx))
DEPFILES_cpp_cxx=$(patsubst src/%.cxx,build/%.d,$(SOURCE_cpp_cxx))
SOURCE_c_c=$(wildcard src/*.c)
SOURCE_c_c+=$(wildcard src/$(ENGINEDIR)/*.c)
OBJECTS_c_c=$(patsubst src/%.c,build/%.o,$(SOURCE_c_c))
DEPFILES_c_c=$(patsubst src/%.c,build/%.d,$(SOURCE_c_c))

CXXARGS=
CCARGS=
ASARGS=

LD=g++

all: game

build/%.o: src/%.asm
	@$(AS) $(ASARGS) -MD -c -o $@ $<
	$(info  	$(AS)	$<)
build/%.o: src/%.s
	@$(AS) $(ASARGS) -MD -c -o $@ $<
	$(info  	$(AS)	$<)
build/%.o: src/%.cpp
	@$(CXX) -O3 $(CXXARGS) -std=c++20 -Wno-write-strings -rdynamic -pedantic -Wno-conversion-null -Wno-narrowing -Wunreachable-code -Wno-literal-suffix -Wno-pointer-arith -MP -MD -fpermissive -Iinc/ -g -c -o $@ $<
	$(info 	$(CXX)	$<)
build/%.o: src/%.cxx
	@$(CXX) -O3 $(CXXARGS) -std=c++20 -Wno-write-strings -rdynamic -pedantic -Wno-conversion-null -Wno-narrowing -Wno-multichar -Wunreachable-code -Wno-literal-suffix -Wno-pointer-arith -MP -MD -fpermissive -Iinc/ -g -c -o $@ $<
	$(info 	$(CXX)	$<)
build/%.o: src/%.c
	@$(CC) $(CCARGS) -Wunreachable-code -Iinc/ -c -o $@ $<
	$(info  	$(CC)	$<)
clean:
	@-rm -r build/*.o
	$(info  	DELETE	build/*.o)
	@-rm -r build/*.d
	$(info  	DELETE	build/*.d)
	@-rm -r build/Engine/*.o
	$(info  	DELETE	build/Engine/*.o)
	@-rm -r build/Engine/*.d
	$(info  	DELETE	build/Engine/*.d)

ARGS_LD_LINUX=
ARGS_LD_PLATFORM=$(ARGS_LD_LINUX)

game:  $(OBJECTS_asm_asm) $(OBJECTS_asm_s) $(OBJECTS_cpp_cxx) $(OBJECTS_cpp_cpp) $(OBJECTS_c_c)
	@$(LD) -g -rdynamic $(OBJECTS_asm_asm) $(OBJECTS_asm_s) $(OBJECTS_cpp_cxx) $(OBJECTS_cpp_cpp) $(OBJECTS_c_c) -Llib/ $(ARGS_LD_PLATFORM) -lc -lstdc++ -lcargparse -lglfw -lGL -o game
	$(info  	$(LD)	$@)

countLines:
	@find src inc -type f \( -iname \*.cxx -o -iname \*.cpp -o -iname \*.c -o -iname \*.h -o -iname \*.hpp -o -iname \*.hxx \) -not -path "src/glad.c" -not -path "inc/Engine/stb_image.h" -not -path "inc/KHR/*" -not -path "inc/glad/*" -exec cat {} \; | wc -l

listCodeFiles:
	@find src inc -type f \( -iname \*.cxx -o -iname \*.cpp -o -iname \*.c -o -iname \*.h -o -iname \*.hpp -o -iname \*.hxx \) -not -path "src/glad.c" -not -path "inc/Engine/stb_image.h" -not -path "inc/KHR/*" -not -path "inc/glad/*" -print

-include $(DEPFILES_asm_asm)
-include $(DEPFILES_asm_s)
-include $(DEPFILES_cpp_cpp)
-include $(DEPFILES_cpp_cxx)
-include $(DEPFILES_c_c)
