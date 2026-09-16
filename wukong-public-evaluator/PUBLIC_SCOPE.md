# Public scope

This directory is a sanitized evaluator surface.

It intentionally contains:
- a small public receipt schema,
- a verifier for safety invariants,
- synthetic test vectors,
- reproducible public CI.

It intentionally does not contain:
- private Wukong engine source,
- production runtime source,
- private memory or evidence,
- credentials or tokens,
- private deployment configuration,
- model-provider secrets,
- private repository history.

Passing this public gate proves only the public evaluator contract.
It does not prove private runtime R9 or Production readiness.
