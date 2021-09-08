all:
	maike2
.PHONY: doc
doc:
	doxygen

clean:
	rm -rf __targets