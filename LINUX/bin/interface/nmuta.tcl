# Copyright (C) 2012 -- Marcio Eduardo Delamaro and Jose Carlos Maldonado and Elisa Yumi Nakagawa
# 
# 
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
# 
#


#!/usr/local/bin/wish4.0

wm withdraw .

source iglobal.tcl

   while {[file exists $TMP_DIR_FILE]} {
      set num [file size $TMP_DIR_FILE];       # Reads size of file
 puts stdout $num
      send -async proteum "set nmuta $num";    # Sends command to application
      after 1000;                              # Waits for 1 second
   }
   exit
