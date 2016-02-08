#MAKEFILE

ENV=env
KORE=kore
CFLAGS=""
LDFLAGS="-ljson-c -lmysqlclient -lpthread -lz -lm -ldl -lconfig"

build:
	$(ENV) CFLAGS=$(CFLAGS) LDFLAGS=$(LDFLAGS) $(KORE) build

run:
	$(ENV) CFLAGS=$(CFLAGS) LDFLAGS=$(LDFLAGS) $(KORE) run

clean:
	$(KORE) clean