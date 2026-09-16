#ifndef WUKONG_PUBLIC_POST_RETURN_CONTRACT_H
#define WUKONG_PUBLIC_POST_RETURN_CONTRACT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WK_PUBLIC_POST_RETURN_ABI_VERSION 1u

typedef enum wk_public_post_verdict {
    WK_PUBLIC_POST_REJECT = 0,
    WK_PUBLIC_POST_PROVENANCE_HOLD = 1,
    WK_PUBLIC_POST_RELEASE_HOLD = 2
} wk_public_post_verdict_t;

typedef struct wk_public_post_return_receipt {
    uint32_t abi_version;
    uint8_t gate02_accepted;
    uint8_t evidence_valid;
    uint8_t lineage_valid;
    uint8_t dinghai_checked;
    uint8_t sanqing_return_admitted;
    uint8_t external_provenance_promoted;
    uint8_t provider_final_attempt;
    uint8_t dinghai_final_attempt;
    uint8_t tr07_final_attempt;
    uint8_t post_bypass_final_attempt;
    uint8_t jiugong_post_entered;
    uint8_t jiugong_finalized;
    uint8_t release_final_emitted;
} wk_public_post_return_receipt_t;

wk_public_post_verdict_t wk_public_post_return_verify(
    const wk_public_post_return_receipt_t *receipt);

#ifdef __cplusplus
}
#endif

#endif
