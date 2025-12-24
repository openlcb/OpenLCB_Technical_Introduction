This repository contains the source for the OpenLCB Technical Introduction book aimed at developers.

Published site
----------------
The rendered book is published at:

https://openlcb.github.io/OpenLCB_Technical_Introduction

Repository layout
------------------
- `src/` — markdown source files for the book (used by mdBook).
- `plan/` — planning and helper documents, notes, and files used during authoring (including LLM/agent artifacts).
- other files — build artifacts and configuration (e.g., `book.toml`).

Building locally
-----------------
This book is built with `mdbook`. To build and preview locally:

1. Install mdBook (see https://rust-lang.github.io/mdBook/ for installers).
2. From the repository root run:

```bash
mdbook build
mdbook serve --open
```

The `build` command generates the static site in `book/` (by default). The `serve` command runs a local server and opens your browser.

Notes
-----
- Keep source edits in `src/` — the published site is generated from these files.
- Update `SUMMARY.md` to change the book table of contents.
- The `plan/` directory contains authoring aids and is not part of the published book.

Authorship & Assistance
------------------------
This repository was authored with assistance from GitHub Copilot. The `plan/` directory contains planning documents, progress tracking, style guidance, task lists, and agent/LLM artifacts used during authoring. Those files are intended to support the writing process and are not required for the published book itself.

License
-------
Source code files (examples, headers, C/C++/Arduino files) in this repository are licensed under the MIT License: see `LICENSE`.

Documentation and book text are Copyright © 2025 John Socha-Leialoha; see `LICENSE_DOCS.md` for details.

Feedback
--------
To provide feedback or report issues with the book, please open an issue on the project's GitHub issues page:

https://github.com/openlcb/OpenLCB_Technical_Introduction/issues

