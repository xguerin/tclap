if DOC

all:
	@DOXYGEN@ Doxyfile

EXTRA_DIST = manual.xml
dist_doc_DATA = index.html manual.html style.css
docdir = ${datadir}/doc/${PACKAGE}

install-data-local :
	$(mkdir_p) $(DESTDIR)$(docdir)
	cp -R $(abs_srcdir)/html $(DESTDIR)$(docdir)

uninstall-local :
	chmod -R o+w $(DESTDIR)$(docdir)
	rm -rf $(DESTDIR)$(docdir)

dist-hook :
	$(mkdir_p) $(distdir)
	cp -R $(abs_srcdir)/html $(distdir)

clean-local:
	$(RM) -rf $(abs_builddir)/html/*
	$(RM) -rf $(abs_builddir)/doxygen_sqlite3.db

else

all:

endif
