.PHONY: all configure build run \
        configure-debug build-debug run-debug \
        configure-tests run-tests coverage \
        format lint format-and-lint \
				format-diff lint-diff format-and-lint-diff \
        documentation clean help

MAKEFLAGS += --no-print-directory

# ------------------ Configurable Variables ------------------

BUILD_TYPE ?= Release
BUILD_DIR ?= build-$(BUILD_TYPE)

SRC_DIR := src
INCLUDE_DIR := include

EXECUTABLE_NAME ?= Lumen
TEST_EXECUTABLE_NAME ?= UnitTests

CMAKE := $(shell command -v cmake 2>/dev/null)
CLANG_FORMAT := $(shell command -v clang-format 2>/dev/null)
CLANG_TIDY := $(shell command -v clang-tidy 2>/dev/null)
GCOVR := $(shell command -v gcovr 2>/dev/null)

# Build flags
RELEASE_FLAGS := -DENABLE_WARNINGS=ON -DENABLE_SANITIZERS=OFF -DENABLE_LTO=ON -DENABLE_OPTIMIZATIONS=ON
DEBUG_FLAGS := -DENABLE_WARNINGS=ON -DENABLE_SANITIZERS=ON -DENABLE_LTO=OFF -DENABLE_OPTIMIZATIONS=OFF
TEST_FLAGS := $(DEBUG_FLAGS) -DENABLE_UNIT_TESTS=ON

CMAKE_FLAGS ?= $(RELEASE_FLAGS)

# Files to format and lint
CHANGED_CPP := $(shell git diff --name-only origin/main... -- $(SRC_DIR)/*.cpp)
CHANGED_HPP := $(shell git diff --name-only origin/main... -- $(INCLUDE_DIR)/*.hpp)

UNTRACKED_CPP := $(shell git ls-files --others --exclude-standard | grep '^$(SRC_DIR)/.*\.cpp$$')
UNTRACKED_HPP := $(shell git ls-files --others --exclude-standard | grep '^$(INCLUDE_DIR)/.*\.hpp$$')

FILES_TO_FORMAT := $(strip $(CHANGED_CPP) $(UNTRACKED_CPP) $(CHANGED_HPP) $(UNTRACKED_HPP))
FILES_TO_LINT := $(strip $(filter-out $(SRC_DIR)/main.cpp, $(CHANGED_CPP) $(UNTRACKED_CPP)))

FIX ?= OFF

# ------------------ Helper Functions ------------------

define check_tool
	@if [ -z "$($1)" ]; then echo "Error: $2 is not installed or not in PATH."; exit 1; fi
endef

# ------------------ Main Targets ------------------

all: build

configure:
	$(call check_tool,CMAKE,CMake)
	@echo "Configuring build in $(BUILD_TYPE) mode..."
	@cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(CMAKE_FLAGS)

build:
	@echo "Building project in $(BUILD_TYPE) mode..."
	@cmake --build $(BUILD_DIR) --parallel

run: build
	@echo "Running executable $(EXECUTABLE_NAME)..."
	@./$(BUILD_DIR)/$(EXECUTABLE_NAME)

# ------------------ Debug Targets ------------------

configure-debug:
	@$(MAKE) configure BUILD_TYPE=Debug CMAKE_FLAGS="$(DEBUG_FLAGS)"

build-debug:
	@$(MAKE) build BUILD_TYPE=Debug CMAKE_FLAGS="$(DEBUG_FLAGS)"

run-debug:
	@$(MAKE) run BUILD_TYPE=Debug CMAKE_FLAGS="$(DEBUG_FLAGS)"

# ------------------ Test Targets ------------------

configure-tests:
	@$(MAKE) configure BUILD_TYPE=Tests CMAKE_FLAGS="$(TEST_FLAGS)"

build-tests:
	@$(MAKE) build BUILD_TYPE=Tests CMAKE_FLAGS="$(TEST_FLAGS)"

run-tests: build-tests
	@echo "Running test executable $(TEST_EXECUTABLE_NAME)..."
	@./build-Tests/tests/$(TEST_EXECUTABLE_NAME)

coverage: run-tests
	$(call check_tool,GCOVR,gcovr)
	@echo "Generating coverage reports..."
	@mkdir -p tests/coverage_report
	@gcovr -f $(SRC_DIR) -f $(INCLUDE_DIR) --exclude='src/main.cpp' --exclude-throw-branches --json-summary -o tests/coverage_report/coverage_report.json
	@gcovr -f $(SRC_DIR) -f $(INCLUDE_DIR) --exclude='src/main.cpp' --exclude-throw-branches --html-details -o tests/coverage_report/coverage_report.html

# ------------------ Code Quality ------------------

format:
	$(call check_tool,CLANG_FORMAT,clang-format)
	@echo "Running clang-format..."
	@cmake -S . -B build-lint -DFILES_TO_CHECK="$(FILES)" -DENABLE_CLANG_FORMAT=ON -DENABLE_CLANG_TIDY=OFF
	@cmake --build build-lint --target run-clang-format

format-diff:
	$(call check_tool,CLANG_FORMAT,clang-format)
	@if [ -z "$(FILES_TO_FORMAT)" ]; then \
		echo "No files to format."; \
	else \
		echo "Running clang-format on changed files : $(FILES_TO_FORMAT)"; \
		cmake -S . -B build-lint -DFILES_TO_CHECK="$(FILES_TO_FORMAT)" -DENABLE_CLANG_FORMAT=ON -DENABLE_CLANG_TIDY=OFF; \
		cmake --build build-lint --target run-clang-format; \
	fi

lint:
	$(call check_tool,CLANG_TIDY,clang-tidy)
	@echo "Running clang-tidy (FIX=$(FIX))..."
	@cmake -S . -B build-lint -DFILES_TO_CHECK="$(FILES)" -DENABLE_CLANG_FORMAT=OFF -DENABLE_CLANG_TIDY=ON -DENABLE_FIX_CLANG_TIDY=$(FIX)
	@cmake --build build-lint --target run-clang-tidy

lint-diff:
	$(call check_tool,CLANG_TIDY,clang-tidy)
	@if [ -z "$(FILES_TO_LINT)" ]; then \
		echo "No files to lint."; \
	else \
		echo "Running clang-tidy on changed files : $(FILES_TO_LINT) (FIX=$(FIX))"; \
		cmake -S . -B build-lint -DFILES_TO_CHECK="$(FILES_TO_LINT)" -DENABLE_CLANG_FORMAT=OFF -DENABLE_CLANG_TIDY=ON -DENABLE_FIX_CLANG_TIDY=$(FIX); \
		cmake --build build-lint --target run-clang-tidy; \
	fi

format-and-lint: format lint
	@echo "Formatting and linting completed."

format-and-lint-diff: format-diff lint-diff
	@echo "Formatting and linting on changed files completed."

# ------------------ Documentation ------------------

documentation:
	@echo "Generating documentation with Doxygen..."
	@cmake -S . -B $(BUILD_DIR) -DENABLE_DOXYGEN=ON
	@cmake --build $(BUILD_DIR) --target generate-doc

# ------------------ Cleanup ------------------

clean:
	@echo "Cleaning all build and coverage artifacts..."
	@rm -rf build-* tests/coverage_report

# ------------------ Help ------------------

help:
	@echo "Available make targets:"
	@echo "  all (default)          - Build the project in Release mode"
	@echo "  configure              - Configure the project"
	@echo "  build                  - Build the project"
	@echo "  run                    - Run the executable"
	@echo "  configure-debug        - Configure the project in Debug mode"
	@echo "  build-debug            - Build the project in Debug mode"
	@echo "  run-debug              - Run the executable in Debug mode"
	@echo "  configure-tests        - Configure the project for tests"
	@echo "  build-tests            - Build the project for tests"
	@echo "  run-tests              - Run the tests"
	@echo "  coverage               - Generate coverage reports"
	@echo "  format                 - Format all source files (use FILES=\"file1.cpp file2.cpp\" to specify files)"
	@echo "  format-diff            - Format changed source files"
	@echo "  lint                   - Lint all source files (use FILES=\"file1.cpp file2.cpp\" to specify files)"
	@echo "  lint-diff              - Lint changed source files"
	@echo "  format-and-lint        - Format and lint all source files"
	@echo "  format-and-lint-diff   - Format and lint changed source files"
	@echo "  documentation          - Generate documentation with Doxygen"
	@echo "  clean                  - Clean all build and coverage artifacts"

