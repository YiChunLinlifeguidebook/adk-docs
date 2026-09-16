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

"$OUT/test_public_evaluator" | tee "$OUT/route.log"

grep -q '^PUBLIC_EVALUATOR_CASES=13$' "$OUT/route.log"
grep -q '^PUBLIC_EVALUATOR_PASS=13$' "$OUT/route.log"
grep -q '^PUBLIC_EVALUATOR_FAIL=0$' "$OUT/route.log"
grep -q '^PUBLIC_EVALUATOR_CORE_SOURCE_INCLUDED=0$' "$OUT/route.log"
grep -q '^PUBLIC_EVALUATOR_SECRET_REQUIRED=0$' "$OUT/route.log"
grep -q '^PUBLIC_EVALUATOR_PROVIDER_FINAL_AUTHORITY=0$' "$OUT/route.log"
grep -q '^SANITIZED_PUBLIC_EVALUATOR_GATE=PASS$' "$OUT/route.log"

$CC $CFLAGS -Iinclude     tests/test_wukong_public_post_return.c     src/wukong_public_post_return_contract.c     -o "$OUT/test_public_post_return"

"$OUT/test_public_post_return" | tee "$OUT/post.log"

grep -q '^PUBLIC_POST_RETURN_CASES=10$' "$OUT/post.log"
grep -q '^PUBLIC_POST_RETURN_PASS=10$' "$OUT/post.log"
grep -q '^PUBLIC_POST_RETURN_FAIL=0$' "$OUT/post.log"
grep -q '^PUBLIC_POST_RETURN_EXTERNAL_PROVENANCE_PROMOTED=0$' "$OUT/post.log"
grep -q '^PUBLIC_POST_RETURN_FINAL_EMITTED=0$' "$OUT/post.log"
grep -q '^SANITIZED_PUBLIC_POST_RETURN_GATE=PASS$' "$OUT/post.log"

sh scripts/verify_private_receipt_bridge.sh | tee "$OUT/receipt.log"

grep -q '^PUBLIC_PRIVATE_RECEIPT_BLOB_COUNT=5$' "$OUT/receipt.log"
grep -q '^PUBLIC_PRIVATE_RECEIPT_RUNTIME_PASS=0$' "$OUT/receipt.log"
grep -q '^PUBLIC_PRIVATE_RECEIPT_SOURCE_FAILURE_PROVEN=0$' "$OUT/receipt.log"
grep -q '^PUBLIC_PRIVATE_RECEIPT_CORE_INCLUDED=0$' "$OUT/receipt.log"
grep -q '^PUBLIC_PRIVATE_RECEIPT_BRIDGE=PASS$' "$OUT/receipt.log"

cat "$OUT/route.log"
cat "$OUT/post.log"
cat "$OUT/receipt.log"

sha256sum $(cat ALLOWLIST.txt) | sort > "$OUT/SHA256SUMS.txt"
cat "$OUT/SHA256SUMS.txt"

echo "PUBLIC_SHA256_MANIFEST=PASS"
echo "PUBLIC_TOTAL_CASES=23"
echo "PUBLIC_TOTAL_PASS=23"
echo "PUBLIC_TOTAL_FAIL=0"
echo "PUBLIC_PRIVATE_RECEIPT_BRIDGE=PASS"
echo "SANITIZED_PUBLIC_EVALUATOR_VERIFY=PASS"
