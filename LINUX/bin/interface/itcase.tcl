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
#   ITCASE.TCL
#   Procedures related with TCASE options.
#   author: Elisa Yumi Nakagawa
#   date: 02/27/96
#   last update: 07/02/96, 09/25/96, 09/15/97
#------------------------------------------------------------------------------#

#------------------------------------------------------------------------------#
#   TCASE_ADD_WINDOW {}    
#   Input parameters.
#   date: 02/27/96
#   last update: 05/10/96
#------------------------------------------------------------------------------#

proc Tcase_Add_Window {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Parameters for Interactive Execution"
   wm iconname $w "Parameters"
   wm geometry $w +10+65
   wm minsize  $w 450 85
   if { $FIXEDSIZE } {
   	wm maxsize  $w 450 85
   	wm geometry $w 450x85
   }

   global parameter

   set parameter ""

   frame $w.f1
   pack  $w.f1 -pady 5  
   label $w.f1.l -text "Parameters:"
   entry $w.f1.in -width 50 -textvariable parameter
   pack  $w.f1.l $w.f1.in -side left 
      

   frame  $w.f9
   pack   $w.f9
   button $w.f9.ok -text "   OK   " -command "
          set ok 1
          Confirm_Add_Tcase
          destroy $w"
   button $w.f9.cancel -text " Cancel " -command "
          set ok 0
          destroy $w"
   pack   $w.f9.ok $w.f9.cancel -side left -pady 10


   bind $w <Any-Motion> {focus %W}
   bind $w.f1.in <Return> {
        set ok 1
        destroy .proteum.tcase.m.dlg
        Confirm_Add_Tcase
   }

   wm protocol $w WM_DELETE_WINDOW {
      set ok 0
      destroy .proteum.tcase.m.dlg
   }
   grab $w
   tkwait variable ok
   grab release $w 
}



#------------------------------------------------------------------------------#
#   CONFIRM_ADD_TCASE {}
#   Execute program for add test case.
#   date: 02/27/96
#   last update: 05/31/96
#------------------------------------------------------------------------------#

proc Confirm_Add_Tcase {} { 
   global parameter 

   set out [Tcase ADD $parameter] 
}



#------------------------------------------------------------------------------#
#   SHOW_OUTPUT {}
#   Show output for add test case.
#   date: 05/21/96
#   last update: 05/21/96
#------------------------------------------------------------------------------#

proc Show_Output {t args} {

   if [eof [lindex $args 0]] {
      catch {close [lindex $args 0]}
   } else {
      gets [lindex $args 0] line
      $t insert end $line\n
      $t see end
   }
}



#------------------------------------------------------------------------------#
#   TCASE_VIEW_WINDOW {}  
#   View test case.
#   date: 02/27/96
#   last update: 05/10/96
#------------------------------------------------------------------------------#

proc Tcase_View_Window {w args} {
   global FIXEDFONT
   global test;     # Information about test session
   global ntcase;   # Number of current test case
   global maxtcase; # Max number of test cases
   global tcase;    # Information of test case
   global f1;       # Font
   global FIXEDSIZE;

   Set_Busy ON
   # Atualize number of test cases
   set infogeneral [Report GENERAL 0]
   if {[string length $infogeneral] == 0} {
      return
   } else {
        Disjoin_Infogeneral $infogeneral PART
   }  
   Set_Busy OFF

   if {$maxtcase == 0} {
      message {No Test Case to View !}
      return
   }

   toplevel $w
   wm title $w "View Test Case" 
   wm iconname $w "View"
   wm geometry $w +10+65
   wm minsize  $w 500 590
   if { $FIXEDSIZE } {
   	wm maxsize  $w 500 590
   	wm geometry $w 500x590
   }

   set ntcase 1;    # Sets number of current test case


   frame $w.f1 
   pack  $w.f1 -fill x -pady 5
   label $w.f1.l1 -text "                   Test Case:"
   entry $w.f1.in -width 10 -textvariable ntcase


   button $w.f1.up -text Up -command {
     .proteum.tcase.m.dlg.f1.dw config -state normal;  # Button DW enabled
     if {$ntcase < $maxtcase} {
        incr ntcase
        if {$ntcase == $maxtcase} {
          .proteum.tcase.m.dlg.f1.up config -state disabled; # Button UP disabled
        }

        # Looks for test case and shows into text area
        if {[Look_Show_Tcase .proteum.tcase.m.dlg] == 0} {
          .proteum.tcase.m.dlg.f1.up config -state disabled; # Button UP disabled
        }
     } 
   }

   button $w.f1.dw -text Dw -command {
     .proteum.tcase.m.dlg.f1.up config -state normal;  # Button UP enabled
     if {$ntcase > 1} {
        incr ntcase -1
        if {$ntcase == 1} {
          .proteum.tcase.m.dlg.f1.dw config -state disabled; # Button DW disabled
        }
        # Looks for test case and shows into text area
        Look_Show_Tcase .proteum.tcase.m.dlg 
     }
   }

   pack  $w.f1.l1 $w.f1.in $w.f1.up $w.f1.dw -side left 

   # If exists only one test case, disables the button UP
   if {$maxtcase == 1} {
      .proteum.tcase.m.dlg.f1.up config -state disabled; # Button UP disabled
   }

   # Disbles button DOWN when it's the first test case
   $w.f1.dw config -state disabled

   frame $w.f2 
   pack  $w.f2 -fill x -pady 5 
   label $w.f2.label -text "                        Status:"
   pack  $w.f2.label -side left

   radiobutton $w.f2.r1 -text Enable -variable tcase(status) -value ENABLE \
                        -selectcolor green -command "Tcase ENABLE" -cursor hand2
   pack $w.f2.r1 -side left  
   radiobutton $w.f2.r2 -text Disable -variable tcase(status) -value DISABLE \
                        -selectcolor green -command "Tcase DISABLE" -cursor hand2
   pack $w.f2.r2 -side left

   frame $w.f3
   pack  $w.f3 -fill x -pady 3
   label $w.f3.l1 -text "                 Parameters:"
   label $w.f3.l2 -font $f1 -width 50 -height 1 -relief sunken -anchor w
   pack  $w.f3.l1 $w.f3.l2 -side left

   frame $w.f4
   pack  $w.f4 -fill x -pady 3 
   label $w.f4.l1 -text "Exec.Time (1/100 Sec):"
   label $w.f4.l2 -font $f1 -width 5 -height 1 -relief sunken -anchor w
   pack  $w.f4.l1 $w.f4.l2 -side left

   frame $w.f5
   pack  $w.f5 -fill x -pady 3 
   label $w.f5.l1 -text "                Return Code:"
   label $w.f5.l2 -font $f1 -width 5 -height 1 -relief sunken -anchor w
   pack  $w.f5.l1 $w.f5.l2 -side left

   frame $w.f6 
   pack  $w.f6 -fill x -pady 2
   label $w.f6.text -text "STDIN:"
   pack  $w.f6.text -side left

   frame $w.f7 
   pack  $w.f7 -side top 
   text $w.f7.t -yscrollcommand "$w.f7.s set" -width 80 -height 7 \
                -cursor top_left_arrow -bg white -font $FIXEDFONT
   scrollbar $w.f7.s -command "$w.f7.t yview"
   pack $w.f7.s -side right -fill y
   pack $w.f7.t -fill both  
 

   frame $w.f10 
   pack  $w.f10 -fill x -pady 2
   label $w.f10.text -text "STDOUT:"
   pack  $w.f10.text -side left

   frame $w.f8
   pack  $w.f8 -side top
   text  $w.f8.t -yscrollcommand "$w.f8.s set" -width 80 -height 7 \
                 -cursor top_left_arrow -bg white -font $FIXEDFONT
   scrollbar $w.f8.s -command "$w.f8.t yview"
   pack $w.f8.s -side right -fill y
   pack $w.f8.t -fill both  
 

   frame $w.f11 
   pack  $w.f11 -fill x -pady 2
   label $w.f11.text -text "STDERR:"
   pack  $w.f11.text -side left

   frame $w.f12
   pack  $w.f12 -side top
   text  $w.f12.t -yscrollcommand "$w.f12.s set" -width 80 -height 7 \
                 -cursor top_left_arrow -bg white -font $FIXEDFONT
   scrollbar $w.f12.s -command "$w.f12.t yview"
   pack $w.f12.s -side right -fill y
   pack $w.f12.t -fill both 


   frame  $w.f9 -borderwidth 1 -relief sunken
   pack   $w.f9 -pady 10
   button $w.f9.ok -text {   OK   } -command "
          set ok 1
          destroy $w"
   pack   $w.f9.ok -expand true -padx 2 -pady 2


   # Shows the first test case
   Look_Show_Tcase $w

   bind $w <Any-Motion> {focus %W}
   bind $w.f1.in <Tab> [list focus $w.f1.up]
   bind $w.f1.up <Tab> [list focus $w.f1.dw]
   bind $w.f1.dw <Tab> [list focus $w.f1.in]

   bind $w.f1.in <Return> {

      if {$ntcase > $maxtcase} {
	   set ntcase $maxtcase
	} elseif { $ntcase < 1 } {
	   set ntcase 1
      }

      if {[string match {[0-9]*} $ntcase]} {
         Look_Show_Tcase .proteum.tcase.m.dlg
      }
      if {$ntcase == 1} {
          .proteum.tcase.m.dlg.f1.up config -state normal;   # Button UP normal
          .proteum.tcase.m.dlg.f1.dw config -state disabled; # Button DW disabled
      } elseif {$ntcase == $maxtcase} {
          .proteum.tcase.m.dlg.f1.up config -state disabled; # Button UP disabled
          .proteum.tcase.m.dlg.f1.dw config -state normal;   # Button DW normal
      } else {
          .proteum.tcase.m.dlg.f1.up config -state normal;   # Button UP normal
          .proteum.tcase.m.dlg.f1.dw config -state normal;   # Button DW normal
      }
   }


   wm protocol $w WM_DELETE_WINDOW {
      set ok 0
      destroy .proteum.tcase.m.dlg
   }
   grab $w
   tkwait variable ok
   grab release $w 
}


#------------------------------------------------------------------------------#
#   LOOK_SHOW_TCASE {}
#   Looks for and shows information of test case. 
#   date: 04/30/96
#   last update: 04/30/96
#------------------------------------------------------------------------------#

proc Look_Show_Tcase {w} {
   global tcase;       # Informations about test case
   global ntcase;      # Number of current test case


   Set_Busy ON
   set out [Tcase VIEW  $ntcase]

   if {[string compare $out {}] == 0} {
      return 0
   }
   Disjoin_Inf_Tcase $out

   if {[string compare $tcase(status) ENABLED] == 0} {
      .proteum.tcase.m.dlg.f2.r1 select
   } else {
      .proteum.tcase.m.dlg.f2.r2 select
   }

   $w.f3.l2 config -text $tcase(param)
   $w.f4.l2 config -text $tcase(exec)
   $w.f5.l2 config -text $tcase(rcode)

   # Enable text areas
   $w.f7.t  config -state normal
   $w.f8.t  config -state normal
   $w.f12.t config -state normal

   $w.f7.t  delete 1.0 end;          # Cleans input text area
   $w.f7.t  insert end $tcase(in);   # Shows input information
   $w.f8.t  delete 1.0 end;          # Cleans output text area
   $w.f8.t  insert end $tcase(out);  # Shows output information
   $w.f12.t delete 1.0 end;          # Cleans error text area
   $w.f12.t insert end $tcase(err);  # Shows error information

   # Disables text areas
   $w.f7.t  config -state disabled
   $w.f8.t  config -state disabled
   $w.f12.t config -state disabled

   Set_Busy OFF
   return 1
}


#------------------------------------------------------------------------------#
#   TCASE_EFFECTIVE_WINDOW {}  
#   Deletes or disables non-effective test cases.
#   date: 11/07/96
#   last update: 11/07/96
#------------------------------------------------------------------------------#

proc Tcase_Effective_Window {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Effective Test Case"
   wm iconname $w "Delete"
   wm geometry $w +10+65
   wm minsize  $w 300 130
   if { $FIXEDSIZE } {
   	wm maxsize  $w 300 130
   	wm geometry $w 300x130
   }

   frame $w.f1
   pack  $w.f1 -pady 10
   label $w.f1.l -text "Choose \"Disable\" to disable non-effective 
test cases, \"Delete\" to delete non-effective 
test cases or \"Cancel\" to 
cancel this operation."
   pack $w.f1.l

   frame  $w.f2
   pack   $w.f2 -pady 5
   button $w.f2.disable -text Disable -command {
          set confirm 1
          destroy .proteum.tcase.m.dlg
          List_Good DISABLE 
   }

   button $w.f2.delete -text Delete -command {
          set confirm 1
          destroy .proteum.tcase.m.dlg
          List_Good DELETE
   }

   button $w.f2.cancel -text Cancel -command {
          set confirm 0
          destroy .proteum.tcase.m.dlg
   }
   pack   $w.f2.disable $w.f2.delete $w.f2.cancel -side left 

   bind $w <Any-Motion> {focus %W}

   wm protocol $w WM_DELETE_WINDOW {
      set confirm 0
      destroy .proteum.tcase.m.dlg
   }
   grab $w
   tkwait variable confirm
   grab release $w 
}



#------------------------------------------------------------------------------#
#   TCASE_DELETE_WINDOW {}  
#   Delete test case.
#   date: 02/27/96
#   last update: 05/10/96, 09/15/97
#------------------------------------------------------------------------------#

proc Tcase_Delete_Window {w args} {

   global test
   global in
   global maxtcase
   global FIXEDSIZE

    # Atualize number of test cases
    set infogeneral [Report GENERAL 0]
    if {[string length $infogeneral] == 0} {
       return
    } else {
         Disjoin_Infogeneral $infogeneral PART
    }  

   if {$maxtcase == 0} {
      message "No Test Case to Delete !"
      return
   }


   toplevel $w
   wm title $w "Delete Test Case"
   wm iconname $w "Delete"
   if { $FIXEDSIZE } {
      wm geometry $w 400x80
      wm maxsize  $w 400 80
   }
   wm geometry $w +10+65
   wm minsize  $w 400 80

   set in(from)  1
   set in(to)    1 

   frame $w.f1
   pack  $w.f1 -pady 5
   label $w.f1.l1 -text "From:"
   entry $w.f1.from -width 5 -textvariable in(from)

   # Button for UP and DOWN to number of test case to delete 
   button $w.f1.up1 -text up -command {
      .proteum.tcase.m.dlg.f1.up1 config -state normal
      if {$in(from) < $maxtcase} {
         .proteum.tcase.m.dlg.f1.dw1 config -state normal
         incr in(from)
         
         if {$in(from) == $maxtcase} {
            .proteum.tcase.m.dlg.f1.up1 config -state disabled
         }
      } 
   }

   button $w.f1.dw1 -text dw -command {
      set v .proteum.tcase.m.dlg.f1
      $v.up1 config -state normal
      if {$in(from) > 1} {
         incr in(from) -1
         if {$in(from) == 1} "$v.dw1 config -state disabled"
      } else {
         $v.dw1 config -state disabled
      } 
   }

   label $w.f1.l2 -text "To:"
   entry $w.f1.to -width 5 -textvariable in(to)

   # Button for UP and DOWN to number of mutant to delete 
   button $w.f1.up2 -text up -command {
      .proteum.tcase.m.dlg.f1.up2 config -state normal
      if {$in(to) < $maxtcase} {
         .proteum.tcase.m.dlg.f1.dw2 config -state normal
         incr in(to)
         if {$in(to) == $maxtcase} {
            .proteum.tcase.m.dlg.f1.up2 config -state disabled
         }
      } 
   }
   button $w.f1.dw2 -text dw -command {
      set v .proteum.tcase.m.dlg.f1
      $v.up2 config -state normal
      if {$in(to) > 1} {
         incr in(to) -1
         if {$in(to) == 1} "$v.dw2 config -state disabled"
      } else {
         $v.dw2 config -state disabled
      } 
   }
   pack  $w.f1.l1 $w.f1.from $w.f1.up1 $w.f1.dw1 $w.f1.l2 \
         $w.f1.to $w.f1.up2 $w.f1.dw2 -side left

  
   $w.f1.dw1 config -state disabled
   $w.f1.dw2 config -state disabled


   frame  $w.f2
   pack   $w.f2 -pady 5
   button $w.f2.confirm -text Confirm -command {
      if {$in(from) == {} || $in(from) > $maxtcase || $in(to) > $maxtcase} {
         message "Invalid Value for Number of Test Case !"
         return
      } elseif {$in(to) == {}} {
         Tcase DELETE $in(from) $in(from)
      } elseif {$in(from) < $in(to)} {
         Tcase DELETE $in(from) $in(to)
      } else {
         Tcase DELETE $in(to) $in(from)
      }

      # Atualize number of test cases
      set infogeneral [Report GENERAL 0]
      if {[string length $infogeneral] == 0} {
         return
      } else {
         Disjoin_Infogeneral $infogeneral PART
      }  



      set confirm 1
      destroy .proteum.tcase.m.dlg
   }

   button $w.f2.cancel  -text Cancel  -command "
          set confirm 0
          destroy $w"
   pack   $w.f2.confirm $w.f2.cancel -side left 


   bind $w <Any-Motion> {focus %W}

   bind $w.f1.from <Return> [list focus $w.f1.to]
   bind $w.f1.to   <Return> "$w.f2.confirm invoke"

   wm protocol $w WM_DELETE_WINDOW {
      set confirm 0
      destroy .proteum.tcase.m.dlg
   }
   grab $w
   tkwait variable confirm
   grab release $w 
}




#------------------------------------------------------------------------------#
#   TCASE_IMP_PROTEUM_WINDOW {}  
#   Import test case from Proteum.
#   date: 02/27/96
#   last update: 05/10/96, 09/16/97
#------------------------------------------------------------------------------#

proc Tcase_Imp_Proteum_Window {w args} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Import Proteum Test Case"
   wm iconname $w "IProteum"
   wm geometry $w +10+65
   wm minsize  $w 490 130
   if { $FIXEDSIZE } {
   	wm maxsize  $w 490 130
   	wm geometry $w 490x130
   }

   global ddirectory \
          in
  
   set in(dir)   [Set_Directory $ddirectory];  # Sets directory name
   set in(tfile) {};                           # Sets test file name
   set in(from)  1
   set in(to)    1

   frame $w.f1
   pack  $w.f1 -fill x
   label $w.f1.l -text "Directory:" 
   entry $w.f1.in -width 50 -textvariable in(dir)
   pack  $w.f1.in $w.f1.l -side right 

   frame $w.f2
   pack  $w.f2 -fill x
   label $w.f2.l -text "Program Test Name:" 
   entry $w.f2.in -width 50 -textvariable in(tfile)
   pack  $w.f2.in $w.f2.l -side right

   frame $w.f4
   pack  $w.f4 -fill x
   label $w.f4.l1 -text "                       From:"
   entry $w.f4.from -width 5 -textvariable in(from)

   # Button for UP and DOWN to number of mutant to import
   button $w.f4.up1 -text up -command {
      .proteum.tcase.m.dlg.f4.dw1 config -state normal
      incr in(from)
   }
   button $w.f4.dw1 -text dw -command {
      set v .proteum.tcase.m.dlg
      $v.f4.up1 config -state normal
      if {$in(from) > 1} {
         incr in(from) -1
         if {$in(from) == 1} "$v.f4.dw1 config -state disabled"
      } else {
         $v.f4.dw1 config -state disabled
      } 
   }

   label $w.f4.l2 -text "To:"
   entry $w.f4.to -width 5 -textvariable in(to)

   # Button for UP and DOWN to number of mutant to import
   button $w.f4.up2 -text up -command {
      .proteum.tcase.m.dlg.f4.dw2 config -state normal
      incr in(to)
   }
   button $w.f4.dw2 -text dw -command {
      set v .proteum.tcase.m.dlg
      $v.f4.up2 config -state normal
      if {$in(to) > 1} {
         incr in(to) -1
         if {$in(to) == 1} "$v.f4.dw2 config -state disabled"
      } else {
         $v.f4.dw2 config -state disabled
      } 
   }

   pack  $w.f4.l1 $w.f4.from $w.f4.up1 $w.f4.dw1 \
         $w.f4.l2 $w.f4.to   $w.f4.up2 $w.f4.dw2 -side left

   $w.f4.dw1 config -state disabled
   $w.f4.dw2 config -state disabled

   frame  $w.f3
   pack   $w.f3 
   button $w.f3.confirm -text Confirm -command "
          set confirm 1
          Confirm_Import_Proteum $w"
   button $w.f3.cancel  -text Cancel  -command "
          set confirm 0
          destroy $w"
   pack   $w.f3.confirm $w.f3.cancel -side left -pady 10

   bind $w <Any-Motion> {focus %W}
   bind $w.f1.in <Return> [list focus $w.f2.in]
   bind $w.f2.in <Return> "$w.f3.confirm invoke"

   wm protocol $w WM_DELETE_WINDOW {
      set confirm 0
      destroy .proteum.tcase.m.dlg
   }
   grab $w
   tkwait variable confirm
   grab release $w 
}



#------------------------------------------------------------------------------#
#   CONFIRM_IMPORT_PROTEUM {}
#   Verify data to import test cases from Proteum.
#   date: 04/02/96
#   last update: 04/02/96, 11/07/96
#------------------------------------------------------------------------------#

proc Confirm_Import_Proteum {w} {

   global test \
          in

   set in(dir)   [Blank_Space_Out $in(dir)]
   set in(tfile) [Blank_Space_Out $in(tfile)]
   set tmpfile $in(dir)/$in(tfile).PTM

   destroy $w

   if {![file isdirectory $in(dir)]} {
      message "This Directory Does Not Exist !"
      return
   } elseif {[string length $in(tfile)] == 0} {
      message "Invalid Program Test Name !"
      return
   } elseif {![file isfile $tmpfile]} {
      message "Program Test Does Not Exist !"
      return
   } else {
       set out [Tcase IMPORT PROTEUM $in(dir) $in(tfile) $in(from) $in(to)]
       message "$out !"
   }
   return
}



#------------------------------------------------------------------------------#
#   TCASE_IMP_POKETOOL_WINDOW {}     
#   Import test case from Poketool.
#   date: 02/27/96
#   last update: 05/10/96, 09/16/97
#------------------------------------------------------------------------------#

proc Tcase_Imp_Poketool_Window {w args} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Import POKE-TOOL Test Case"
   wm iconname $w "IPoketool"
   wm geometry $w +10+65
   wm minsize  $w 430 110
   if { $FIXEDSIZE } {
   	wm maxsize  $w 430 110
   	wm geometry $w 430x110
   }

   global in

   set in(dir) {}
   set in(from)  1
   set in(to)    1

   frame $w.f1
   pack  $w.f1
   label $w.f1.l -text "Directory:" 
   entry $w.f1.in -width 50 -textvariable in(dir) 
   pack  $w.f1.l $w.f1.in -side left 

   frame $w.f3
   pack  $w.f3 -fill x
   label $w.f3.l1 -text "       From:"
   entry $w.f3.from -width 5 -textvariable in(from)

   # Button for UP and DOWN to number of mutant to import
   button $w.f3.up1 -text up -command {
      .proteum.tcase.m.dlg.f3.dw1 config -state normal
      incr in(from)
   }
   button $w.f3.dw1 -text dw -command {
      set v .proteum.tcase.m.dlg
      $v.f3.up1 config -state normal
      if {$in(from) > 1} {
         incr in(from) -1
         if {$in(from) == 1} "$v.f3.dw1 config -state disabled"
      } else {
         $v.f3.dw1 config -state disabled
      } 
   }

   label $w.f3.l2 -text "To:"
   entry $w.f3.to -width 5 -textvariable in(to)

   # Button for UP and DOWN to number of mutant to import
   button $w.f3.up2 -text up -command {
      .proteum.tcase.m.dlg.f3.dw2 config -state normal
      incr in(to)
   }
   button $w.f3.dw2 -text dw -command {
      set v .proteum.tcase.m.dlg
      $v.f3.up2 config -state normal
      if {$in(to) > 1} {
         incr in(to) -1
         if {$in(to) == 1} "$v.f3.dw2 config -state disabled"
      } else {
         $v.f3.dw2 config -state disabled
      } 
   }

   pack  $w.f3.l1 $w.f3.from $w.f3.up1 $w.f3.dw1 \
         $w.f3.l2 $w.f3.to   $w.f3.up2 $w.f3.dw2 -side left

   $w.f3.dw1 config -state disabled
   $w.f3.dw2 config -state disabled

   frame  $w.f2
   pack   $w.f2
   button $w.f2.confirm -text Confirm -command "
          set confirm 1
          Confirm_Import_Poketool $w"
   button $w.f2.cancel  -text Cancel  -command "
          set confirm 0
          destroy $w"
   pack   $w.f2.confirm $w.f2.cancel -side left -pady 10

   bind $w <Any-Motion> {focus %W}
   bind $w.f1.in <Return> "$w.f2.confirm invoke"

   wm protocol $w WM_DELETE_WINDOW {
      set confirm 0
      destroy .proteum.tcase.m.dlg
   }
   grab $w
   tkwait variable confirm
   grab release $w 
}



#------------------------------------------------------------------------------#
#   CONFIRM_IMPORT_POKETOOL {}
#   Verify data to import test cases from Poke-Tool. 
#   date: 04/02/96
#   last update: 04/02/96
#------------------------------------------------------------------------------#

proc Confirm_Import_Poketool {w} {
 
   global test \
          in

   set in(dir) [Blank_Space_Out $in(dir)]

   if {![file isdirectory $in(dir)]} {
      message {This Directory Does Not Exist !}
      return
   } else {
      destroy $w
      set out [Tcase IMPORT POKE $in(dir) $in(from) $in(to)]
      message "$out !"
   }
   return   
}



#------------------------------------------------------------------------------#
#   TCASE_IMP_ASCII_WINDOW
#   Import test case from ASCII file.
#   date: 02/27/96
#   last update: 05/10/96
#------------------------------------------------------------------------------#

proc Tcase_Imp_Ascii_Window {w args} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Import ASCII Test Case"
   wm iconname $w "IAscii"
   wm geometry $w +10+65
   wm minsize  $w 430 130
   if { $FIXEDSIZE } {
   	wm maxsize  $w 430 130
   	wm geometry $w 430x130
   }

   global ddirectory;   # Default directory
   global in
 
   set in(dir)   [Set_Directory $ddirectory];     # Sets directory default
   set in(tfile) {}
   set in(from)  1
   set in(to)    1

   frame $w.f1
   pack  $w.f1 -fill x
   label $w.f1.l -text "Directory:" 
   entry $w.f1.in -width 50 -textvariable in(dir)
   pack  $w.f1.in $w.f1.l -side right

   frame $w.f2
   pack  $w.f2 -fill x
   label $w.f2.l -text "Input File:" 
   entry $w.f2.in -width 50 -textvariable in(tfile)
   pack  $w.f2.in $w.f2.l -side right 

   frame $w.f5
   pack  $w.f5 -fill x
   label $w.f5.l -text "Parameter File:" 
   entry $w.f5.in -width 50 -textvariable in(pfile)
   pack  $w.f5.in $w.f5.l -side right 

   frame $w.f3
   pack  $w.f3 -fill x
   label $w.f3.l1 -text "From:"
   entry $w.f3.from -width 5 -textvariable in(from)

   # Button for UP and DOWN to number of mutant to import
   button $w.f3.up1 -text up -command {
      .proteum.tcase.m.dlg.f3.dw1 config -state normal
      incr in(from)
   }
   button $w.f3.dw1 -text dw -command {
      set v .proteum.tcase.m.dlg
      $v.f3.up1 config -state normal
      if {$in(from) > 1} {
         incr in(from) -1
         if {$in(from) == 1} "$v.f3.dw1 config -state disabled"
      } else {
         $v.f3.dw1 config -state disabled
      } 
   }

   label $w.f3.l2 -text "To:"
   entry $w.f3.to -width 5 -textvariable in(to)

   # Button for UP and DOWN to number of mutant to import
   button $w.f3.up2 -text up -command {
      .proteum.tcase.m.dlg.f3.dw2 config -state normal
      incr in(to)
   }
   button $w.f3.dw2 -text dw -command {
      set v .proteum.tcase.m.dlg
      $v.f3.up2 config -state normal
      if {$in(to) > 1} {
         incr in(to) -1
         if {$in(to) == 1} "$v.f3.dw2 config -state disabled"
      } else {
         $v.f3.dw2 config -state disabled
      } 
   }

   pack  $w.f3.l1 $w.f3.from $w.f3.up1  $w.f3.dw1 \
         $w.f3.l2 $w.f3.to   $w.f3.up2  $w.f3.dw2 -side left

   $w.f3.dw1 config -state disabled
   $w.f3.dw2 config -state disabled

   frame  $w.f4
   pack   $w.f4 -pady 5
   button $w.f4.confirm -text Confirm -command "
          set confirm 1
          Confirm_Import_Ascii $w"
   button $w.f4.cancel  -text Cancel  -command "
          set confirm 0
          destroy $w"
   pack   $w.f4.confirm $w.f4.cancel -side left

   bind $w <Any-Motion> {focus %W}
   bind $w.f1.in   <Return> [list focus $w.f2.in]
   bind $w.f2.in   <Return> [list focus $w.f5.in] 
   bind $w.f5.in   <Return> [list focus $w.f3.from] 
   bind $w.f3.from <Return> [list focus $w.f3.to] 
   bind $w.f3.to   <Return> "$w.f4.confirm invoke"

   wm protocol $w WM_DELETE_WINDOW {
      set confirm 0
      destroy .proteum.tcase.m.dlg
   }
   grab $w
   tkwait variable confirm
   grab release $w
}



#------------------------------------------------------------------------------#
#   CONFIRM_IMPORT_ASCII {}
#   Verify data to import test cases from Ascii file. 
#   date: 04/02/96
#   last update: 04/02/96
#------------------------------------------------------------------------------#

proc Confirm_Import_Ascii {w} {

   global test \
          in  

   set in(dir)   [Blank_Space_Out $in(dir)]
   set in(tfile) [Blank_Space_Out $in(tfile)]
   set in(pfile) [Blank_Space_Out $in(pfile)]


   set tmp $in(dir)/$in(tfile)
   set tmp2 $in(dir)/$in(pfile)

   if {![file isdirectory $in(dir)]} {
      message {This Directory Does Not Exist !}
      return
   } elseif {[string length $tmp] == 0 && [string length $tmp2] == 0} {
      message "Missing File Name !" 
      return
   } else {
      set out [Tcase IMPORT ASCII $in(dir)  $in(tfile) $in(pfile) \
                                  $in(from) $in(to)]
      message "$out !"
   }
   destroy $w
   return
}


#------------------------------------------------------------------------------#
#   TCASE_MINIMIZE_WINDOW
#   Minimize set of test cases.
#   date: 07/02/96
#   last update: 07/02/96
#------------------------------------------------------------------------------#

proc Tcase_Minimize_Window {w} {
  message "Minimization Function Does Not Available !"
}
