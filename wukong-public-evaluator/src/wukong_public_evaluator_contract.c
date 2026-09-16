#include "wukong_public_evaluator_contract.h"

wk_public_verdict_t wk_public_evaluator_verify(
    const wk_public_receipt_t *r)
{
    if (r == 0 || r->abi_version != WK_PUBLIC_EVALUATOR_ABI_VERSION)
        return WK_PUBLIC_VERDICT_REJECT;

    if (r->final_authority != 0u || r->direct_final_attempt != 0u)
        return WK_PUBLIC_VERDICT_REJECT;

    switch ((wk_public_route_t)r->route) {
    case WK_PUBLIC_ROUTE_NATIVE:
        if (r->provider_call_count != 0u ||
            r->external_call_count != 0u ||
            r->proposal_present != 0u)
            return WK_PUBLIC_VERDICT_REJECT;
        return WK_PUBLIC_VERDICT_NATIVE_OK;

    case WK_PUBLIC_ROUTE_LLM:
        if (r->provider_call_count != 1u ||
            r->external_call_count != 0u ||
            r->proposal_present != 1u ||
            r->evidence_present != 1u ||
            r->proposal_digest_ok != 1u ||
            r->evidence_digest_ok != 1u ||
            r->provider_identity_ok != 1u ||
            r->lineage_ok != 1u)
            return WK_PUBLIC_VERDICT_REJECT;
        return WK_PUBLIC_VERDICT_PROPOSAL_ONLY;

    case WK_PUBLIC_ROUTE_EXTERNAL_TOOL:
        if (r->provider_call_count != 0u ||
            r->proposal_present != 0u)
            return WK_PUBLIC_VERDICT_REJECT;
        return WK_PUBLIC_VERDICT_EXTERNAL_DISPATCH;

    case WK_PUBLIC_ROUTE_HOLD:
        if (r->provider_call_count != 0u ||
            r->external_call_count != 0u ||
            r->proposal_present != 0u)
            return WK_PUBLIC_VERDICT_REJECT;
        return WK_PUBLIC_VERDICT_HOLD;

    default:
        return WK_PUBLIC_VERDICT_REJECT;
    }
}

const char *wk_public_verdict_name(wk_public_verdict_t verdict)
{
    switch (verdict) {
    case WK_PUBLIC_VERDICT_NATIVE_OK:
        return "NATIVE_OK";
    case WK_PUBLIC_VERDICT_PROPOSAL_ONLY:
        return "PROPOSAL_ONLY";
    case WK_PUBLIC_VERDICT_EXTERNAL_DISPATCH:
        return "EXTERNAL_DISPATCH";
    case WK_PUBLIC_VERDICT_HOLD:
        return "HOLD";
    case WK_PUBLIC_VERDICT_REJECT:
    default:
        return "REJECT";
    }
}
