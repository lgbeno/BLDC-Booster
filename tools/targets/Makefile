ifneq (,$(findstring /cygdrive/,$(PATH)))
	UNAME := Cygwin
else
ifneq (,$(findstring WINDOWS,$(PATH)))
	UNAME := MinGW
else
	UNAME := $(shell uname -s)
endif
endif


ifeq ($(UNAME), Linux)
	SUBDIRS = linux.x86
endif
ifeq ($(UNAME), Cygwin)
	SUBDIRS = cygwin.x86
endif
ifeq ($(UNAME), MinGW)
	SUBDIRS = mingw.x86
endif

all:
	$(foreach dir, $(SUBDIRS), $(MAKE) -C $(dir) all || exit 1;)

clean:
	$(foreach dir, $(SUBDIRS), $(MAKE) -C $(dir) clean || exit 1;)

