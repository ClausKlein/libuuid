#
# Standard stuff
#
.SUFFIXES:

# Disable the built-in implicit rules.
MAKEFLAGS += --no-builtin-rules

export CC:=clang
export CCFLAGS:=-Wall -Wextra -Wpedantic

.PHONY: all check clean distclean

all: build
	ninja -C build test

build:
	cmake -S . -B build

check: build
	run-clang-tidy -p build -checks='-*,clang-analyzer-*' -warnings-as-errors='*'
	run-clang-tidy -p build -checks='-*,cert-*' -warnings-as-errors='*'
	run-clang-tidy -p build -checks='-*,cert-*,portability-*,performance-*,readability-*,-readability-magic-numbers,-readability-identifier-*'

fix: build
	run-clang-tidy -p build -checks='-*,\
readability-braces-around-statements,\
readability-use-concise-preprocessor-directives,\
readability-inconsistent-declaration-parameter-name,\
readability-isolate-declaration,\
readability-named-parameter,\
readability-non-const-parameter,\
readability-redundant-casting,\
google-runtime-int' -fix

clean: build
	ninja -C build clean

distclean:
	rm -rf build
