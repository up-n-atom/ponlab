#!/usr/bin/awk -f

/Test cdrouter-[0-9]+:/ {
	name=$0
}

/SECTION\(cdrouter-[0-9]+\)/ {
	section=substr($0,index($0,$3))
}

/FAIL: Test .* \([0-9]+\) failed$/ {
	next;
}

/FAIL:/ {
	failure=substr($0,index($0,$3))
	print name"\n\x1b[33m "section"\n\x1b[1;31m "failure"\n\x1b[0m"
}
