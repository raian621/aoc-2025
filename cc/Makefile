BUILD_DIR := /tmp/aoc/build
DAY := day05
BINARY_PATH := ${BUILD_DIR}/${DAY}

.PHONY: all
all: ${BINARY_PATH}

${BINARY_PATH}: ${BUILD_DIR}/${DAY}.o ${BUILD_DIR}/common.o 
	g++ -o $@ $^

${BUILD_DIR}/%.o: %.cc | ${BUILD_DIR}
	g++ -c -o $@ $<

${BUILD_DIR}:
	@mkdir -p $@

.PHONY: clean
clean: ${BUILD_DIR}
	rm -rf ${BUILD_DIR}