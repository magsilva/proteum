#!/bin/csh -f
if (-e $PROTEUMIMHOME/interface/proteumIM) then
	set path = ($PROTEUMIMHOME $path)
	set wish = `which wish`
	$wish $PROTEUMIMHOME/interface/proteumIM -- $1
else
	echo "Tcl/Tk PROTEUM/IM interface not found in $PROTEUMIMHOME/interface"
	echo "Install it or run Proteum through the command line"
endif


