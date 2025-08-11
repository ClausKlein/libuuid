#
# Standard stuff
#
.SUFFIXES:

# Disable the built-in implicit rules.
MAKEFLAGS += --no-builtin-rules

export CC:=clang-20
export CCFLAGS:=-Wall -Wextra -Wpedantic

.PHONY: all check clean distclean

all: build
	ninja -C build test

build:
	cmake -S . -B build

check: build
	# run-clang-tidy -p build -checks='-*,clang-analyzer-*' -warnings-as-errors='*'
	run-clang-tidy -p build -checks='-*,cert-*' -warnings-as-errors='*'
	run-clang-tidy -p build -checks='-*,cert-*,portability-*,performance-*,readability-*,-readability-magic-numbers,-readability-identifier-*'

fix: build
	run-clang-tidy -p build -checks='-*,readability-braces-around-statements,readability-isolate-declaration,readability-non-const-parameter,google-runtime-int,readability-named-parameter,readability-inconsistent-declaration-parameter-name' -fix

clean: build
	ninja -C build clean

distclean:
	rm -rf build
