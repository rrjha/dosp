
#Redirect make into the Xinu tree for now
all:
	make -C src/compile

clean:
	make -C src/compile clean

.PHONY: all
