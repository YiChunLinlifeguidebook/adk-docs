#include "wukong_public_evaluator_contract.h"

#include <stdio.h>
#include <string.h>

static unsigned pass_count;
static unsigned fail_count;

static void check(const char *name, int ok)
{
    if (ok) {
        ++pass_count;
        printf("PASS %s\n", name);
    } else {
        ++fail_count;
        printf("FAIL %s\n", name);
    }
}

static wk_public_receipt_t base_receipt(uint8_t route)
{
    wk_public_receipt_t r;
    memset(&r, 0, sizeof(r));
    r.abi_version = WK_PUBLIC_EVALUATOR_ABI_VERSION;
    r.route = route;
    return r;
}

int main(void)
{
    wk_public_receipt_t r;

    r = base_receipt(WK_PUBLIC_ROUTE_NATIVE);
    check("native-no-provider",
          wk_public_evaluator_verify(&r) ==
          WK_PUBLIC_VERDICT_NATIVE_OK);

    r = base_receipt(WK_PUBLIC_ROUTE_LLM);
    r.provider_call_count = 1u;
    r.proposal_present = 1u;
    r.evidence_present = 1u;
    r.proposal_digest_ok = 1u;
    r.evidence_digest_ok = 1u;
    r.provider_identity_ok = 1u;
    r.lineage_ok = 1u;
    check("llm-proposal-only",
          wk_public_evaluator_verify(&r) ==
          WK_PUBLIC_VERDICT_PROPOSAL_ONLY);

    r = base_receipt(WK_PUBLIC_ROUTE_EXTERNAL_TOOL);
    r.external_call_count = 1u;
    check("external-dispatch",
          wk_public_evaluator_verify(&r) ==
          WK_PUBLIC_VERDICT_EXTERNAL_DISPATCH);

    r = base_receipt(WK_PUBLIC_ROUTE_HOLD);
    check("hold-no-calls",
          wk_public_evaluator_verify(&r) ==
          WK_PUBLIC_VERDICT_HOLD);

    r = base_receipt(WK_PUBLIC_ROUTE_LLM);
    r.provider_call_count = 1u;
    r.proposal_present = 1u;
    r.proposal_digest_ok = 1u;
    r.provider_identity_ok = 1u;
    r.lineage_ok = 1u;
    check("missing-evidence-rejected",
          wk_public_evaluator_verify(&r) ==
          WK_PUBLIC_VERDICT_REJECT);

    r = base_receipt(WK_PUBLIC_ROUTE_LLM);
    r.provider_call_count = 1u;
    r.proposal_present = 1u;
    r.evidence_present = 1u;
    r.proposal_digest_ok = 0u;
    r.evidence_digest_ok = 1u;
    r.provider_identity_ok = 1u;
    r.lineage_ok = 1u;
    check("proposal-tamper-rejected",
          wk_public_evaluator_verify(&r) ==
          WK_PUBLIC_VERDICT_REJECT);

    r = base_receipt(WK_PUBLIC_ROUTE_LLM);
    r.provider_call_count = 1u;
    r.proposal_present = 1u;
    r.evidence_present = 1u;
    r.proposal_digest_ok = 1u;
    r.evidence_digest_ok = 1u;
    r.provider_identity_ok = 0u;
    r.lineage_ok = 1u;
    check("identity-mismatch-rejected",
          wk_public_evaluator_verify(&r) ==
          WK_PUBLIC_VERDICT_REJECT);

    r = base_receipt(WK_PUBLIC_ROUTE_LLM);
    r.provider_call_count = 1u;
    r.proposal_present = 1u;
    r.evidence_present = 1u;
    r.proposal_digest_ok = 1u;
    r.evidence_digest_ok = 1u;
    r.provider_identity_ok = 1u;
    r.lineage_ok = 0u;
    check("lineage-mismatch-rejected",
          wk_public_evaluator_verify(&r) ==
          WK_PUBLIC_VERDICT_REJECT);

    r = base_receipt(WK_PUBLIC_ROUTE_LLM);
    r.provider_call_count = 1u;
    r.proposal_present = 1u;
    r.evidence_present = 1u;
    r.proposal_digest_ok = 1u;
    r.evidence_digest_ok = 1u;
    r.provider_identity_ok = 1u;
    r.lineage_ok = 1u;
    r.final_authority = 1u;
    check("provider-final-authority-rejected",
          wk_public_evaluator_verify(&r) ==
          WK_PUBLIC_VERDICT_REJECT);

    r = base_receipt(WK_PUBLIC_ROUTE_NATIVE);
    r.provider_call_count = 1u;
    check("native-model-fallback-rejected",
          wk_public_evaluator_verify(&r) ==
          WK_PUBLIC_VERDICT_REJECT);

    r = base_receipt(WK_PUBLIC_ROUTE_EXTERNAL_TOOL);
    r.provider_call_count = 1u;
    check("external-model-fallback-rejected",
          wk_public_evaluator_verify(&r) ==
          WK_PUBLIC_VERDICT_REJECT);

    r = base_receipt(WK_PUBLIC_ROUTE_HOLD);
    r.provider_call_count = 1u;
    check("hold-model-fallback-rejected",
          wk_public_evaluator_verify(&r) ==
          WK_PUBLIC_VERDICT_REJECT);

    r = base_receipt(WK_PUBLIC_ROUTE_LLM);
    r.direct_final_attempt = 1u;
    check("direct-final-rejected",
          wk_public_evaluator_verify(&r) ==
          WK_PUBLIC_VERDICT_REJECT);

    printf("PUBLIC_EVALUATOR_CASES=%u\n", pass_count + fail_count);
    printf("PUBLIC_EVALUATOR_PASS=%u\n", pass_count);
    printf("PUBLIC_EVALUATOR_FAIL=%u\n", fail_count);
    printf("PUBLIC_EVALUATOR_CORE_SOURCE_INCLUDED=0\n");
    printf("PUBLIC_EVALUATOR_SECRET_REQUIRED=0\n");
    printf("PUBLIC_EVALUATOR_PROVIDER_FINAL_AUTHORITY=0\n");

    if (fail_count == 0u && pass_count == 13u) {
        printf("SANITIZED_PUBLIC_EVALUATOR_GATE=PASS\n");
        return 0;
    }
    printf("SANITIZED_PUBLIC_EVALUATOR_GATE=FAIL\n");
    return 1;
}
