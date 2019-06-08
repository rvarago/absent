PROJECT_NAME        = absent
PROFILE             = ../profiles/common
BUILD_TESTING       = true
PACKAGE_VERSION     =
PACKAGE_REFERENCE   = ${PROJECT_NAME}/${PACKAGE_VERSION}@rvarago/stable

.PHONY: all conan-package env-conan-package test install compile gen dep mk clean env env-test

all: compile

env:
	docker build -t ${PROJECT_NAME} .

env-test: env
	docker run --rm ${PROJECT_NAME}

env-conan-package: env
	docker run --rm ${PROJECT_NAME} make conan-package PACKAGE_VERSION=${PACKAGE_VERSION}

install: compile
	cd build && cmake --build . --target install

conan-package: test
	conan create . ${PACKAGE_REFERENCE}

test: compile
	cd build && ctest .

compile: gen
	cd build && cmake --build .

gen: dep
	cd build && cmake -D BUILD_TESTING=${BUILD_TESTING} ..

dep: mk
	cd build && conan install .. --build=missing -pr ${PROFILE}

mk:
	-mkdir build

clean:
	rm -rf build
