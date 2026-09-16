# Public scope

This directory is a sanitized evaluator surface.

It intentionally contains:
- a small public receipt schema,
- a verifier for safety invariants,
- synthetic test vectors,
- reproducible public CI,
- non-secret private source identity metadata: gate name, HEAD, TREE, additive-file blob IDs, HOLD status, and claim boundary.

It intentionally does not contain:
- private Wukong engine source bytes,
- production runtime source,
- private memory or evidence payloads,
- credentials or tokens,
- private deployment configuration,
- model-provider secrets,
- private repository history.

Passing this public gate proves only the public evaluator contract and receipt integrity.
It does not prove private runtime R9 or Production readiness.
