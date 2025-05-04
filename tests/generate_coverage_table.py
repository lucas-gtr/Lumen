# generate_coverage_table.py

import json

with open("tests/coverage_report/coverage_report.json") as f:
    data = json.load(f)

def format_entry(percent, covered, total):
    if percent is None or covered is None or total is None:
        return "N/A"
    return f"{percent:.2f}% ({covered}/{total})"

lines = []
lines.append("| Metric    | Coverage |")
lines.append("|-----------|----------|")

total_func = format_entry(data["function_percent"], data["function_covered"], data["function_total"])
total_line = format_entry(data["line_percent"], data["line_covered"], data["line_total"])

lines.append(f"| Functions | {total_func} |")
lines.append(f"| Lines     | {total_line} |")

with open("COVERAGE_TABLE.md", "w") as f:
    f.write("\n".join(lines))
    f.write("\n")
