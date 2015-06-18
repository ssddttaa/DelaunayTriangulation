#!/bin/sh
bindir=$(pwd)
cd /Users/shaikazmath/Desktop/Desktop Folder/Hackathon/STEM 2014-2015/OpenGL/OpenGL-tutorial_v0014_21/tutorial05_textured_cube/
export DYLD_LIBRARY_PATH=:$DYLD_LIBRARY_PATH

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		 -batch -command=$bindir/gdbscript  /Users/shaikazmath/Desktop/Desktop\ Folder/Hackathon/STEM\ 2014-2015/OpenGL/MinSizeRel/tutorial05_textured_cube 
	else
		"/Users/shaikazmath/Desktop/Desktop\ Folder/Hackathon/STEM\ 2014-2015/OpenGL/MinSizeRel/tutorial05_textured_cube"  
	fi
else
	"/Users/shaikazmath/Desktop/Desktop\ Folder/Hackathon/STEM\ 2014-2015/OpenGL/MinSizeRel/tutorial05_textured_cube"  
fi
