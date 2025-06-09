objects = src/main.o src/system.o src/auth.o

atm : $(objects)
	cc -o atm $(objects)

main.o : src/header.h
kbd.o : src/header.h
command.o : src/header.h
display.o : src/header.h
insert.o : src/header.h
search.o : src/header.h
files.o : src/header.h
utils.o : src/header.h

clean :
	rm -f $(objects) atm
	rm -f ./share/atm/data/users.txt
	rm -f ./share/atm/data/records.txt

uninstall :
	rm -f $(objects) atm
	rm -f $(DESTDIR)$(prefix)/share/atm/data/users.txt
	rm -f $(DESTDIR)$(prefix)/share/atm/data/records.txt
	-rmdir --ignore-fail-on-non-empty $(DESTDIR)$(prefix)/share/atm/data

.PHONY: all clean uninstall install-local uninstall-local
all: atm
install: install-local
install: install-local
install: uninstall-local
install: uninstall-local
install: atm
	@echo "Installing ATM Management System..."
	@mkdir -p $(DESTDIR)$(prefix)/share/atm/data
	@cp atm $(DESTDIR)$(bindir)
	@cp src/header.h $(DESTDIR)$(includedir)/atm/
	@cp -r share/atm/data/* $(DESTDIR)$(prefix)/share/atm/data/
	@echo "Installation complete."

install-local:
	mkdir -p $(DESTDIR)$(datadir)/atm/data

uninstall-local:
	rm -f $(DESTDIR)$(datadir)/atm/data/users.txt
	rm -f $(DESTDIR)$(datadir)/atm/data/records.txt
	-rmdir --ignore-fail-on-non-empty $(DESTDIR)$(datadir)/atm/data