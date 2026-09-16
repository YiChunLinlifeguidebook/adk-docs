#ifndef WUKONG_PUBLIC_EVALUATOR_CONTRACT_H
#define WUKONG_PUBLIC_EVALUATOR_CONTRACT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WK_PUBLIC_EVALUATOR_ABI_VERSION 1u

typedef enum wk_public_route {
    WK_PUBLIC_ROUTE_HOLD = 0,
    WK_PUBLIC_ROUTE_NATIVE = 1,
    WK_PUBLIC_ROUTE_LLM = 2,
    WK_PUBLIC_ROUTE_EXTERNAL_TOOL = 3
} wk_public_route_t;

typedef enum wk_public_verdict {
    WK_PUBLIC_VERDICT_REJECT = 0,
    WK_PUBLIC_VERDICT_NATIVE_OK = 1,
    WK_PUBLIC_VERDICT_PROPOSAL_ONLY = 2,
    WK_PUBLIC_VERDICT_EXTERNAL_DISPATCH = 3,
    WK_PUBLIC_VERDICT_HOLD = 4
} wk_public_verdict_t;

typedef struct wk_public_receipt {
    uint32_t abi_version;
    uint8_t route;
    uint8_t proposal_present;
    uint8_t evidence_present;
    uint8_t proposal_digest_ok;
    uint8_t evidence_digest_ok;
    uint8_t provider_identity_ok;
    uint8_t lineage_ok;
    uint8_t final_authority;
    uint8_t direct_final_attempt;
    uint32_t provider_call_count;
    uint32_t external_call_count;
} wk_public_receipt_t;

wk_public_verdict_t wk_public_evaluator_verify(
    const wk_public_receipt_t *receipt);

const char *wk_public_verdict_name(wk_public_verdict_t verdict);

#ifdef __cplusplus
}
#endif

#endif
