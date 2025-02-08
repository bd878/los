.PHONY: dist
dist: 
	gcc -shared -Wall -Wl,-soname,libcgutils.so -fPIC cgroup_utils.c cgroup_utils.h compiler.h -o libcgutils.so -lc

.PHONY: install
install:
	mkdir -p $(DESTDIR)/usr/local/{lib,include}
	install -m 0755 libcgutils.so $(DESTDIR)/usr/local/lib/
	install -m 0644 cgroup_utils.h $(DESTDIR)/usr/local/include/
