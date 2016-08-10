XENO_CONFIG := /home/matthieuk/Téléchargements/xenomai-3-3.0.2/fs/usr/xenomai/bin/xeno-config
CFLAGS := $(shell $(XENO_CONFIG) --autosar --cflags)
LDFLAGS := $(shell $(XENO_CONFIG) --autosar --ldflags)
CC := $(shell $(XENO_CONFIG) --cc)

EXECUTABLE := alarm_1

all: $(EXECUTABLE)

%: %.c
	 $(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)
