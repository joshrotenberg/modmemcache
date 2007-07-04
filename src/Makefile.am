mod_memcache_la_SOURCES = mod_memcache.c mod_memcache.h 
mod_memcache_la_CFLAGS = -Wall ${MODULE_CFLAGS}
mod_memcache_la_LDFLAGS = -rpath ${AP_LIBEXECDIR} -module -avoid-version ${MODULE_LDFLAGS}

mod_LTLIBRARIES = mod_memcache.la
moddir=${AP_LIBEXECDIR}

install: install-am
	rm -f $(DESTDIR)${AP_LIBEXECDIR}/mod_memcache.a
	rm -f $(DESTDIR)${AP_LIBEXECDIR}/mod_memcache.la