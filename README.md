LibGUI Loom is a compatibility layer for running SerenityOS LibGUI applications through a native compositor.

# Plan tm

The plan :tm: is to bridge in several stages:

1. Create a standalone app that pretends to be WindowServer, and translates LibGUI<->WindowServer IPC into native (wayland-client, AppKit, etc.) commands

2. Decouple LibGUI from compositor backend

3. Add plugins for desired compositor to make running Serenity apps on your host more fun


# Building

Set `$SERENITY_SOURCE_DIR` to your serenity checkout

`cmake -GNinja -B build` and we're off.

On non-macOS, the development packages for libdbus-1 and libxkbcommon are required on top of Serenity dependencies.
The Wayland development packages can be handled automatically via FetchContent, or provided by your system package manager.

# Architecture

## IPC

Loom creates an IPC server for each of the SerenityOS system services that LibGUI apps expect to talk to.
As such, the Loom application is a single process with a large handful of sockets open for LibGUI apps to connect to
on one end. The other end is where each Loom port differentiates itself. The AppKit port talks to the macOS compositor and
system services through AppKit, Cocoa, and other macOS libraries. The Wayland port talks to the Wayland compositor and
system services through libwayland-client, libdbus-1, and other Unix libraries. Several services don't have a non-SerenityOS
analog, and are thus managed directly by Loom.

```text
Native                    ┌────────────────────────────────────┐        ┌────────────────────────────────────┐
Compositor                │  Loom (Server)                     │        │  LibGUI App                        │
┌──────────────┐          │                                    │        │                                    │
│  Wayland/    │          │  ┌──────────────────────────────┐  │        │  ┌────────────────────────────┐    │
│  AppKit      │◄────────►│  │  WindowServer                │◄─┼────────┼─►│  LibGUI                    │    │
│              │          │  │  /tmp/portal/window          │  │        │  │                            │    │
└──────────────┘          │  └──────────────────────────────┘  │        │  └────────────────────────────┘    │
                          │                                    │        │                                    │
Native                    │  ┌──────────────────────────────┐  │        │  ┌────────────────────────────┐    │
System Services           │  │  Clipboard                   │◄─┼────────┼─►│  LibClipboard              │    │
┌──────────────┐          │  │  /tmp/session/0/portal/      │  │        │  │                            │    │
│  D-Bus       │◄────────►│  │   clipboard                  │  │        │  └────────────────────────────┘    │
│  Config      │          │  └──────────────────────────────┘  │        │                                    │
│  Notification│          │                                    │        │  ┌────────────────────────────┐    │
│              │          │  ┌──────────────────────────────┐  │        │  │  LibConfig                 │    │
└──────────────┘          │  │  ConfigServer                │◄─┼────────┼─►│                            │    │
                          │  │  /tmp/session/0/portal/      │  │        │  └────────────────────────────┘    │
                          │  │   config                     │  │        │                                    │
                          │  └──────────────────────────────┘  │        │  ┌────────────────────────────┐    │
                          │                                    │        │  │  LibFileSystemAccessClient │    │
                          │  ┌──────────────────────────────┐  │        │  │                            │    │
                          │  │  FileSystemAccessServer      │◄─┼────────┼─►│                            │    │
                          │  │  /tmp/session/0/portal/      │  │        │  └────────────────────────────┘    │
                          │  │   filesystemaccess          │  │        │                                    │
                          │  └──────────────────────────────┘  │        │  ┌────────────────────────────┐    │
                          │                                    │        │  │  LibDesktop                │    │
                          │  ┌──────────────────────────────┐  │        │  │                            │    │
                          │  │  LaunchServer                │◄─┼────────┼─►│                            │    │
                          │  │  /tmp/session/0/portal/      │  │        │  └────────────────────────────┘    │
                          │  │   launch                     │  │        │                                    │
                          │  └──────────────────────────────┘  │        │  ┌────────────────────────────┐    │
                          │                                    │        │  │  LibGUI                    │    │
                          │  ┌──────────────────────────────┐  │        │  │  (Notifications)           │    │
                          │  │  NotificationServer          │◄─┼────────┼─►│                            │    │
                          │  │  /tmp/session/0/portal/      │  │        │  └────────────────────────────┘    │
                          │  │   notification               │  │        │                                    │
                          │  └──────────────────────────────┘  │        │  ┌────────────────────────────┐    │
                          │                                    │        │  │  LibImageDecoderClient     │    │
                          │  ┌──────────────────────────────┐  │        │  │                            │    │
                          │  │  ImageDecoderServer          │◄─┼────────┼─►│                            │    │
                          │  │  /tmp/session/0/portal/      │  │        │  │                            │    │
                          │  │   image-decoder              │  │        │  └────────────────────────────┘    │
                          │  └──────────────────────────────┘  │        │                                    │
                          │                                    │        │                                    │
                          └────────────────────────────────────┘        └────────────────────────────────────┘
```

## Windows

At a high level, each window is rendered into a single Loom-owned buffer that contains both the decorations and the client content.

```text
┌──────────────────────────── frame rect ────────────────────────────┐
│  frame decorations                                                 │
│  (titlebar, borders, shadow, bevel)                                │
│                                                                    │
│  ┌────────────────────── window rect ────────────────────────────┐ │
│  │  client content                                               │ │
│  │  (LibGUI backing store must be blit manually)                 │ │
│  └───────────────────────────────────────────────────────────────┘ │
└────────────────────────────────────────────────────────────────────┘
```

This is actually how the SerenityOS WindowServer renders windows. WindowServer blits each bitmap onto a double-buffered
'screen buffer' that is rendered directly to a frame buffer.

For the Wayland port, that means:
- allocate a shm-backed `wl_buffer` sized to the full frame rect
- paint the frame decorations into the buffer first
- blit the client backing store into the window rect area
- attach and commit the completed buffer to the window's `wl_surface`
- use `xdg_surface_set_window_geometry()` to hint that the client rect is the inner content area, not the full decorated frame
