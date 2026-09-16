#include "wukong_public_post_return_contract.h"

wk_public_post_verdict_t wk_public_post_return_verify(
    const wk_public_post_return_receipt_t *r)
{
    if (r == 0 || r->abi_version != WK_PUBLIC_POST_RETURN_ABI_VERSION)
        return WK_PUBLIC_POST_REJECT;

    if (r->provider_final_attempt != 0u ||
        r->dinghai_final_attempt != 0u ||
        r->tr07_final_attempt != 0u ||
        r->post_bypass_final_attempt != 0u)
        return WK_PUBLIC_POST_REJECT;

    if (r->gate02_accepted != 1u ||
        r->evidence_valid != 1u ||
        r->lineage_valid != 1u ||
        r->dinghai_checked != 1u ||
        r->sanqing_return_admitted != 1u ||
        r->jiugong_post_entered != 1u)
        return WK_PUBLIC_POST_REJECT;

    /*
     * Public evaluator intentionally models the current fail-closed boundary:
     * external provenance is not promoted by this surface.
     */
    if (r->external_provenance_promoted != 0u)
        return WK_PUBLIC_POST_REJECT;

    if (r->jiugong_finalized != 0u ||
        r->release_final_emitted != 0u)
        return WK_PUBLIC_POST_REJECT;

    return WK_PUBLIC_POST_PROVENANCE_HOLD;
}
