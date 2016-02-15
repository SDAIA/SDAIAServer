#MAKEFILE

KORE=kore
CFLAGS+=-std=c99
LDFLAGS+=-ljson-c -lmysqlclient -lpthread -lz -lm -ldl -lconfig

build:
	CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" $(KORE) build

run:
	CFLAGS=$(CFLAGS) LDFLAGS=$(LDFLAGS) $(KORE) run

clean:
	$(KORE) clean
