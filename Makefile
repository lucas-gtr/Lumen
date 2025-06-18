.PHONY: configure build run \
		configure-debug build-debug run-debug \
		configure-tests run-tests coverage \
	  format lint format-and-lint \
		update-readme generate-doc \
	  clean

MAKEFLAGS += --no-print-directory

configure:
	@echo "Configuring development build environment..."
	@cmake -S . -B build-dev -DENABLE_LTO=ON
	
build: 
	@echo "Building project..."
	@cmake --build build-dev

run: build
	@echo "Launching Lumen executable..."
	@./build-dev/Lumen

configure-debug:
	@echo "Configuring debug build environment..."
	@cmake -S . -B build-debug -DENABLE_LTO=OFF -DENABLE_COMPILER_OPTIMIZATIONS=OFF -DENABLE_DEBUG_COMPILE=ON

build-debug:
	@echo "Building project in debug mode..."
	@cmake --build build-debug

run-debug: build-debug
	@echo "Launching Lumen executable in debug mode..."
	@./build-debug/Lumen

format:
	@echo "Running clang-format on codebase..."
	@cmake -S . -B build-lint -DFILES_TO_CHECK=all -DENABLE_CLANG_FORMAT=ON -DENABLE_CLANG_TIDY=OFF
	@cmake --build build-lint --target run-clang-format

lint:
	@echo "Running clang-tidy analysis..."
	@cmake -S . -B build-lint -DFILES_TO_CHECK=all -DENABLE_CLANG_FORMAT=OFF -DENABLE_CLANG_TIDY=ON
	@cmake --build build-lint --target run-clang-tidy

format-and-lint: format lint
	@echo "Clang-format and clang-tidy checks completed."

configure-tests:
	@echo "Setting up test environment..."
	@cmake -S . -B build-tests -DENABLE_TESTS=ON -DENABLE_COMPILER_OPTIMIZATIONS=OFF

run-tests:
	@echo "Executing unit tests..."
	@cmake --build build-tests
	@./build-tests/tests/Tests

coverage: run-tests
	@echo "Generating coverage reports..."
	@mkdir -p tests/coverage_report
	@gcovr -f src -f include --exclude='src/main.cpp' --exclude-throw-branches --json-summary -o tests/coverage_report/coverage_report.json
	@gcovr -f src -f include --exclude='src/main.cpp' --exclude-throw-branches --html-details -o tests/coverage_report/coverage_report.html

update-readme:
	@echo "Injecting coverage report into README.md..."
	@python3 tests/generate_coverage_table.py
	@awk ' \
		BEGIN { in_block = 0 } \
		/<!-- COVERAGE-START -->/ { print; print ""; while ((getline line < "COVERAGE_TABLE.md") > 0) print line; in_block = 1; next } \
		/<!-- COVERAGE-END -->/ { in_block = 0; print ""; print; next } \
		!in_block { print } \
	' README.md > README.tmp
	@mv README.tmp README.md
	@rm COVERAGE_TABLE.md
	@echo "README.md successfully updated."

generate-doc:
	@echo "Building project documentation..."
	@cmake -S . -B build-dev -DENABLE_DOXYGEN=ON
	@cmake --build build-dev --target generate-doc

clean:
	@echo "Cleaning all build artifacts..."
	@rm -rf build-dev build-tests build-lint
	@echo "Cleanup complete."
