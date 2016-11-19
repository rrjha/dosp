
#Redirect make into the Xinu tree for now
all:
	make -C xinu/compile

clean:
	make -C xinu/compile clean

.PHONY: all
