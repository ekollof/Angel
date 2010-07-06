TOP             = .
DIRS            = src testprg
DISTNAME        = angel
VERSION         = 0.70
CVSROOT         = /home/coolvibe/cvsroot
CVS_RSH         = ssh
CTAGS           = ctags


all             :
	@for i in $(DIRS); do \
                cd $$i; \
                echo "===> $$i"; \
                CC=$(CC) $(MAKE) all; \
                cd ..; \
        done

clean           :
	@for i in $(DIRS); do \
        	cd $$i; \
		echo "===> $$i"; \
		$(MAKE) clean; \
                cd ..; \
        done
	-rm -f *~ *.BAK *.core core etc/*~

tags            :
	@for i in $(DIRS); do \
                cd $$i; \
                echo "===> $$i"; \
                $(MAKE) tagfile; \
                cd ..; \
        done

commit          :
	export CVSROOT=$(CVSROOT)
	export CVS_RSH=$(CVS_RSH)
	cvs commit

update          :
	cvs update

# Distribution... Test build, clean and then pack.

dist: clean
	@echo Packing...
	@cd .. && \
	tar zcf $(DISTNAME)-$(VERSION).tar.gz $(DISTNAME) && \
	ls -l $(DISTNAME)-$(VERSION).tar.gz

