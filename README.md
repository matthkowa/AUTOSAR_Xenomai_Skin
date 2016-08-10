# AUTOSAR API FOR XENOMAI







##### MEMO
VARIABLES : 

    $XENO_DIR = Xenomai directory
    $BUILD_DIR = Build directory
    $DEST_DIR = Destination directory
##### Install Libraries for I686 
Cmd :

    cd $XENO_DIR
	./scripts/bootstrap --with-core=cobalt
	mkdir $BUILD_DIR
	cd $BUILD_DIR
	../configure --with-core=cobalt --host=i686-linux CFLAGS="-m32 -O2" LDFLAGS="-m32" --disable-doc-install --enable-debug --enable-lores-clock --enable-registry
	make DESTDIR=$DEST_DIR install
	
##### Cross compile app
Makefile :

    XENO_CONFIG := $DEST_DIR/usr/xenomai/bin/xeno-config
    CFLAGS := $(shell $(XENO_CONFIG) --autosar --cflags)
    LDFLAGS := $(shell $(XENO_CONFIG) --autosar --ldflags)
    CC := $(shell $(XENO_CONFIG) --cc)

    EXECUTABLE := myApp

    all: $(EXECUTABLE)

    %: %.c
	    $(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)
	    
Cmd :
    
    DESTDIR=$DEST_DIR make
