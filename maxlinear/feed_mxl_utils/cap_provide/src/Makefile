PKG_NAME := cap_provide

bins := cap_provide

all: capabilities-names.h

capabilities-names.h:
	./make_capabilities_h.sh $(CC) > ./capabilities-names.h

clean: cap_clean

cap_clean:
	rm -f ./capabilities-names.h

include make.inc
