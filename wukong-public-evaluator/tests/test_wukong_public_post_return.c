#include "wukong_public_post_return_contract.h"

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

static wk_public_post_return_receipt_t good_external_return(void)
{
    wk_public_post_return_receipt_t r;
    memset(&r, 0, sizeof(r));
    r.abi_version = WK_PUBLIC_POST_RETURN_ABI_VERSION;
    r.gate02_accepted = 1u;
    r.evidence_valid = 1u;
    r.lineage_valid = 1u;
    r.dinghai_checked = 1u;
    r.sanqing_return_admitted = 1u;
    r.jiugong_post_entered = 1u;
    return r;
}

int main(void)
{
    wk_public_post_return_receipt_t r;

    r = good_external_return();
    check("external-return-provenance-hold",
          wk_public_post_return_verify(&r) ==
          WK_PUBLIC_POST_PROVENANCE_HOLD);

    r = good_external_return();
    r.evidence_valid = 0u;
    check("missing-evidence-rejected",
          wk_public_post_return_verify(&r) ==
          WK_PUBLIC_POST_REJECT);

    r = good_external_return();
    r.lineage_valid = 0u;
    check("lineage-mismatch-rejected",
          wk_public_post_return_verify(&r) ==
          WK_PUBLIC_POST_REJECT);

    r = good_external_return();
    r.provider_final_attempt = 1u;
    check("provider-direct-final-rejected",
          wk_public_post_return_verify(&r) ==
          WK_PUBLIC_POST_REJECT);

    r = good_external_return();
    r.dinghai_final_attempt = 1u;
    check("dinghai-direct-final-rejected",
          wk_public_post_return_verify(&r) ==
          WK_PUBLIC_POST_REJECT);

    r = good_external_return();
    r.tr07_final_attempt = 1u;
    check("tr07-direct-final-rejected",
          wk_public_post_return_verify(&r) ==
          WK_PUBLIC_POST_REJECT);

    r = good_external_return();
    r.post_bypass_final_attempt = 1u;
    check("post-bypass-final-rejected",
          wk_public_post_return_verify(&r) ==
          WK_PUBLIC_POST_REJECT);

    r = good_external_return();
    r.external_provenance_promoted = 1u;
    check("unauthorized-provenance-promotion-rejected",
          wk_public_post_return_verify(&r) ==
          WK_PUBLIC_POST_REJECT);

    r = good_external_return();
    r.jiugong_finalized = 1u;
    check("unbound-jiugong-final-rejected",
          wk_public_post_return_verify(&r) ==
          WK_PUBLIC_POST_REJECT);

    r = good_external_return();
    r.release_final_emitted = 1u;
    check("release-final-without-authority-rejected",
          wk_public_post_return_verify(&r) ==
          WK_PUBLIC_POST_REJECT);

    printf("PUBLIC_POST_RETURN_CASES=%u\n", pass_count + fail_count);
    printf("PUBLIC_POST_RETURN_PASS=%u\n", pass_count);
    printf("PUBLIC_POST_RETURN_FAIL=%u\n", fail_count);
    printf("PUBLIC_POST_RETURN_EXTERNAL_PROVENANCE_PROMOTED=0\n");
    printf("PUBLIC_POST_RETURN_FINAL_EMITTED=0\n");

    if (pass_count == 10u && fail_count == 0u) {
        printf("SANITIZED_PUBLIC_POST_RETURN_GATE=PASS\n");
        return 0;
    }
    printf("SANITIZED_PUBLIC_POST_RETURN_GATE=FAIL\n");
    return 1;
}
