.PHONY: all run check-format-and-lint configure-tests run-tests coverage-report update-readme generate-doc configure-dev build-dev clean

MAKEFLAGS += --no-print-directory

# Run the development version of the program
run:
	@make build-dev
	@./build-dev/Lumen

# Check all source files with clang-format and clang-tidy
check-format-and-lint:
	@echo "Checking all files with clang-format and clang-tidy..."
	@cmake -S . -B build-lint -DFILES_TO_CHECK=all -DENABLE_CLANG_FORMAT=ON -DENABLE_CLANG_TIDY=ON
	@cmake --build build-lint --target run-clang-format
	@cmake --build build-lint --target run-clang-tidy

# Configure the project for unit tests
configure-tests:
	@echo "Configuring unit tests..."
	@cmake -S . -B build-tests -DENABLE_TESTS=ON -DENABLE_COMPILER_OPTIMIZATIONS=OFF

# Run tests after building
run-tests:
	@echo "Running unit tests..."
	@cmake --build build-tests
	@./build-tests/tests/Tests

coverage-report: run-tests
	@echo "Generating code coverage report..."
	@gcovr -f src -f include --exclude='src/main.cpp' --exclude-throw-branches --json-summary -o tests/coverage_report/coverage_report.json
	@gcovr -f src -f include --exclude='src/main.cpp' --exclude-throw-branches --html-details -o tests/coverage_report/coverage_report.html

# Met à jour le README.md avec la table
update-readme:
	@python3 tests/generate_coverage_table.py
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
configure-dev:
	@echo "Configuring the project with CMake..."
	@cmake -S . -B build-dev

# Build the project
build-dev:
	@echo "Building development version..."
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
