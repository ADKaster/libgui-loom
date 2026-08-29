#!/usr/bin/env python3
"""
Run Loom D-Bus ping or an app on an isolated session bus.

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

def resolve_build_dir(build_dir: str | None) -> pathlib.Path:
    root = repo_root()
    build_dir_path = pathlib.Path(build_dir).expanduser() if build_dir else (root / "build")
    return build_dir_path.resolve()

def find_cmake_cache_variable(build_dir: pathlib.Path, variable: str) -> str | None:
    cache_file = build_dir / "CMakeCache.txt"
    if not cache_file.exists():
        return None
    with cache_file.open() as f:
        for line in f:
            if line.startswith(f"{variable}:"):
                return line.split("=", 1)[1].strip()
    return None


def run_in_isolated_session(
    command: list[str],
    build_dir: pathlib.Path,
    timeout: int | None = None,
) -> int:
    current_dirs = os.environ.get("XDG_DATA_DIRS", "/usr/local/share:/usr/share")
    env = os.environ.copy()
    env["XDG_DATA_DIRS"] = f"{build_dir}:{current_dirs}"
    cmd = [
        "dbus-run-session",
        "--",
        *command,
    ]
    try:
        completed = subprocess.run(cmd, env=env, check=False, timeout=timeout)
    except subprocess.TimeoutExpired:
        print(f"Command timed out after {timeout}s.", file=sys.stderr)
        return 1
    except KeyboardInterrupt:
        print("Command interrupted by user.", file=sys.stderr)
        return 1
    return completed.returncode


def run_ping(build_dir: pathlib.Path) -> int:
    ping_cmd = [
        "dbus-send",
        "--session",
        "--print-reply",
        "--reply-timeout=20000",
        "--dest=org.serenityos.Loom",
        "/org/serenityos/Loom",
        "org.serenityos.Loom.Ping",
    ]
    rc = run_in_isolated_session(ping_cmd, build_dir, timeout=20)
    if rc == 0:
        print("Ping reply received from org.serenityos.Loom on isolated session bus.")
    else:
        print(f"Ping failed with return code {rc}.", file=sys.stderr)
    return rc


def run_app(build_dir: pathlib.Path, app_name: str) -> int:
    app = build_dir / "bin" / app_name
    if not app.exists():
        print(f"App not found: {app}", file=sys.stderr)
        return 1
    return run_in_isolated_session([str(app)], build_dir)


def main() -> int:
    parser = argparse.ArgumentParser(description="Loom local D-Bus/dev runner")
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--ping", action="store_true", help="Send Loom Ping and require a reply")
    group.add_argument("--app", metavar="NAME", help="Launch app from <build-dir>/bin on an isolated session")
    parser.add_argument("--build-dir", metavar="PATH", help="Build directory (default: <repo>/build)")

    args = parser.parse_args()

    build_dir = resolve_build_dir(args.build_dir)

    if not os.environ.get("SERENITY_SOURCE_DIR"):
        serenity_source_dir = find_cmake_cache_variable(build_dir, "SERENITY_SOURCE_DIR")
        os.environ["SERENITY_SOURCE_DIR"] = serenity_source_dir

    if args.ping:
        return run_ping(build_dir)
    elif args.app:
        return run_app(build_dir, args.app)
    else:
        assert False


if __name__ == "__main__":
    sys.exit(main())
