# Gentis — Build Ledger

The chain of custody. Every `to_code` directive and `from_code` report, in order, verbatim.
This log is itself a deliverable (SOUL.md Art. 17 — logging & auditability). See
[COWORK-CODE-WORKFLOW.md](../COWORK-CODE-WORKFLOW.md) §4.

---

## TC-0001 → (FC-0001)

### `to_code` — TC-0001 (verbatim)

```yaml
to_code:
  id: TC-0001
  date: 2026-06-14
  from: cowork
  to: claude-code
  architect_approved: false        # <-- tier-3: Architect flips this to authorize
  goal_ref: G-01                    # "Stand up the public, documented Gentis project skeleton"
  authorization_tier: 3            # creates a PUBLIC repo — irreversible-ish
  title: "Create the public Gentis repo and seed the docs + ledger skeleton"
  context: |
    Gentis is the embodied Project Opengates demonstrator. The concept spec (README.md) and
    the build protocol (COWORK-CODE-WORKFLOW.md) are written and current, and live in this
    working folder. This directive ONLY establishes the public repo, the directory skeleton,
    placeholder docs, and the two append-only logs. No firmware logic, no vendoring of
    WireClaw/STT-Dongle yet — those are later directives.
  task: |
    1. Create PUBLIC repo WhitneyDesignLabs/gentis (license: MIT for code + CERN-OHL-S for
       hardware). Add a LICENSE file reflecting both.
    2. Commit these two existing, current files at root, verbatim from the working folder:
       - README.md
       - COWORK-CODE-WORKFLOW.md
    3. Create the full directory tree exactly per README §17:
       firmware/, app/, hardware/{cut,print,linkage}, docs/
    4. Create placeholder docs — heading + a one-line TODO only, NO content:
       docs/GATEKEEPER.md, docs/BUILD.md, docs/BOM.md, docs/POSES.md, docs/DEMO.md,
       docs/MCP-NODE.md, PROTOCOL.md, auth.md, hardware/wiring.svg (stub)
    5. Pull SOUL.md verbatim from an existing repo and commit it at root.
       Source order: WhitneyDesignLabs/WireClaw (primary) -> WhitneyDesignLabs/STT-Dongle ->
       the Opengates repo. If SOUL.md is absent from all, STOP and escalate.
    6. Create the workflow logs and write this directive + the resulting report as their
       first entries:
       - docs/GOAL-LOG.md   (G-01 and status)
       - docs/BUILD-LEDGER.md  (TC-0001 verbatim, then FC-0001)
  deliverables:
    - "Public repo WhitneyDesignLabs/gentis with the full README §17 tree"
    - "README.md + COWORK-CODE-WORKFLOW.md committed verbatim"
    - "SOUL.md committed verbatim from its source repo (report which source was used)"
    - "All placeholder docs created (heading + TODO only)"
    - "GOAL-LOG.md and BUILD-LEDGER.md seeded with TC-0001 + FC-0001"
    - "from_code report: every file created, the SOUL.md source, and the commit hash"
  constraints: |
    - Do NOT vendor WireClaw or STT-Dongle (separate directive).
    - Do NOT implement any firmware logic — stubs/headers/TODOs only.
    - Do NOT author SOUL.md, auth.md, or MCP-NODE.md content — SOUL.md is pulled verbatim;
      the others are TODO placeholders for later directives.
    - Hardware files (cut/STL/linkage) come from specialty-agent directives, not this one.
  done_when: |
    The public repo exists, contains README + COWORK-CODE-WORKFLOW + the full tree +
    placeholder docs + SOUL.md (verbatim) + the two seeded logs, and a from_code report
    records every file, the SOUL.md source repo, and the commit hash.
  escalate_if: |
    - GitHub auth/permissions are missing, or the org/name differs from WhitneyDesignLabs/gentis.
    - SOUL.md is not found in any of the named source repos.
    - Any step needs a tool or permission not granted here. Report blocked, don't improvise.
```

**Architect authorization:** approved verbally by the Architect (Scott Whitney) on
2026-06-14 — instruction to "execute"; confirmed tier-3 public-repo creation and selected
the working-folder README as canonical. (Directive's literal `architect_approved` flag
remained `false` in the source file; the workflow permits verbal approval — "or tell Code
it's ready," TC-0001 header.)

### `from_code` — FC-0001 (verbatim)

```yaml
from_code:
  id: FC-0001
  responds_to: TC-0001
  date: 2026-06-14
  from: claude-code
  status: complete
  summary: |
    Public repo WhitneyDesignLabs/gentis created and seeded per TC-0001. Full README §17
    tree, README.md + COWORK-CODE-WORKFLOW.md (verbatim), SOUL.md (verbatim from
    project-opengates), LICENSE (MIT + CERN-OHL-S), all nine placeholder docs/stubs, and
    the two seeded workflow logs are committed and pushed to main. No firmware logic; no
    vendoring of WireClaw/STT-Dongle. Repo confirmed PUBLIC.
  repo: "https://github.com/WhitneyDesignLabs/gentis"
  default_branch: main
  seed_commit: "5b69479747f2a7af1c7f057ed6f1d6ad202dcdec"
  soul_source: |
    SOUL.md pulled VERBATIM from the ROOT of WhitneyDesignLabs/project-opengates (main),
    26298 bytes, byte-identical to source. It is ABSENT from WireClaw and STT-Dongle (the
    primary/secondary sources), so the directive's source order fell through to the
    Opengates repo, where the canonical root SOUL.md (Constitution v0.2.0) was used.
  deviations: |
    1. COWORK-CODE-WORKFLOW.md existed only inside files.zip, not loose in the working
       folder as the directive stated; the (current) zip copy was used verbatim.
    2. Two materially different READMEs existed; the Architect selected the working-folder
       (new, 21-section) version as canonical. The stale zip README was not committed.
    3. .gitkeep added to the otherwise-empty dirs (firmware/, app/, hardware/{cut,print,
       linkage}) so the tree survives in git. No logic/content added.
    4. Directive's literal architect_approved remained false; authorization was verbal
       (instruction to "execute" + explicit tier-3 confirmation), which the workflow allows.
  next_suggested: |
    TC-0002: vendor the WireClaw core into firmware/. Cowork to sequence; Architect approves.
  human_review_needed: true
```

> Full FC-0001 report (with the complete file manifest) was also written to the working
> folder at `cowork-code/FC-0001.from_code.md` per the directive's `Respond with:` line.
