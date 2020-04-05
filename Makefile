PROJECT_NAME            = absent
PROFILE                 = ../profiles/common
BUILD_TESTS             = ON
BUILD_DIR               = build
BUILD_TYPE              = Debug

.PHONY: all test install compile gen dep mk clean env env-test env-format-check format-check format

all: compile

env:
	docker build -t ${PROJECT_NAME} .

env-format-check: env
	docker run --rm ${PROJECT_NAME} make format-check --no-print-directory

env-test: env
	docker run --rm ${PROJECT_NAME} make compile test --no-print-directory

install:
	cd ${BUILD_DIR} && cmake --build . --target install

test:
	cd ${BUILD_DIR} && ctest -VV .

compile: gen
	cd ${BUILD_DIR} && cmake --build .

gen: dep
	cd ${BUILD_DIR} && cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DBUILD_TESTS=${BUILD_TESTS} ..

dep: mk
	cd ${BUILD_DIR} && conan install .. --build=missing -pr ${PROFILE} -s build_type=${BUILD_TYPE}

mk:
	mkdir -p ${BUILD_DIR}

clean:
	rm -rf ${BUILD_DIR}

format-check:
	@if git ls-files -- '*.cpp' '*.h' | xargs clang-format -style=file -output-replacements-xml | grep -q "<replacement "; then \
		echo "Source code did not adhere to the expected formatting style"; exit 1; \
	fi

format:
	@git ls-files -- '*.cpp' '*.h' | xargs clang-format -i -style=file