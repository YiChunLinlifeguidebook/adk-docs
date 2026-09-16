#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
RECEIPT="$ROOT/receipts/private_r9_identity_receipt.tsv"

test -f "$RECEIPT"

grep -q '^receipt_version[[:space:]]1$' "$RECEIPT"
grep -q '^private_gate[[:space:]]R9_PROVIDER_RESULT_TO_DINGHAI_JIUGONG_POST_CLOSED_LOOP$' "$RECEIPT"
grep -q '^runtime_status[[:space:]]HOLD_PRIVATE_REPO_ACTIONS_EXECUTION$' "$RECEIPT"
grep -q '^runtime_pass[[:space:]]false$' "$RECEIPT"
grep -q '^source_failure_proven[[:space:]]false$' "$RECEIPT"
grep -q '^private_core_included[[:space:]]0$' "$RECEIPT"
grep -q '^blob_count[[:space:]]5$' "$RECEIPT"
grep -q '^claim_boundary[[:space:]]IDENTITY_RECEIPT_ONLY_NOT_PRIVATE_RUNTIME_PASS$' "$RECEIPT"

awk -F '\t' '
$1=="private_head" || $1=="private_tree" || $1=="base_r8_head" {
    if ($2 !~ /^[0-9a-f]{40}$/) exit 1
}
$1=="blob" {
    count++
    if ($3 !~ /^[0-9a-f]{40}$/) exit 1
}
END {
    if (count != 5) exit 1
}
' "$RECEIPT"

grep -q '^blob[[:space:]]\.github/workflows/wukong-r9-provider-return-post.yml[[:space:]]afd341e1a5c98e6dc9c9850dc4be41e30aacb01a$' "$RECEIPT"
grep -q '^blob[[:space:]]evaluation/r9/provider_return_post_seam.c[[:space:]]5b87932906e8e7d657ae59f982f2f2c1dfac743a$' "$RECEIPT"
grep -q '^blob[[:space:]]evaluation/r9/provider_return_post_seam.h[[:space:]]d26e5da0bf69265760973b25a9aec9349393d9fa$' "$RECEIPT"
grep -q '^blob[[:space:]]scripts/wukong_r9_provider_return_post.sh[[:space:]]1e9bb5565189468afcd7e3ca66365189a4a0813d$' "$RECEIPT"
grep -q '^blob[[:space:]]tests/550/hybrid_r9/test_provider_return_post_r9.c[[:space:]]ff59660e3297b9d8754e227ae9c10c4da13eee1a$' "$RECEIPT"

sha256sum "$RECEIPT"

echo "PUBLIC_PRIVATE_RECEIPT_BLOB_COUNT=5"
echo "PUBLIC_PRIVATE_RECEIPT_RUNTIME_PASS=0"
echo "PUBLIC_PRIVATE_RECEIPT_SOURCE_FAILURE_PROVEN=0"
echo "PUBLIC_PRIVATE_RECEIPT_CORE_INCLUDED=0"
echo "PUBLIC_PRIVATE_RECEIPT_BRIDGE=PASS"
