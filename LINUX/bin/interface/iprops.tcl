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
#   IPROPS.TCL
#   Procedures related with PROPERTIES options.
#   author: Elisa Yumi Nakagawa
#   date: 02/23/96
#   last update: 04/18/96
#------------------------------------------------------------------------------#
 

#------------------------------------------------------------------------------#
#   PROPS_WINDOW
#   Create properties window.
#   date: 02/23/96
#   last update: 04/18/96
#------------------------------------------------------------------------------#

proc Props_Window {w args} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Properties" 
   wm iconname $w "Properties"
   wm geometry $w +10+65
   wm minsize  $w 470 105
   if { $FIXEDSIZE } {
	wm maxsize  $w 470 105
   	wm geometry $w 470x105
   }

   global ddirectory \
          in         \
          timeout
   
   set in(dir)  [Set_Directory $ddirectory];  # set directory name 
   set in(tout) [Set_Timeout $timeout]

   frame $w.f1
   pack  $w.f1 -fill x -pady 5
   label $w.f1.l  -text "Default Directory:" 
   entry $w.f1.in -width 50 -textvariable in(dir)
   pack  $w.f1.l $w.f1.in -side left 

   frame $w.f2
   pack  $w.f2 -fill x
   label $w.f2.l  -text "Timeout Mutation Execution:" 
   entry $w.f2.in -width 5 -justify right -textvariable in(tout)
   pack  $w.f2.l $w.f2.in -side left 


   # Button for UP and DOWN of default timeout 
   button $w.f2.up -text up -command {
      .proteum.props.dlg.f2.dw config -state normal
      incr in(tout)
   }
   button $w.f2.dw -text dw -command {
      set v .proteum.props.dlg.f2

      $v.up config -state normal
      if {$in(tout) > 2} {
         incr in(tout) -1
         if {$in(tout) == 2} "$v.dw config -state disabled"
      } else {
         $v.dw config -state disabled
      } 
   }
   pack $w.f2.up $w.f2.dw -side left

   
   if {$in(tout) == 0} {
      .proteum.muta.m.dlg.stat.f2.dw config -state disabled
   }  

   frame  $w.f3 
   pack   $w.f3 -pady 5
   button $w.f3.confirm -text Confirm -command "
          Verify_Props_Data $w
          set confirm 1"
   button $w.f3.cancel  -text Cancel  -command "
          destroy $w
          set confirm 0"
   pack   $w.f3.confirm $w.f3.cancel -side left 

   bind $w <Any-Motion> {focus %W}
   bind $w.f1.in      <Tab> [list focus $w.f2.in]
   bind $w.f2.in      <Tab> [list focus $w.f3.confirm]
   bind $w.f3.confirm <Tab> [list focus $w.f3.cancel]
   bind $w.f3.cancel  <Tab> [list focus $w.f1.in]
 
   bind $w.f1.in <Return> [list focus $w.f2.in]
   bind $w.f2.in <Return> "$w.f3.confirm invoke"

   wm protocol $w WM_DELETE_WINDOW {
      destroy .proteum.props.dlg  
      set confirm 0
   }
   grab $w
   tkwait variable confirm
   grab release $w
}



#------------------------------------------------------------------------------#
#   VERIFY_PROPS_DATA {}
#   Verify properties data.
#   date: 03/25/96
#   last update: 04/18/96
#------------------------------------------------------------------------------#

proc Verify_Props_Data {w} {

   global  ddirectory \
           in         \
           timeout
 
   set in(dir)  [Blank_Space_Out $in(dir)];   # take out blank space
   set in(tout) [Blank_Space_Out $in(tout)];  # take out blank space


   if {![file isdirectory $in(dir)]} {
      message {This Directory Does Not Exist!}
      return
   } elseif {![regexp {[0-9]+} $in(tout)]} {
      message {Invalid Value to Timeout !}
      return
   } else {
      set ddirectory $in(dir)
      set timeout    $in(tout)
      destroy $w
   }
}
