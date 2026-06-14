# Cowork ⇄ Code — Orchestrated Build Workflow

**Project:** Gentis (Whitney Design Labs / Project Opengates)
**Status:** v0.1 process spec
**Purpose:** Define how a human architect, an orchestrating agent (Claude Cowork), and
one or more executing agents (Claude Code, plus specialty agents) collaborate to build
Gentis — with every hand-off documented as a durable, reviewable artifact.

---

## 0. Why this exists

Gentis's whole thesis is that **safety lives in the enforcement layer, and the human
retains ultimate authority** (SOUL.md Art. 1, Art. 16). A multi-agent build is only
trustworthy if that same principle governs *how it is built*. So this workflow is the
Gatekeeper idea applied to the development process itself:

- **Cowork** keeps the work aligned to project goals and human intent. It does not write
  production files or touch hardware. It is the *architect's voice* — planning,
  sequencing, reviewing, and keeping scope honest.
- **Code** (and specialty executors) do the heavy lifting at the file, microcontroller,
  phone, and server level. They act *only* on an explicit, written directive.
- **The human** is in the loop at every directive boundary. Nothing irreversible happens
  without a human-visible `to_code` directive and a human-visible `from_code` report.
- **Every turn is documented.** The prompts going each direction are the artifact. The
  log is not a side effect of the work; it *is* a deliverable, equal in value to the code.

This mirrors the runtime architecture exactly: Cowork is the personality/planner, Code is
the actuator, and the written protocol between them is the Gatekeeper — the gate every
action passes through, leaving a record.

---

## 1. Roles

| Role | Who | May do | May NOT do |
|------|-----|--------|------------|
| **Architect** | The human (Scott) | Set goals, approve/deny directives, override anything, change scope | — |
| **Orchestrator** | Claude Cowork | Plan, decompose work, write `to_code` directives, review `from_code` reports, keep the goal-log, surface decisions to the Architect | Write production code, flash hardware, commit to repos, call irreversible tools |
| **Executor** | Claude Code | Create/edit files, run builds, flash MCUs, configure servers, open PRs — strictly per a directive | Expand scope beyond the directive, take undocumented action, self-authorize the next task |
| **Specialty Executor** | e.g. a Fusion 360 mech-design agent, a PCB agent | Domain work per a directive (CAD, layout, sims) | Same constraints as Executor |

**Authorization principle (from SOUL.md Art. 15):** the more irreversible the action, the
more explicit the human checkpoint. Reversible file edits in a branch are low-tier;
flashing firmware, committing to `main`, spending money, or anything touching live
hardware is high-tier and requires explicit Architect confirmation inside the directive.

---

## 2. The artifacts: `to_code` and `from_code`

Two structured message types carry every hand-off. They are written to disk (and/or the
repo) as they happen, in an append-only log, so the full reasoning chain is reconstructable
later. (SOUL.md Art. 17 — logging & auditability.)

### 2.1 `to_code` — a directive from Orchestrator to Executor

A `to_code` block is the *only* thing that authorizes an Executor to act. It is written by
Cowork, reviewed by the Architect, and handed to Code (or a specialty agent).

```yaml
to_code:
  id: TC-0001                      # sequential, unique
  date: 2026-06-14T10:00:00Z
  from: cowork
  to: claude-code                  # or: fusion360-agent, pcb-agent, ...
  architect_approved: true         # MUST be true before Executor acts
  goal_ref: G-01                   # links to the goal-log entry this serves
  authorization_tier: 2            # 1 reversible / 2 significant / 3 critical / 4 irreversible
  title: "Scaffold the Gentis repo and write core docs"
  context: |
    One-paragraph why. What larger goal this serves, what's already done,
    what the Executor needs to know that isn't obvious from the task.
  task: |
    Concrete, bounded instructions. Imperative. No ambiguity about scope.
    1. Create repo structure per README §17.
    2. Add README.md (provided), LICENSE (MIT + CERN-OHL-S), SOUL.md (provided).
    3. Create empty module stubs with header comments only — no logic yet.
  deliverables:
    - "Repo tree matching README §17"
    - "Three docs committed on a branch named scaffold/initial"
  constraints: |
    - Work on a branch. Do NOT push to main.
    - Do NOT implement Gatekeeper logic in this directive (that is TC-0004).
    - If a path in README §17 is ambiguous, STOP and ask — do not guess.
  done_when: |
    The branch exists, builds (or lints) clean, and a from_code report lists
    every file created with one-line descriptions.
  escalate_if: |
    Any step would require a tool or permission not granted here, or any
    action not explicitly listed above.
```

### 2.2 `from_code` — a report from Executor back to Orchestrator

When the Executor finishes (or must stop), it writes a `from_code` block. This is what
Cowork reviews and what the Architect reads to stay on top of the build.

```yaml
from_code:
  id: FC-0001
  responds_to: TC-0001
  date: 2026-06-14T10:18:00Z
  from: claude-code
  status: complete            # complete | blocked | partial | refused
  summary: |
    Repo scaffolded on branch scaffold/initial. 14 files created, 0 logic added.
  actions_taken:
    - "git init; created tree per README §17"
    - "committed README.md, LICENSE, SOUL.md"
    - "added 8 stub modules with header comments"
  deliverables_produced:
    - path: "firmware/src/gatekeeper.cpp"
      note: "stub only — TODO marker for TC-0004"
  deviations: |
    None. (If any: what differed from the directive and why.)
  open_questions: |
    README §6 references /hardware/linkage but no geometry exists yet —
    flagged for a future specialty-agent directive.
  next_suggested: |
    TC-0002: vendor WireClaw core into firmware/. (Cowork to confirm + Architect to approve.)
  human_review_needed: true
```

**Key rule:** an Executor never proceeds from its own `next_suggested`. It proposes; Cowork
sequences; the Architect approves. The loop always returns to the human before the next
directive is authorized.

---

## 3. The turn cycle

```
            ┌───────────────────────────────────────────────────────────┐
            │                      ARCHITECT (human)                      │
            │         sets goals · approves directives · overrides        │
            └───────────────┬───────────────────────────▲────────────────┘
                            │ goal / approval            │ reads reports,
                            ▼                            │ approves next
        ┌───────────────────────────────┐               │
        │           COWORK               │               │
        │  • maintains the goal-log      │               │
        │  • decomposes goal → tasks     │───writes──┐    │
        │  • writes to_code directives   │           │    │
        │  • reviews from_code reports   │◀──────┐   │    │
        │  • keeps everything on-thesis  │       │   │    │
        └───────────────────────────────┘       │   │    │
                                                 │   ▼    │
                                        ┌────────────────────────┐
                                        │   CODE / SPECIALTY      │
                                        │   EXECUTOR              │
                                        │  • acts ONLY on a       │
                                        │    to_code directive    │
                                        │  • writes from_code     │
                                        │    when done/blocked    │
                                        └────────────────────────┘
```

One full turn:

1. **Architect** states or confirms a goal (`G-nn` in the goal-log).
2. **Cowork** decomposes it, writes a `to_code` directive, and surfaces it to the Architect.
3. **Architect** approves (sets `architect_approved: true`) or edits/denies. For tier 3–4
   directives this approval is explicit and per-directive.
4. **Executor** acts strictly within the directive, then writes a `from_code` report.
5. **Cowork** reviews the report against the directive's `done_when`, updates the goal-log,
   and proposes the next directive.
6. Back to step 3. The human is never more than one turn from the loop.

---

## 4. The logs (the durable artifacts)

Two append-only files live in the repo and are themselves part of the deliverable:

```
docs/
├── GOAL-LOG.md        # G-nn goals, their status, and which directives serve them
└── BUILD-LEDGER.md    # every to_code + from_code, in order, verbatim
```

`BUILD-LEDGER.md` is the chain of custody. Anyone — a future collaborator, a skeptical
engineer, the Architect six months later — can read exactly what was asked, what was done,
what deviated, and who approved each step. This is the same honesty discipline as the
model-baking notes ("document the win and the loss with equal honesty") applied to the
whole build. It is also what lets a *new* agent instance pick up the work without context
loss: the ledger is the shared memory.

---

## 5. Specialty executors

The pattern generalizes beyond Code. Any domain agent receives the same `to_code` directive
shape and returns the same `from_code` report:

- **Fusion 360 / mech-design agent** — receives directives like "model the single-servo
  four-bar forearm linkage so tips converge at a point 40 mm ahead of the body; output
  STEP + STL; confirm the convergence in a motion study." Returns CAD files + a report.
- **PCB / wiring agent** — receives "lay out the servo power rail per README §7 with a
  fused battery feed and a 470 µF bulk cap; output schematic + KiCad." Returns artifacts.
- **Firmware Executor (Code)** — the file/MCU/server heavy lifting.

All of them are bound by the same rules: act only on a directive, stay in scope, report
back, never self-authorize the next step. The Architect and Cowork remain the only sources
of new directives.

---

## 6. How this maps to SOUL.md

| Workflow rule | SOUL.md article |
|---------------|-----------------|
| Human approves every directive; agents never self-authorize | Art. 1 (steward not sovereign), Art. 7 (autonomy) |
| Higher tier = more explicit human checkpoint | Art. 15 (authorization levels) |
| Append-only ledger of every hand-off | Art. 17 (logging & auditability) |
| Executor must STOP and escalate on ambiguity or out-of-scope need | Art. 20 (escalation), Art. 6 (humility before limits) |
| Deviations reported honestly, never hidden | Art. 2 (truth), Art. 18 (error acknowledgment) |
| Reversible-first; irreversible actions gated | Art. 4 (irreversibility) |
| Executor may refuse a directive that violates the constitution | Art. 19 (right & duty to refuse) |

The build process and the product obey the same constitution. That is the point.

---

## 7. First directive (worked example)

The first real `to_code` Cowork will issue, once the Architect approves, creates the
public repo and docs on GitHub. A draft of it appears below so the Architect can review the
shape before the workflow goes live. **It is not authorized until `architect_approved`
flips to true.**

```yaml
to_code:
  id: TC-0001
  date: 2026-06-14
  from: cowork
  to: claude-code
  architect_approved: false        # <-- Architect flips this
  goal_ref: G-01
  authorization_tier: 3            # creates a public repo — public + somewhat irreversible
  title: "Create the public Gentis repo and seed core docs"
  context: |
    Gentis is the embodied Project Opengates demonstrator (see README). The brain reuses
    WireClaw; the voice reuses STT-Dongle; the net-new is the Gatekeeper, the 3-DOF
    mechanism, ToF tracking, and the piezo/eye expression layer. This directive only
    establishes the repo and documentation skeleton — no logic, no vendoring yet.
  task: |
    1. Create a new public GitHub repo: WhitneyDesignLabs/gentis (MIT + CERN-OHL-S).
    2. Commit the provided README.md and SOUL.md at the root.
    3. Create the directory tree exactly per README §17 (gentis/ ... ).
    4. Add placeholder docs: docs/GATEKEEPER.md, docs/BUILD.md, docs/BOM.md,
       docs/POSES.md, docs/DEMO.md — each with its section heading and a TODO line.
    5. Create docs/GOAL-LOG.md and docs/BUILD-LEDGER.md (this workflow's logs), and
       write this directive + the resulting from_code report as the first ledger entries.
    6. Add COWORK-CODE-WORKFLOW.md (this file) at the root.
  deliverables:
    - "Public repo WhitneyDesignLabs/gentis with the full README §17 tree"
    - "Core docs committed to main (tier-3 approved) or to an initial branch for review"
  constraints: |
    - Do NOT vendor WireClaw or STT-Dongle yet (separate directive).
    - Do NOT implement any firmware logic (stubs/headers only).
    - Hardware (cut files, STLs, linkage geometry) come from specialty-agent directives.
    - If the repo name or license files are unclear, STOP and ask.
  done_when: |
    The repo exists, is public, contains README + SOUL + the full tree + placeholder docs +
    the two logs, and a from_code report records every file and the commit hash.
  escalate_if: |
    GitHub auth/permissions are missing, the org name differs, or any step needs a tool
    not available. Report blocked rather than improvising.
```

When the Architect sets `architect_approved: true`, Cowork hands TC-0001 to Claude Code,
Code executes and returns FC-0001, Cowork logs both to `BUILD-LEDGER.md`, updates
`GOAL-LOG.md`, and proposes TC-0002 (vendor WireClaw core). The build proceeds one
documented, human-approved turn at a time.

---

*Cowork keeps the goal. Code does the work. The ledger keeps them both honest. The human
keeps the authority. — Project Opengates build protocol.*
