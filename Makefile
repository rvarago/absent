PROJECT_NAME=absent
PROFILE=../profiles/common
BUILD_TESTING=true

.PHONY: all test install compile gen dep mk clean env env-test

all: compile

env:
	docker build -t ${PROJECT_NAME}:0.1 -f Dockerfile .

env-test: env
	docker run --rm -t ${PROJECT_NAME}:0.1

install: compile
	cd build && cmake --build . --target install

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
