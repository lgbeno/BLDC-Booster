SUBDIRS = targets

all:
	$(foreach dir, $(SUBDIRS), $(MAKE) -C $(dir) all || exit 1;)

clean:
	$(foreach dir, $(SUBDIRS), $(MAKE) -C $(dir) clean || exit 1;)
	rm -rf instaspin instaspin.exe

