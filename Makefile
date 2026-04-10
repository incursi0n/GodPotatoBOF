BOFNAME := BOF
COMINCLUDE := -I. -Icommon
LIBINCLUDE := 

# Compiler settings
# -nostdlib is used to prevent linking against standard system startup files
CC_x64 := x86_64-w64-mingw32-gcc -nostdlib
CC_x86 := i686-w64-mingw32-gcc -nostdlib
CXX_x64 := x86_64-w64-mingw32-g++
CXX_x86 := i686-w64-mingw32-g++
BOFLINK := ./boflink

# CFLAGS for BOF compatibility
# -fno-builtin: Prevents GCC from replacing calls with __mingw_ equivalents
# -D__USE_MINGW_ANSI_STDIO=0: Forces use of native msvcrt.dll exports for printf/sprintf
CFLAGS := -Os -c -DBOF -fno-builtin -D__USE_MINGW_ANSI_STDIO=0 -mno-stack-arg-probe

COMMON_SRCS := $(wildcard common/*.c)
COMMON_OBJS_x64 := $(patsubst common/%.c,build/common/%.x64.o,$(COMMON_SRCS))
COMMON_OBJS_x86 := $(patsubst common/%.c,build/common/%.x86.o,$(COMMON_SRCS))

ENTRY_OBJ_x64 := build/entry.x64.o
ENTRY_OBJ_x86 := build/entry.x86.o

OBJS_x64 := $(ENTRY_OBJ_x64) $(COMMON_OBJS_x64)
OBJS_x86 := $(ENTRY_OBJ_x86) $(COMMON_OBJS_x86)

.PHONY: all clean check scanbuild test

all: clean dist/$(BOFNAME).x64.o dist/$(BOFNAME).x86.o

# Linking stage

dist/$(BOFNAME).x64.o: $(OBJS_x64)
	@mkdir -p dist
	$(BOFLINK) --mingw64 $(OBJS_x64) --entry=go -lkernel32 -lmsvcrt -lntdll -ladvapi32 -lucrt -lgdi32 -luser32 -lws2_32 -lshell32 -lcomdlg32 -lole32 -loleaut32 -luuid -lwininet -lcrypt32 -ldbghelp -lversion -lsetupapi -o $@

dist/$(BOFNAME).x86.o: $(OBJS_x86)
	@mkdir -p dist
	$(BOFLINK) --mingw32 $(OBJS_x86) --entry=go -lkernel32 -lmsvcrt -lntdll -ladvapi32 -lucrt -lgdi32 -luser32 -lws2_32 -lshell32 -lcomdlg32 -lole32 -loleaut32 -luuid -lwininet -lcrypt32 -ldbghelp -lversion -lsetupapi -o $@

# Compilation stage for entry.c
$(ENTRY_OBJ_x64): entry.c
	@mkdir -p build
	$(CC_x64) -o $@ $(COMINCLUDE) $(CFLAGS) $<

$(ENTRY_OBJ_x86): entry.c
	@mkdir -p build
	$(CC_x86) -o $@ $(COMINCLUDE) $(CFLAGS) $<

# Compilation stage for common files
build/common/%.x64.o: common/%.c
	@mkdir -p build/common
	$(CC_x64) -o $@ $(COMINCLUDE) $(CFLAGS) $<

build/common/%.x86.o: common/%.c
	@mkdir -p build/common
	$(CC_x86) -o $@ $(COMINCLUDE) $(CFLAGS) $<

# Helpers
test:
	$(CXX_x64) entry.c $(COMMON_SRCS) -g $(COMINCLUDE) $(LIBINCLUDE) -o $(BOFNAME).x64.exe
	$(CXX_x86) entry.c $(COMMON_SRCS) -g $(COMINCLUDE) $(LIBINCLUDE) -o $(BOFNAME).x86.exe

scanbuild:
	$(CC_x64) --analyze entry.c $(COMMON_SRCS) -o $(BOFNAME).scanbuild.exe $(COMINCLUDE) $(LIBINCLUDE)

check:
	cppcheck --enable=all $(COMINCLUDE) --platform=win64 entry.c $(COMMON_SRCS)

clean:
	rm -f $(BOFNAME).*.exe
	rm -rf build dist
