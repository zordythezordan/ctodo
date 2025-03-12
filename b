gcc $(find . | grep -E '\.c$' | tr '\n' ' ') -lcgui -lcobj -lxcb-keysyms

