
LibGUI Loom is a compatability layer to run SerenityOS LibGUI applications through a native compositor.

# Plan tm

The plan :tm: is to bridge in several stages:

1. Create a standalone app that pretends to be WindowServer, and translates LibGUI<->WindowServer IPC into native (wayland-client, AppKit, etc) commands

2. Decouple LibGUI from compositor backend

3. Add plugins for desired compositor to make running Serenity apps on your host more fun


# Building

Set `$SERENITY_SOURCE_DIR` to your serenity checkout

`cmake -GNinja -B build` and we're off
