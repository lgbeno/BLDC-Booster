TARGET := $(shell basename "`pwd`")
include ../../etc/$(TARGET).mk
VPATH = ../../src
INCLUDES = -I../../src/
FULLPATHSRCS = $(wildcard $(VPATH)/*.c)
SRCS = $(notdir $(FULLPATHSRCS))
OBJS = $(SRCS:.c=.o)
CFLAGS += $(INCLUDES)

EXECUTABLE = instaspin$(EXTENTION)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS) $(FULLPATHLIBS)
	$(LD) -o $@ $(OBJS) $(LDFLAGS) $(SYSLIBRARIES)
	cp -rf $(EXECUTABLE) ../../

-include $(OBJS:.o=.d)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *.d *.a *.so $(EXECUTABLE)

