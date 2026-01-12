#!/usr/bin/env bash

# Count the total number of lines in all .cpp and .hpp files, skipping build outputs.
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Directories to ignore during the search.
EXCLUDES=(
	"$ROOT_DIR/build"
	"$ROOT_DIR/assets"
	"$ROOT_DIR/docs"
	"$ROOT_DIR/.vcpkg"
	"$ROOT_DIR/.git"
	"$ROOT_DIR/vcpkg_installed"
)

# Build the find command safely, handling spaces in paths.
find_cmd=(find "$ROOT_DIR")

if [[ ${#EXCLUDES[@]} -gt 0 ]]; then
	find_cmd+=(
		\(
	)
	for idx in "${!EXCLUDES[@]}"; do
		find_cmd+=( -path "${EXCLUDES[$idx]}" )
		if [[ $idx -lt $((${#EXCLUDES[@]} - 1)) ]]; then
			find_cmd+=( -o )
		fi
	done
	find_cmd+=( \) -prune -o )
fi

find_cmd+=( -type f \( -name '*.cpp' -o -name '*.hpp' \) -print0 )

mapfile -d '' files < <("${find_cmd[@]}")

if [[ ${#files[@]} -eq 0 ]]; then
	echo "No .cpp or .hpp files found."
	exit 0
fi

total=0
for file in "${files[@]}"; do
	lines=$(wc -l < "$file")
	printf "%6d %s\n" "$lines" "${file#$ROOT_DIR/}"
	total=$((total + lines))
done

echo "------"
echo "Total lines: $total"
