PROJECT_NAME            = absent
PROFILE                 = ../profiles/common
BUILD_TESTS             = ON
BUILD_TESTS_FOR_BOOST   = OFF
PACKAGE_VERSION         =
PACKAGE_REFERENCE       = ${PROJECT_NAME}/${PACKAGE_VERSION}@rvarago/stable
BUILD_DIR               = build

.PHONY: all conan-package env-conan-package test install compile gen dep mk clean env env-test

all: compile

env:
	docker build -t ${PROJECT_NAME} .

env-test: env
	docker run --rm ${PROJECT_NAME} make compile test --no-print-directory

env-conan-package: env
	docker run --rm ${PROJECT_NAME} make compile conan-package BUILD_TESTS=${BUILD_TESTS} PACKAGE_VERSION=${PACKAGE_VERSION} --no-print-directory

install:
	cd ${BUILD_DIR} && cmake --build . --target install

conan-package:
	conan create . ${PACKAGE_REFERENCE}

test:
	cd ${BUILD_DIR} && ctest -VV .

compile: gen
	cd ${BUILD_DIR} && cmake --build .

gen: dep
	cd ${BUILD_DIR} && cmake -D BUILD_TESTS=${BUILD_TESTS} -D BUILD_TESTS_FOR_BOOST=${BUILD_TESTS_FOR_BOOST} ..

dep: mk
	cd ${BUILD_DIR} && conan install .. --build=missing -pr ${PROFILE}

mk:
	mkdir -p ${BUILD_DIR}

clean:
	rm -rf ${BUILD_DIR}
