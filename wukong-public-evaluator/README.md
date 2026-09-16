# Wukong Sanitized Public Evaluator

Purpose: provide a small, independently runnable public verification surface without publishing the private Wukong engine.

The evaluator checks public safety invariants:
- a model/provider cannot grant itself final authority;
- LLM results require proposal + evidence + digest + identity + lineage validation;
- Native, External Tool and HOLD routes reject silent model fallback;
- direct-final attempts are rejected;
- invalid or incomplete receipts fail closed.

## Run locally

```sh
sh scripts/verify_public_evaluator.sh
```

Expected terminal marker:

```text
SANITIZED_PUBLIC_EVALUATOR_GATE=PASS
```

## Claim boundary

This is not the private Wukong runtime and does not contain its core source.
A PASS here proves only that the published evaluator contract and synthetic vectors satisfy the stated public invariants.
