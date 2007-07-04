mod_memcache_example_la_SOURCES = mod_memcache_example.c
mod_memcache_example_la_CFLAGS = -Wall ${MODULE_CFLAGS}
mod_memcache_example_la_LDFLAGS = -rpath ${AP_LIBEXECDIR} -module -avoid-version ${MODULE_LDFLAGS}

mod_LTLIBRARIES = mod_memcache_example.la
moddir=${AP_LIBEXECDIR}

install: install-am
	rm -f $(DESTDIR)${AP_LIBEXECDIR}/mod_memcache_example.a
	rm -f $(DESTDIR)${AP_LIBEXECDIR}/mod_memcache_example.la