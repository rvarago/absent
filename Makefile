PROJECT_NAME            = absent
PROFILE                 = ../profiles/common
WITH_TESTS              = true
WITH_TESTS_FOR_BOOST    = false
PACKAGE_VERSION         =
PACKAGE_REFERENCE       = ${PROJECT_NAME}/${PACKAGE_VERSION}@rvarago/stable
BUILD_DIR               = build

.PHONY: all conan-package env-conan-package test install compile gen dep mk clean env env-test

all: compile

env:
	docker build -t ${PROJECT_NAME} .

env-test: env
	docker run --rm ${PROJECT_NAME}

env-conan-package: env
	docker run --rm ${PROJECT_NAME} make conan-package WITH_TESTS=${WITH_TESTS} PACKAGE_VERSION=${PACKAGE_VERSION}

install: compile
	cd ${BUILD_DIR} && cmake --build . --target install

conan-package: compile
	conan create . ${PACKAGE_REFERENCE}

test: compile
	cd ${BUILD_DIR} && ctest -VV .

compile: gen
	cd ${BUILD_DIR} && cmake --build .

gen: dep
	cd ${BUILD_DIR} && cmake -D WITH_TESTS=${WITH_TESTS} -D WITH_TESTS_FOR_BOOST=${WITH_TESTS_FOR_BOOST} ..

dep: mk
	cd ${BUILD_DIR} && conan install .. --build=missing -pr ${PROFILE}

mk:
	mkdir -p ${BUILD_DIR}

clean:
	rm -rf ${BUILD_DIR}
