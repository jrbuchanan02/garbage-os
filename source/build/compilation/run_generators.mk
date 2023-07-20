
source/build/generation/%.py:
	@echo Running $@
	python3 $@
	@echo $@ built.

run: $(wildcard source/build/generation/*.py)