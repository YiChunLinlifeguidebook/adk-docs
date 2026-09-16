#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
OUT=$(mktemp -d "${TMPDIR:-/tmp}/wukong-public-evaluator.XXXXXX")
trap 'rm -rf "$OUT"' EXIT HUP INT TERM

cd "$ROOT"

find . -type f | sed 's#^./##' | sort > "$OUT/actual.txt"
sort ALLOWLIST.txt > "$OUT/allow.txt"
if ! cmp -s "$OUT/actual.txt" "$OUT/allow.txt"; then
    echo "PUBLIC_ALLOWLIST=FAIL"
    diff -u "$OUT/allow.txt" "$OUT/actual.txt" || true
    exit 1
fi
echo "PUBLIC_ALLOWLIST=PASS"

if grep -R -nE '(BEGIN [A-Z ]*PRIVATE KEY|gh[pousr]_[A-Za-z0-9_]{20,}|sk-[A-Za-z0-9_-]{20,})' .; then
    echo "PUBLIC_SECRET_SCAN=FAIL"
    exit 1
fi
echo "PUBLIC_SECRET_SCAN=PASS"

if grep -R -nE '(^|/)(src/550|modules/controller_gate02|modules/external_runtime|PMJG|WAL)(/|$)' .; then
    echo "PUBLIC_CORE_PATH_SCAN=FAIL"
    exit 1
fi
echo "PUBLIC_CORE_PATH_SCAN=PASS"

CC=${CC:-cc}
CFLAGS='-std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Wundef -Wstrict-prototypes -Wmissing-prototypes -Werror -O2'

$CC $CFLAGS -Iinclude     tests/test_wukong_public_evaluator.c     src/wukong_public_evaluator_contract.c     -o "$OUT/test_public_evaluator"

"$OUT/test_public_evaluator" | tee "$OUT/test.log"

grep -q '^PUBLIC_EVALUATOR_CASES=13$' "$OUT/test.log"
grep -q '^PUBLIC_EVALUATOR_PASS=13$' "$OUT/test.log"
grep -q '^PUBLIC_EVALUATOR_FAIL=0$' "$OUT/test.log"
grep -q '^PUBLIC_EVALUATOR_CORE_SOURCE_INCLUDED=0$' "$OUT/test.log"
grep -q '^PUBLIC_EVALUATOR_SECRET_REQUIRED=0$' "$OUT/test.log"
grep -q '^PUBLIC_EVALUATOR_PROVIDER_FINAL_AUTHORITY=0$' "$OUT/test.log"
grep -q '^SANITIZED_PUBLIC_EVALUATOR_GATE=PASS$' "$OUT/test.log"

sha256sum $(cat ALLOWLIST.txt) | sort > "$OUT/SHA256SUMS.txt"
cat "$OUT/SHA256SUMS.txt"

echo "PUBLIC_SHA256_MANIFEST=PASS"
echo "SANITIZED_PUBLIC_EVALUATOR_VERIFY=PASS"
