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


#------------------------------------------------------------------------------#
#   IGERAL.TCL
#   General functions for user interface.
#   author: Elisa Yumi Nakagawa
#   date: 02/21/96
#   last update: 07/02/96, 11/18/96
#------------------------------------------------------------------------------#


#------------------------------------------------------------------------------#
#   Set_Busy
#   Sets mouse pointer as busy/nobusy.
#   date: 10/11/96
#   last update: 10/11/96, 11/18/96
#------------------------------------------------------------------------------#

proc Set_Busy {option} {
}


proc feedback {str} {
   global comandwindow
   $comandwindow insert end "$str\n"
   $comandwindow see end
}

#------------------------------------------------------------------------------#
#    MESSAGE {}
#    Print error message.
#    date: 03/21/96
#    last update: 05/06/96, 09/19/96
#------------------------------------------------------------------------------#

proc message {text} {
   toplevel .msg
   wm title .msg "Message"
   wm iconname .msg "MESSAGE"
   wm geometry .msg +155+190

   label .msg.title -text $text
   pack .msg.title -padx 20 -pady 10

   frame  .msg.f1 -bd 1 -relief sunken
   pack   .msg.f1 -pady 10
   button .msg.f1.ok -text "   OK   " -command {set ok 1}
   pack   .msg.f1.ok -padx 2 -pady 2

   tkwait visibility .msg;  # Waits the window .msg to become visible
   focus .msg.f1.ok
   grab  .msg
   tkwait variable ok
   grab release .msg

   destroy .msg
   return
}


#------------------------------------------------------------------------------#
#    MESSAGE_2B {}
#    Print error message in a two buttons window.
#    date: 03/21/96
#    last update: 05/06/96
#------------------------------------------------------------------------------#

proc message_2b {w msg} {
   toplevel .msg
   wm title .msg "Message"
   wm iconname .msg "MESSAGE"
   wm geometry .msg +155+190
  
   global maxtcase

   label .msg.title -text $msg
   pack  .msg.title -padx 20 -pady 10

   frame  .msg.f1  
   pack   .msg.f1 -pady 10 -padx 10

   frame  .msg.f1.f1 -borderwidth 1 -relief sunken 
   frame  .msg.f1.f2
   pack   .msg.f1.f1 .msg.f1.f2  -padx 4 -side left

   set x false
   button .msg.f1.f1.cancel -text {  Cancel  } -command "
            set x true"
   button .msg.f1.f2.confirm -text {Overwrite} -command "
            set x false"
   pack .msg.f1.f1.cancel .msg.f1.f2.confirm -side left
   tkwait visibility .msg;  # Waits the window .msg to become visible
   focus .msg.f1.f1.cancel
   grab  .msg
   tkwait variable x
   grab release .msg
   destroy .msg
   return $x
}




#------------------------------------------------------------------------------#
#    LOAD_SRC {}
#    Loads a file into text area.
#
#    date: 5/9/00
#    last update: 5/9/00
#------------------------------------------------------------------------------#

proc Load_Src { filename textarea} {

   set data ""
   if [catch {open $filename r} fileId] {
      message "Cannot Open File $filename !"
      return
   }
   $textarea config -state normal
   $textarea delete 1.0 end;          # Deletes last mutant of text area

   while { ! [eof $fileId] } {
	set data [read $fileId 1024]
	$textarea insert end $data
   }
   close $fileId
}

#------------------------------------------------------------------------------#
#    LOAD_SRC_Muta {}
#    Loads source code of program (original and mutant) into text area.
#
#    date: 5/9/00
#    last update: 5/9/00
#------------------------------------------------------------------------------#

proc Load_Src_Muta { filename textarea} {

   set data ""
   if [catch {open $filename r} fileId] {
      message "Cannot Open File $filename !"
      return
   }
   $textarea config -state normal -foreground black
   $textarea delete 1.0 end;          # Deletes last mutant of text area
   set on 0
   set i 0
   $textarea tag add fgc$i 1.0 insert
   $textarea tag configure fgc$i -foreground black

   while { ! [eof $fileId] } {
	set data [read $fileId 1]
	if { $data == "@"} {
	   set data [read $fileId 1]
	   if {$data == "\n"} {
	      set data ""
	      if {$on != 0} {
	         set on 0
	         incr i 1
	         $textarea mark set m$i insert
                 $textarea tag add fgc$i m$i insert
                 $textarea tag configure fgc$i -foreground black
	      } else {
	         set on 1
	         incr i 1
	         $textarea mark set m$i insert
                 $textarea tag add fgc$i m$i insert
                 $textarea tag configure fgc$i -foreground red
	      }
	   } else {
	      set data "@$data"
	   }
	}
	$textarea insert end $data fgc$i
   }
   close $fileId
}


#------------------------------------------------------------------------------#
#    HILIGHT {}
#
#    date: 
#    last update:
#------------------------------------------------------------------------------#

proc HiLight {textarea ehmuta} {
   global test
   global muta
   set nline  0;     # Counts the number of lines
   set mline  0;     # First line in the file when the mutation occur
   set nchar0 0;     # Counts the number of character

   $textarea tag configure red -foreground red; # Color to arrow
   $textarea insert "$textarea.mutationhere.last linestart"  ">>" red
   $textarea see "$textarea.mutationhere.last linestart"
   return

}


#------------------------------------------------------------------------------#
#   ENABLE_MENUBUTTONS {}
#   Enables menu buttons if a test session is open.
#   date: 06/05/96
#   last update: 06/05/96
#------------------------------------------------------------------------------#

proc Enable_MenuButtons {} {
   .proteum.tcase  configure -state normal
   .proteum.muta   configure -state normal
   .proteum.report configure -state normal
   .proteum.status configure -state normal
}


#------------------------------------------------------------------------------#
#   SHOW_CONNECTION {}
#   Show connections.
#   date: 11/13/96
#   last update: 11/13/96, 11/20/96
#------------------------------------------------------------------------------#

proc Show_Connection {textarea type} {
   global MAXCONNECTIONS
   global connection
   global ind_con_gen
   global ind_con_sel

   # destroys checkbutton and the frame if they exist
   set i 0
   while {[winfo exists $textarea.$i]} {
      destroy $textarea.$i
      incr i
   }

   for {set i 0} {$i < $MAXCONNECTIONS} {incr i} {
      frame $textarea.$i
      pack  $textarea.$i
      $textarea window create end -window $textarea.$i
      if {[string compare $type GENERATE] == 0} {
         checkbutton $textarea.$i.c$i -text "[expr $i+1].$connection($i)" \
                    -variable ind_con_gen($i) -selectcolor green          \
                    -cursor hand2
      } else {
         checkbutton $textarea.$i.c$i -text "[expr $i+1].$connection($i)" \
                    -variable ind_con_sel($i) -selectcolor green          \
                    -cursor hand2
      }
      pack $textarea.$i.c$i -fill x
   }
}


#------------------------------------------------------------------------------#
#   ADD_CONNECTION {}
#   Add connection into connections list.
#   date: 11/13/96
#   last update: 11/13/96
#------------------------------------------------------------------------------#

proc Add_Connection {} {
   global MAXCONNECTIONS
   global connection
   global ind_con_gen
   global in

   if {[scan $in(con) "%s %s" func1 func2] != 2} {
       message "Wrong Number of Functions !"
       return 0
   }
   set connection($MAXCONNECTIONS) $in(con)
   set ind_con_gen($MAXCONNECTIONS) 1
   incr MAXCONNECTIONS
   set in(con) ""
   return 1
}

#------------------------------------------------------------------------------#
#   ADD_UNIT {}
#   Add unit into unit list.
#   date: 11/13/96
#   last update: 11/13/96
#------------------------------------------------------------------------------#

proc Add_Unit {} {
   global MAXCONNECTIONS
   global connection
   global ind_con_gen
   global in

   if {[scan $in(con) "%s" func1] != 1} {
       message "Wrong Number of Functions !"
       return 0
   }
   set in(con) $func1
   set connection($MAXCONNECTIONS) $in(con)
   set ind_con_gen($MAXCONNECTIONS) 1
   incr MAXCONNECTIONS
   set in(con) ""
   return 1
}


#------------------------------------------------------------------------------#
#   DELETE_CONNECTION {}
#   Delete connection of the connections list.
#   date: 11/13/96
#   last update: 11/13/96, 11/20/96
#------------------------------------------------------------------------------#

proc Delete_Connection {} {
   global MAXCONNECTIONS
   global connection
   global ind_con_gen

   set j 0

   for {set i 0} {$i < $MAXCONNECTIONS} {incr i} {
      if {$ind_con_gen($i) != 0} {
         set tmp($j) $connection($i)
         set ind($j) $ind_con_gen($i)
         incr j
      }
   }
   set MAXCONNECTIONS $j

   for {set i 0} {$i < $MAXCONNECTIONS} {incr i} {
      set connection($i) $tmp($i) 
      set ind_con_gen($i) $ind($i)
   }
}

#------------------------------------------------------------------------------#
#   DELETE_UNIT {}
#   Delete unit of the unit list.
#   date: 24/05/00
#   last update: 24/05/00
#------------------------------------------------------------------------------#

proc Delete_Connection {} {
   global MAXCONNECTIONS
   global connection
   global ind_con_gen

   set j 0

   for {set i 0} {$i < $MAXCONNECTIONS} {incr i} {
      if {$ind_con_gen($i) != 0} {
         set tmp($j) $connection($i)
         set ind($j) $ind_con_gen($i)
         incr j
      }
   }
   set MAXCONNECTIONS $j

   for {set i 0} {$i < $MAXCONNECTIONS} {incr i} {
      set connection($i) $tmp($i) 
      set ind_con_gen($i) $ind($i)
   }
}


#------------------------------------------------------------------------------#
#   MIN {}
#   Returns the minimum number between two numbers.
#   date: 06/06/97
#   last update: 06/06/97
#------------------------------------------------------------------------------#


proc min {a b} {
   if {$a <= $b} "return $a"
   return $b
}
