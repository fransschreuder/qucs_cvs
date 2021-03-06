## Process this file with automake to produce Makefile.in
# 
# Makefile.am
#
# Automake input file.
#
# Copyright (C) 2004, 2005, 2006, 2008, 2009 Stefan Jahn <stefan@lkcc.org>
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this package; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
# Boston, MA 02110-1301, USA.  
#

SUBDIRS = qucs qucs-edit qucs-help qucs-transcalc qucs-filter qucs-lib \
	qucs-attenuator qucs-filter-v2 contrib $(RELEASEDIRS)

EXTRA_DIST = autogen.sh depcomp PLATFORMS RELEASE Info.plist

# MacOSX specific installation of applications
if COND_MACOSX
app_PROGS = $(top_builddir)/qucs/qucs \
  $(top_builddir)/qucs-attenuator/qucsattenuator \
  $(top_builddir)/qucs-filter/qucsfilter \
  $(top_builddir)/qucs-help/qucshelp \
  $(top_builddir)/qucs-lib/qucslib \
  $(top_builddir)/qucs-edit/qucsedit \
  $(top_builddir)/qucs-transcalc/qucstrans

install-exec-hook: mac-install-apps mac-install-framework

mac-install-apps:
	@echo "Creating MacOSX applications...";
	@list='$(app_PROGS)'; for file in $$list; do \
	app=`basename $$file` && \
	$(mkinstalldirs) $(bindir)/$$app.app/Contents && \
	$(mkinstalldirs) $(bindir)/$$app.app/Contents/Resources && \
	$(mkinstalldirs) $(bindir)/$$app.app/Contents/MacOS && \
	$(install_sh_PROGRAM) $$file $(bindir)/$$app.app/Contents/MacOS/ && \
	strip $(bindir)/$$app.app/Contents/MacOS/$$app && \
	case $$app in \
	  qucs)		  desc="Qucs";; \
	  qucsattenuator) desc="Qucs Attenuator";; \
	  qucsfilter)	  desc="Qucs Filter";; \
	  qucshelp)	  desc="Qucs Help";; \
	  qucslib)	  desc="Qucs Library";; \
	  qucsedit)	  desc="Qucs Editor";; \
	  qucstrans)	  desc="Qucs Transcalc";; \
	esac && \
	cat $(srcdir)/Info.plist | \
	  sed -e "s/@version@/$(PACKAGE_VERSION)/g" | \
	  sed -e "s/@name@/$$desc/g" | \
	  sed -e "s/@exec@/$$app/g" > \
	  $(bindir)/$$app.app/Contents/Info.plist && \
	$(install_sh_DATA) $(srcdir)/qucs/bitmaps/$$app.icns \
	  $(bindir)/$$app.app/Contents/Resources/application.icns && \
	echo "#!/bin/sh\n$(bindir)/$$app.app/Contents/MacOS/$$app \$$*\n" > \
	  $(bindir)/$$app && \
	chmod +x $(bindir)/$$app; \
	done

mac-install-framework:
	@echo "Copying and setting up MacOSX/Qt framework...";
	@if test -e "$(QTDIR)/lib/libqt.3.dylib"; then \
	  qtlib="$(QTDIR)/lib/libqt.3.dylib"; fi && \
	if test -e "$(QTDIR)/lib/libqt-mt.3.dylib"; then \
	  qtlib="$(QTDIR)/lib/libqt-mt.3.dylib"; fi && \
	if ! test -z "$$qtlib"; then \
	  $(mkinstalldirs) "$(bindir)/qucs.app/Contents/Frameworks"; \
	  cp "$$qtlib" "$(bindir)/qucs.app/Contents/Frameworks/"; \
	  qtlib=`basename $$qtlib`; \
	  install_name_tool -id "@executable_path/../Frameworks/$$qtlib" \
	    "$(bindir)/qucs.app/Contents/Frameworks/$$qtlib"; \
	  list='$(app_PROGS)'; for file in $$list; do \
	  app=`basename $$file` && \
	  install_name_tool -change $$qtlib \
	    "@executable_path/../../../qucs.app/Contents/Frameworks/$$qtlib" \
	    "$(bindir)/$$app.app/Contents/MacOS/$$app"; \
	  done; \
	fi

uninstall-hook:
	@list='$(app_PROGS)'; for file in $$list; do \
	app=`basename $$file` && \
	rm -rf $(bindir)/$$app.app; \
	done
else
install-exec-hook:
uninstall-hook:
endif

CLEANFILES = *~ *.rej *.orig
MAINTAINERCLEANFILES = aclocal.m4 config.h.in configure Makefile.in \
        stamp-h.in stamp-h[0-9].in
DISTCLEANFILES = config.cache config.log
