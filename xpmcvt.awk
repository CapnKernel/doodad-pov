#! /bin/gawk -f

# TODO: Read the colour map
# At the moment it assumes the two colours are "." and "+".
match($0, /^"(........)(........)"/, a) {
	gsub(/\./, "0", a[1])
	gsub(/\./, "0", a[2])
	gsub(/+/, "1", a[1]);
	gsub(/+/, "1", a[2]);
	print " { 0b" a[1] ", 0b" a[2] "},"
	next
}
