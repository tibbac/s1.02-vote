#!/usr/bin/env -S make -f
PROJ = subdirs.pro
OUT_DIR ?= out
QMAKEFLAGS ?=

# On cherche si qmake est dans le PATH, sinon on prend un autre chemin
QMAKE != sh -ce 'command -v qmake || \
	{ [ -d /usr/lib/qt6 ] && echo "/usr/lib/qt6/bin/qmake"; } || \
	{ [ -d /usr/lib/qt5 ] && echo "/usr/lib/qt5/bin/qmake"; }'

all: release

run: all
	$(OUT_DIR)/vote/vote

debug: QMAKEFLAGS += CONFIG+=debug
debug: build

release: QMAKEFLAGS += CONFIG+=release
release: build

rel: release

build:
	mkdir -p $(OUT_DIR)
	$(QMAKE) $(QMAKEFLAGS) -o $(OUT_DIR)/Makefile $(PROJ)
	test -f $(OUT_DIR)/Makefile && cd $(OUT_DIR) && $(MAKE) all

format:
	clang-format -i $$(find . -type f -name '*.cc' -o -name '*.hh')

clean:
	test -f $(OUT_DIR)/Makefile && cd $(OUT_DIR) && $(MAKE) clean || :

distclean:
	test -f $(OUT_DIR)/Makefile && cd $(OUT_DIR) && $(MAKE) distclean || :

