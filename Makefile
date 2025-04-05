.PHONY: all configure build clean check-format-and-lint run-tests generate-doc

MAKEFLAGS += --no-print-directory

# Configure and build the project
all: configure build

# Check all source files with clang-format and clang-tidy
check-format-and-lint:
	@echo "Checking all files with clang-format and clang-tidy..."
	@cmake -S . -B build-lint -DFILES_TO_CHECK=all -DENABLE_CLANG_FORMAT=ON -DENABLE_CLANG_TIDY=ON
	@cmake --build build-lint --target run-clang-format
	@cmake --build build-lint --target run-clang-tidy

# Run tests after building
run-tests:
	@echo "Running unit tests..."
	@cmake -S . -B build-tests -DENABLE_TESTS=ON -DENABLE_COMPILER_OPTIMIZATIONS=OFF
	@cmake --build build-tests
	@./build-tests/tests/Tests

coverage-report:
	@echo "Generating code coverage report..."
	@gcovr -f src --exclude='src/main.cpp' --json-summary -o tests/coverage_report/coverage_report.json

# Met à jour le README.md avec la table
update-readme:
	@python3 tests/coverage_report/generate_coverage_table.py
	@awk '/<!-- COVERAGE-START -->/,/<!-- COVERAGE-END -->/ { next } { print }' README.md > README.tmp
	@echo '<!-- COVERAGE-START -->' >> README.tmp
	@cat COVERAGE_TABLE.md >> README.tmp
	@echo '<!-- COVERAGE-END -->' >> README.tmp
	@mv README.tmp README.md
	@rm COVERAGE_TABLE.md
	@echo "README.md mis à jour avec la couverture de code."


# Generate documentation using Doxygen
generate-doc:
	@echo "Generating documentation..."
	@cmake -S . -B build-dev -DENABLE_DOXYGEN=ON
	@cmake --build build-dev --target generate-doc

# Configure the project with CMake
configure:
	@echo "Configuring the project with CMake..."
	@cmake -S . -B build

# Build the project
build:
	@echo "Building development version..."
	@cmake -S . -B build-dev
	@cmake --build build-dev

# Clean the build folder
clean:
	@if [ -d "build" ]; then \
		echo "Removing 'build' folder..."; \
		rm -rf build-dev; \
		rm -rf build-tests; \
		rm -rf build-lint; \
	else \
		echo "'build' folder does not exist."; \
	fi
