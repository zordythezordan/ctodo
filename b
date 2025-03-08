gcc $(find . | grep -E '\.c$' | tr '\n' ' ') -lcgui -lxcb-keysyms

