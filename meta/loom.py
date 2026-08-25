#!/usr/bin/env python3
"""Run Loom D-Bus ping or a demo app on an isolated session bus.

This script intentionally uses `dbus-run-session` so it does not touch the
currently running user D-Bus service.
"""

import argparse
import os
import pathlib
import subprocess
import sys


def repo_root() -> pathlib.Path:
    return pathlib.Path(__file__).resolve().parent.parent


def run_in_isolated_session(
    command: list[str],
    build_dir_path: pathlib.Path,
    timeout: int | None = None,
) -> int:
    bdir = build_dir_path
    current_dirs = os.environ.get("XDG_DATA_DIRS", "/usr/local/share:/usr/share")
    env = os.environ.copy()
    env["XDG_DATA_DIRS"] = f"{bdir}:{current_dirs}"
    cmd = [
        "dbus-run-session",
        "--",
        *command,
    ]
    try:
        completed = subprocess.run(cmd, env=env, check=False, timeout=timeout)
    except subprocess.TimeoutExpired:
        print(f"Command timed out after {timeout}s.", file=sys.stderr)
        return 124
    return completed.returncode


def run_mode(build_dir_path: pathlib.Path, args: argparse.Namespace) -> int:
    if args.ping:
        ping_cmd = [
            "dbus-send",
            "--session",
            "--print-reply",
            "--reply-timeout=20000",
            "--dest=org.serenityos.Loom",
            "/org/serenityos/Loom",
            "org.serenityos.Loom.Ping",
        ]
        rc = run_in_isolated_session(ping_cmd, build_dir_path, timeout=20)
        if rc == 0:
            print("Ping reply received from org.serenityos.Loom on isolated session bus.")
        return rc

    if args.app is not None:
        app = build_dir_path / "bin" / args.app
        if not app.exists():
            print(f"App not found: {app}", file=sys.stderr)
            return 2
        return run_in_isolated_session([str(app)], build_dir_path)

    return 1


def main() -> int:
    parser = argparse.ArgumentParser(description="Loom local D-Bus/dev runner")
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--ping", action="store_true", help="Send Loom Ping and require a reply")
    group.add_argument("--app", metavar="NAME", help="Launch app from <build-dir>/bin on an isolated session")
    parser.add_argument("--build-dir", metavar="PATH", help="Build directory (default: <repo>/build)")

    args = parser.parse_args()
    root = repo_root()
    build_dir_path = pathlib.Path(args.build_dir).expanduser() if args.build_dir else (root / "build")
    build_dir_path = build_dir_path.resolve()
    return run_mode(build_dir_path, args)


if __name__ == "__main__":
    sys.exit(main())
