# Wukong Sanitized Public Evaluator

Purpose: provide a small, independently runnable public verification surface without publishing the private Wukong engine.

The evaluator checks public safety invariants:
- a model/provider cannot grant itself final authority;
- LLM results require proposal + evidence + digest + identity + lineage validation;
- Native, External Tool and HOLD routes reject silent model fallback;
- direct-final attempts are rejected;
- invalid or incomplete receipts fail closed;
- an external provider return can enter post-return governance without being auto-promoted;
- provider, QC, translation or bypass layers cannot self-finalize;
- release remains closed when external provenance is not promoted.

## Run locally

```sh
sh scripts/verify_public_evaluator.sh
```

Expected terminal markers include:

```text
SANITIZED_PUBLIC_EVALUATOR_GATE=PASS
SANITIZED_PUBLIC_POST_RETURN_GATE=PASS
PUBLIC_TOTAL_PASS=23
SANITIZED_PUBLIC_EVALUATOR_VERIFY=PASS
```

## Claim boundary

This is not the private Wukong runtime and does not contain its core source.
A PASS here proves only that the published evaluator contracts and synthetic vectors satisfy the stated public invariants.
It does not prove private R9 runtime or Production readiness.
