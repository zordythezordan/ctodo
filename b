gcc $(find . | grep -e "\.c$" | tr "\n" " ") -lcgui -o crecord
