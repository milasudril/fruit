all:
	maike2
.PHONY: doc
doc:
	doxygen

.PHONY: clean
clean:
	rm -rf __targets

.PHONY: format
format:
	find -name '*.hpp' | xargs devtools/include_guard_fix.py