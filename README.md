## Multithreaded Network Firewall Rule Management System

A compact C-based prototype for high-throughput packet inspection and firewall rule management. The repository contains lean, focused components that demonstrate rule parsing, client/server interaction, and a multithreaded server for concurrent request handling.

### Highlights
- **Rule-driven filtering**: Loads simple text-based rules (e.g., `ban.txt`) to allow/deny traffic.
- **Client–server harness**: Minimal client and server programs for local validation.
- **Multithreaded server**: A threaded implementation for concurrent processing.
- **Portable build**: Standard Makefiles per module; no external runtime dependencies.

### Architecture Overview
- **Packet inspection utilities (`e1`)**: Basic rule I/O and packet checks for quick experiments and validation.
- **Baseline client/server (`e2`)**: Straightforward single-threaded reference for behavior comparison and quick testing.
- **Threaded server (`serverThreaded`)**: Concurrent server variant intended for higher throughput under load.

### Repository Layout
- `e1/` — Rule reading and packet checking utilities; includes sample `ban.txt`.
- `e2/` — Minimal client and server for interactive tests.
- `serverThreaded/` — Multithreaded server and matching client harness.

### Quick Start
- **Requirements**: POSIX-like environment, a C compiler (GCC/Clang), and `make`.
- **Build**: Use the Makefile within each module directory.
- **Run**: Start the server component for a module, then connect with the provided client. Adjust `ban.txt` to experiment with rules.
