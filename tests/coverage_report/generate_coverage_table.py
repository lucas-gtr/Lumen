# generate_coverage_table.py

import json

with open("tests/coverage_report/coverage_report.json") as f:
    data = json.load(f)

def format_entry(percent, covered, total):
    return f"{percent:.0f}% ({covered}/{total})"

lines = []
lines.append("| File | Functions | Lines | Branches |")
lines.append("|------|-----------|-------|----------|")

for file in data["files"]:
    filename = file["filename"]
    func = format_entry(file["function_percent"], file["function_covered"], file["function_total"])
    line = format_entry(file["line_percent"], file["line_covered"], file["line_total"])
    branch = format_entry(file["branch_percent"], file["branch_covered"], file["branch_total"])
    lines.append(f"| `{filename}` | {func} | {line} | {branch} |")

total_func = format_entry(data["function_percent"], data["function_covered"], data["function_total"])
total_line = format_entry(data["line_percent"], data["line_covered"], data["line_total"])
total_branch = format_entry(data["branch_percent"], data["branch_covered"], data["branch_total"])
lines.append(f"| **Total** | **{total_func}** | **{total_line}** | **{total_branch}** |")

with open("COVERAGE_TABLE.md", "w") as f:
    f.write("### Code Coverage Summary\n\n")
    f.write("\n".join(lines))
    f.write("\n")
