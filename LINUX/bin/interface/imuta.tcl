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
#   IMUTA.TCL
#   Procedures related with MUTANTS options.
#   author: Elisa Yumi Nakagawa
#   date: 02/27/96
#   last update: 05/28/96, 09/25/96, 11/13/96, 11/20/96, 11/25/96
#------------------------------------------------------------------------------#


#------------------------------------------------------------------------------#
#   MUTANTS_GENERATE_WINDOW {}
#
#   date: 11/13/96
#   last update: 11/13/96
#------------------------------------------------------------------------------#

proc Mutants_Generate_Window {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Generate Mutants/Connections"
   wm iconname $w "Connections"
   wm geometry $w +10+65
   wm minsize  $w 450 290
   if { $FIXEDSIZE } {
   wm maxsize  $w 450 290
   wm geometry $w 450x290
   }

   global connection
   global MAXCONNECTIONS
   global in
   global ALL_CONNEC_GEN

   set in(con) ""
   for {} {$MAXCONNECTIONS >= 0 } {set MAXCONNECTIONS \
                                    [expr $MAXCONNECTIONS-1 ]} {
       set connection($MAXCONNECTIONS) ""
   } 
   set MAXCONNECTIONS 0

   frame  $w.f1    
   pack   $w.f1 -pady 5
   label  $w.f1.l -text "Input Connection:"
   pack   $w.f1.l -anchor w
   entry  $w.f1.e -width 70 -textvariable in(con)
   pack   $w.f1.e -anchor w
  
   frame  $w.f2    
   pack   $w.f2 -pady 5
   text   $w.f2.t -yscrollcommand "$w.f2.s set" -width 77 \
                   -height 7 -cursor top_left_arrow -state disabled
   scrollbar $w.f2.s -command "$w.f2.t yview"
   pack $w.f2.s -side right -fill both
   pack $w.f2.t -expand true


   Show_Connection $w.f2.t GENERATE


   frame  $w.f3    
   pack   $w.f3 -pady 5 -fill x
   set ALL_CONNEC_GEN 1

#   checkbutton $w.f3.c1 -text "All Connections" -variable ALL_CONNEC_GEN \
#                        -selectcolor green -cursor hand2
   checkbutton $w.f3.c2 -text "Only Source File Functions" -variable \
                        ONLY_SRC_FUNC -selectcolor green -cursor hand2
   pack $w.f3.c2 -side top -anchor w

#   $w.f3.c1 select
   $w.f3.c2 select


   frame  $w.f4    
   pack   $w.f4 -pady 10 -side bottom

   button $w.f4.add -width 5 -text Add -command {
      if {[Add_Connection]} {
         Show_Connection .proteum.muta.m.dlg.f2.t GENERATE
      }
   }

   button $w.f4.delete -width 5 -text Delete -command {
      Delete_Connection
      Show_Connection .proteum.muta.m.dlg.f2.t GENERATE
   }

   button $w.f4.continue  -text Continue -command {
      destroy .proteum.muta.m.dlg
      Mutants_Generate .proteum.muta.m.dlg
   }

   button $w.f4.cancel -width 5 -text Cancel -command "
      destroy $w
      set confirm 0";         # Variable for user not interact with other window
   pack   $w.f4.add $w.f4.delete $w.f4.continue $w.f4.cancel -side left


   wm protocol $w WM_DELETE_WINDOW {
      set confirm 0
      destroy .proteum.muta.m.dlg
   }
   bind $w <Any-Motion> {focus %W}
   grab $w 
   tkwait variable confirm
   grab release $w
}


#------------------------------------------------------------------------------#
#   MUTANTS_GENERATE_WINDOW2 {}
#
#   date: 24/05/00
#   last update: 24/05/00
#------------------------------------------------------------------------------#

proc Mutants_Generate_Window2 {w} {
   global FIXEDSIZE
   global ONLY_SRC_FUNC

   set ONLY_SRC_FUNC 1
   toplevel $w
   wm title $w "Generate Mutants/Unit"
   wm iconname $w "Units"
   wm geometry $w +10+65
   wm minsize  $w 450 290
   if { $FIXEDSIZE } {
   wm maxsize  $w 450 290
   wm geometry $w 450x290
   }

   global connection
   global MAXCONNECTIONS
   global in

   set in(con) ""
   for {} {$MAXCONNECTIONS >= 0 } {set MAXCONNECTIONS \
                                    [expr $MAXCONNECTIONS-1 ]} {
       set connection($MAXCONNECTIONS) ""
   } 
   set MAXCONNECTIONS 0

   frame  $w.f1    
   pack   $w.f1 -pady 5
   label  $w.f1.l -text "Unit to test:"
   pack   $w.f1.l -anchor w
   entry  $w.f1.e -width 70 -textvariable in(con)
   pack   $w.f1.e -anchor w
  
   frame  $w.f2    
   pack   $w.f2 -pady 5
   text   $w.f2.t -yscrollcommand "$w.f2.s set" -width 77 \
                   -height 7 -cursor top_left_arrow -state disabled
   scrollbar $w.f2.s -command "$w.f2.t yview"
   pack $w.f2.s -side right -fill both
   pack $w.f2.t -expand true


   Show_Connection $w.f2.t GENERATE


   frame  $w.f3    
   pack   $w.f3 -pady 5 -fill x

   set ALL_UNIT_GEN 1
#   checkbutton $w.f3.c1 -text "All units" -variable ALL_UNIT_GEN \
#                        -selectcolor green -cursor hand2
#   pack $w.f3.c1 -side top -anchor w

#   $w.f3.c1 select


   frame  $w.f4    
   pack   $w.f4 -pady 10 -side bottom

   button $w.f4.add -width 5 -text Add -command {
      if {[Add_Unit]} {
         Show_Connection .proteum.muta.m.dlg.f2.t GENERATE
      }
   }

   button $w.f4.delete -width 5 -text Delete -command {
      Delete_Connection
      Show_Connection .proteum.muta.m.dlg.f2.t GENERATE
   }

   button $w.f4.continue  -text Continue -command {
      destroy .proteum.muta.m.dlg
      Mutants_Generate2 .proteum.muta.m.dlg
   }

   button $w.f4.cancel -width 5 -text Cancel -command "
      destroy $w
      set confirm 0";         # Variable for user not interact with other window
   pack   $w.f4.add $w.f4.delete $w.f4.continue $w.f4.cancel -side left


   wm protocol $w WM_DELETE_WINDOW {
      set confirm 0
      destroy .proteum.muta.m.dlg
   }
   bind $w <Any-Motion> {focus %W}
   grab $w 
   tkwait variable confirm
   grab release $w
}


#------------------------------------------------------------------------------#
#   MUTANTS_GENERATE {}
#
#   date: 02/27/96
#   last update: 04/03/96, 11/07/96
#------------------------------------------------------------------------------#


proc Mutants_Generate {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Interface Mutants Generation"
   wm iconname $w "Generation"
   wm geometry $w +10+65
   wm minsize  $w 250 100
   if { $FIXEDSIZE } {
   	wm maxsize  $w 250 100
    	wm geometry $w 250x100
   }
   set infogeneral [Report GENERAL 0]

   # If error building report, return 
   if {[string length $infogeneral] == 0} {
      return
   } else {
       Disjoin_Infogeneral $infogeneral ALL
   }  

   frame $w.f1 
   pack  $w.f1 -fill x -pady 10

   frame  $w.f1.f1 
   frame  $w.f1.f2
   pack   $w.f1.f1 $w.f1.f2 -side left

   label $w.f1.f1.l -text "Classes:" 
   pack  $w.f1.f1.l 


   menubutton $w.f1.f2.r -menu $w.f1.f2.r.m -relief raised -cursor hand2
   pack $w.f1.f2.r -side left  
      menu $w.f1.f2.r.m -tearoff 0

      # if option "Group I" was choosen in menu
      $w.f1.f2.r.m add command -label "Group I" -command { \
         set v .proteum.muta.m.dlg

         if {![winfo exists $v.group1]} {
            $v.f1.f2.l configure -text "Group I"
            Group_I_Operator $v.group1
         }
         # Destroy window of other class mutation operators
         if {[winfo exists $v.group2]} {
            destroy $v.group2
         }
      }


      # if option "Group II" was choosen in menu
      $w.f1.f2.r.m add command -label "Group II" -command { \
         set v .proteum.muta.m.dlg

         if {![winfo exists $v.group2]} {
            $v.f1.f2.l configure -text "Group II"
            Group_II_Operator $v.group2    
         }
         # Destroy window of other class mutation operators
         if {[winfo exists $v.group1]} {
            destroy $v.group1
         } 
      }
    

   label  $w.f1.f2.l -text "Group I" 
   pack   $w.f1.f2.l -side left
   Group_I_Operator $w.group1

   frame  $w.f2    
   pack   $w.f2 -pady 5
   button $w.f2.confirm -text Generate -command {
      # Destroy mutation operator window
      set v .proteum.muta.m.dlg  
      if {[winfo exists $v.group1]} {
         destroy $v.group1
      } elseif {[winfo exists $v.group2]} {
         destroy $v.group2
      }  
      set confirm 1;          # Variable for user not interact with other window
      Generate_Mutants
   }

   button $w.f2.cancel  -text Cancel -command "
      destroy $w
      set confirm 0";         # Variable for user not interact with other window
   pack   $w.f2.confirm $w.f2.cancel -side left

   wm protocol $w WM_DELETE_WINDOW {
      set confirm 0
      destroy .proteum.muta.m.dlg
   }
 
   bind $w <Any-Motion> {focus %W}

   grab $w 
   tkwait variable confirm
   grab release $w
}

#------------------------------------------------------------------------------#
#   MUTANTS_GENERATE2 {}
#
#   date: 24/05/00
#   last update: 24/05/00
#------------------------------------------------------------------------------#


proc Mutants_Generate2 {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Unit Mutants Generation"
   wm iconname $w "Generation"
   wm geometry $w +10+65
   wm minsize  $w 250 100
   if { $FIXEDSIZE } {
   	wm maxsize  $w 250 100
    	wm geometry $w 250x100
   }
   set infogeneral [Report GENERAL 0]

   # If error building report, return 
   if {[string length $infogeneral] == 0} {
      return
   } else {
       Disjoin_Infogeneral $infogeneral ALL
   }  

   frame $w.f1 
   pack  $w.f1 -fill x -pady 10

   frame  $w.f1.f1 
   frame  $w.f1.f2
   pack   $w.f1.f1 $w.f1.f2 -side left

   label $w.f1.f1.l -text "Classes:" 
   pack  $w.f1.f1.l 


   menubutton $w.f1.f2.r -menu $w.f1.f2.r.m -relief raised -cursor hand2
   pack $w.f1.f2.r -side left  
      menu $w.f1.f2.r.m -tearoff 0

      # if option "Operators" was choosen in menu
      $w.f1.f2.r.m add command -label "Operators" -command { \
         set v .proteum.muta.m.dlg

         if {![winfo exists $v.operators]} {
            $v.f1.f2.l configure -text "Operators"
            Operators_Operator $v.operators
         }
         # Destroy window of other class mutation operators
         if {[winfo exists $v.statements]} {
            destroy $v.statements
         }
         if {[winfo exists $v.variables]} {
            destroy $v.variables
         }
         if {[winfo exists $v.constants]} {
            destroy $v.constants
         }

      }

      # if option "Statements" was choosen in menu
      $w.f1.f2.r.m add command -label "Statements" -command { \
         set v .proteum.muta.m.dlg

         if {![winfo exists $v.statements]} {
            $v.f1.f2.l configure -text "Statements"
            Statements_Operator $v.statements
         }
         # Destroy window of other class mutation operators
         if {[winfo exists $v.operators]} {
            destroy $v.operators
         }
         if {[winfo exists $v.variables]} {
            destroy $v.variables
         }
         if {[winfo exists $v.constants]} {
            destroy $v.constants
         }

      }

      # if option "Variables" was choosen in menu
      $w.f1.f2.r.m add command -label "Variables" -command { \
         set v .proteum.muta.m.dlg

         if {![winfo exists $v.variables]} {
            $v.f1.f2.l configure -text "Variables"
            Variables_Operator $v.variables
         }
         # Destroy window of other class mutation operators
         if {[winfo exists $v.operators]} {
            destroy $v.operators
         }
         if {[winfo exists $v.statements]} {
            destroy $v.statements
         }
         if {[winfo exists $v.constants]} {
            destroy $v.constants
         }

      }

      # if option "Constants" was choosen in menu
      $w.f1.f2.r.m add command -label "Constants" -command { \
         set v .proteum.muta.m.dlg

         if {![winfo exists $v.constants]} {
            $v.f1.f2.l configure -text "Constants"
            Constants_Operator $v.constants
         }
         # Destroy window of other class mutation operators
         if {[winfo exists $v.operators]} {
            destroy $v.operators
         }
         if {[winfo exists $v.variables]} {
            destroy $v.variables
         }
         if {[winfo exists $v.statements]} {
            destroy $v.statements
         }

      }

    

   label  $w.f1.f2.l -text "Operators" 
   pack   $w.f1.f2.l -side left
   Operators_Operator $w.operators

   frame  $w.f2    
   pack   $w.f2 -pady 5
   button $w.f2.confirm -text Generate -command {
      # Destroy mutation operator window
      set v .proteum.muta.m.dlg  
      if {[winfo exists $v.operators]} {
         destroy $v.operators
      } elseif {[winfo exists $v.statements]} {
         destroy $v.statements
      } elseif {[winfo exists $v.variables]} {
         destroy $v.variables
      } elseif {[winfo exists $v.constants]} {
         destroy $v.constants
      }  
      set confirm 1;          # Variable for user not interact with other window
      Generate_Mutants2
   }

   button $w.f2.cancel  -text Cancel -command "
      destroy $w
      set confirm 0";         # Variable for user not interact with other window
   pack   $w.f2.confirm $w.f2.cancel -side left

   wm protocol $w WM_DELETE_WINDOW {
      set confirm 0
      destroy .proteum.muta.m.dlg
   }
 
   bind $w <Any-Motion> {focus %W}

   grab $w 
   tkwait variable confirm
   grab release $w
}


#------------------------------------------------------------------------------#
#  GENERATE_MUTANTS {}
#  Generate mutants using mutantion operators percentage.
#  date: 04/08/96
#  last update: 04/11/96
#------------------------------------------------------------------------------#


proc Generate_Mutants {} {
   global ONLY_SRC_FUNC
   global ALL_CONNEC_GEN
   destroy .proteum.muta.m.dlg;          # Destroy last window
   set g_tot 0;                          # Total of generated mutants
 
   Set_Busy ON

   set g_tot 0

   set listconnect [Join_Connection $ALL_CONNEC_GEN]
   if {$ONLY_SRC_FUNC != 0 } {
      set listconnect "$listconnect -r "
   }

   set listopt [Join_Operator_Percentage_Maxmut 1]
   if {[string length $listopt] > 0} {
      set listopt "$listopt $listconnect"
      set mutgen [Muta ADD $listopt]
      set g_tot $mutgen
   }

   set listopt [Join_Operator_Percentage_Maxmut 2]
   if {[string length $listopt] > 0} {
      set listopt "$listopt $listconnect"
      set mutgen [Muta ADD $listopt]
      set g_tot [expr $g_tot + $mutgen]
   }

   Set_Busy OFF

#   message "$g_tot Mutants Generated"
   set infogeneral [Report GENERAL 0]

   # If error building report, return 
   if {[string length $infogeneral] == 0} {
      return
   } else {
       Disjoin_Infogeneral $infogeneral PART
   }    
}

#------------------------------------------------------------------------------#
#  GENERATE_MUTANTS2 {}
#  Generate mutants using mutantion operators percentage.
#  date: 8/8/00
#  last update: 8/8/00
#------------------------------------------------------------------------------#


proc Generate_Mutants2 {} {
   global ALL_UNIT_GEN
   destroy .proteum.muta.m.dlg;          # Destroy last window
   set g_tot 0;                          # Total of generated mutants

   Set_Busy ON

   set listconnect [Join_Unit $ALL_UNIT_GEN]

   set listopt [Join_Operator_Percentage_Maxmut 3]
   if {[string length $listopt] > 0} {
      set listopt "$listopt $listconnect"
      set mutgen [Muta ADD $listopt]
      set g_tot $mutgen
   }

   set listopt [Join_Operator_Percentage_Maxmut 4]
   if {[string length $listopt] > 0} {
      set listopt "$listopt $listconnect"
      set mutgen [Muta ADD $listopt]
      set g_tot [expr $g_tot + $mutgen]
   }

   set listopt [Join_Operator_Percentage_Maxmut 5]
   if {[string length $listopt] > 0} {
      set listopt "$listopt $listconnect"
      set mutgen [Muta ADD $listopt]
      set g_tot [expr $g_tot + $mutgen]
   }

   set listopt [Join_Operator_Percentage_Maxmut 6]
   if {[string length $listopt] > 0} {
      set listopt "$listopt $listconnect"
      set mutgen [Muta ADD $listopt]
      set g_tot [expr $g_tot + $mutgen]
   }

   Set_Busy OFF

#   message "$g_tot Mutants Generated"
   set infogeneral [Report GENERAL 0]

   # If error building report, return
   if {[string length $infogeneral] == 0} {
      return
   } else {
       Disjoin_Infogeneral $infogeneral PART
   }
}


#------------------------------------------------------------------------------#
#   GROUP_I_OPERATOR {}
#   Shows Group II operators list to select percentage and the number of mutants
#   to generate.
#   date: 02/29/96
#   last update: 11/07/96
#------------------------------------------------------------------------------#


proc Group_I_Operator {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Group I Mutation"
   wm iconname $w "GII_Mut"
   wm geometry $w 552x300+10+198
#   wm maxsize  $w 552 300

   global group1_data; # Names of the mutation operators 
   global group1_n;    # Percentage of mutations operator
   global group1_m;    # Number of mutants to be generated in each mut. point
   global group1_o;    # Number of generated mutants
   global in

   set tot       0;    # Total of generated mutants
   set in(def_n) 100
   set in(def_m) 0


   frame $w.f1
   pack  $w.f1 -fill x -padx 50

   button $w.f1.default -text "Apply Default" -command "
      Apply_Default GROUP1 GENERATE"
   label  $w.f1.l1 -text "Percentage:"
   entry  $w.f1.n -width 4 -justify right -textvariable in(def_n)
   label  $w.f1.l2 -text "Limit:"
   entry  $w.f1.m -width 4 -justify right -textvariable in(def_m)
   pack   $w.f1.m $w.f1.l2 $w.f1.n $w.f1.l1 $w.f1.default -side right -pady 10
 
   # Create text area to insert operators name, in and out field 
   text $w.t -yscrollcommand "$w.s set" -relief sunken -bd 1 -state disabled \
             -cursor top_left_arrow
   scrollbar $w.s -command "$w.t yview"
   pack $w.s -side right -fill both
   pack $w.t -expand yes 

   set i 1
   foreach item $group1_data {
      frame $w.t.$i 
      pack  $w.t.$i 
      $w.t window create end -window $w.t.$i

      label $w.t.$i.l -text $item -width 58 -anchor w
      pack  $w.t.$i.l -side left

      entry $w.t.$i.n -width 4 -justify right -textvariable group1_n($i)
      entry $w.t.$i.m -width 4 -justify right -textvariable group1_m($i)
      label $w.t.$i.o -width 4 -relief sunken -cursor top_left_arrow

      pack  $w.t.$i.n $w.t.$i.m $w.t.$i.o -side left


      $w.t.$i.o config -text $group1_o($i) -anchor e
      incr i
   }

   frame $w.t.f1 
   pack  $w.t.f1
   $w.t window create end -window $w.t.f1 -pady 10

#   label $w.t.f1.l -width 48 -text {TOTAL:  } -anchor e
#   label $w.t.f1.out -width 4 -text $tot -relief sunken -anchor e  
#   pack  $w.t.f1.l $w.t.f1.out -side left

   bind $w <Any-Motion> {focus %W}
}


 

#------------------------------------------------------------------------------#
#   GROUP_II_OPERATOR {}
#   Shows Operator Mutations operators list to select percentage.
#   date: 02/29/96
#   last update: 04/17/96
#------------------------------------------------------------------------------#

proc Group_II_Operator {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Group II Mutants"
   wm iconname $w "GII_Mut"
   wm geometry $w 500x260+10+198
#   wm maxsize $w 500 260

   global group2_data; # Names of the mutation operators 
   global group2_n;    # Percentage of mutations operator
   global group2_m;    # Number of mutants to generate
   global group2_o;    # Number of generated mutants
   global in

   set tot       0;     # Total of generated mutants
   set in(def_n) 100
   set in(def_m) 0

   frame $w.f1
   pack  $w.f1 -fill x -padx 50

   button $w.f1.default -text "Apply Default" -command "
      Apply_Default GROUP2 GENERATE"
   label  $w.f1.l1 -text "Percentage:"
   entry  $w.f1.n -width 4 -justify right -textvariable in(def_n)
   label  $w.f1.l2 -text "Limit:"
   entry  $w.f1.m -width 4 -justify right -textvariable in(def_m)
   pack   $w.f1.m $w.f1.l2 $w.f1.n $w.f1.l1 $w.f1.default -side right -pady 10
 
   # Button for UP and DOWN of default percentage 
   button $w.f1.up -text up -command {
      set v .proteum.muta.m.dlg.group2.f1

      $v.dw config -state normal
      if {$default < 100} {
         incr default
         if {$default == 100} "$v.up config -state disabled"
      } else {
         $v.up config -state disabled
      }
   }
   button $w.f1.dw -text dw -command {
      set v .proteum.muta.m.dlg.group2.f1

      $v.up config -state normal
      if {$default > 0} {
         incr default -1
         if {$default == 0} "$v.dw config -state disabled"
      } else {
         $v.dw config -state disabled
      } 
   }


   # Create text area to insert operators name, in and out field 
   text $w.t -relief sunken -bd 1 -state disabled \
             -cursor top_left_arrow
#   text $w.t -yscrollcommand "$w.s set" -relief sunken -bd 1 -state disabled \
             -cursor top_left_arrow
#   scrollbar $w.s -command "$w.t yview"
#   pack $w.s -side right -fill both
   pack $w.t -expand yes 

   set i 1
   foreach item $group2_data {
      frame $w.t.$i 
      pack  $w.t.$i 
      $w.t window create end -window $w.t.$i

      label $w.t.$i.l -text $item -width 50 -anchor w
      pack  $w.t.$i.l -side left

      entry $w.t.$i.n -width 4 -justify right -textvariable group2_n($i)
      entry $w.t.$i.m -width 4 -justify right -textvariable group2_m($i)
      label $w.t.$i.o -width 4 -relief sunken -cursor top_left_arrow

      pack  $w.t.$i.n $w.t.$i.m $w.t.$i.o -padx 3 -side left


      $w.t.$i.o config -text $group2_o($i) -anchor e
      incr i
   }

   frame $w.t.f1 
   pack  $w.t.f1
   $w.t window create end -window $w.t.f1 -pady 10

   bind $w <Any-Motion> {focus %W}
}

#------------------------------------------------------------------------------#
#   OPERATORS_OPERATOR {}
#   Shows Operator Mutations operators list to select percentage.
#   date: 11/06/00
#   last update: 11/06/00
#------------------------------------------------------------------------------#

proc Operators_Operator {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Operators Unit Mutants"
   wm iconname $w "Opera_Mut"
   wm geometry $w 500x300+10+198
#   wm maxsize  $w 500 300

   global oper_data; # Names of the mutation operators 
   global oper_n;    # Percentage of mutations operator
   global oper_m;    # Number of mutants to generate
   global oper_o;    # Number of generated mutants
   global in

   set tot       0;     # Total of generated mutants
   set in(def_n) 100
   set in(def_m) 0

   frame $w.f1
   pack  $w.f1 -fill x -padx 50

   button $w.f1.default -text "Apply Default" -command "
      Apply_Default OPERA GENERATE"
   label  $w.f1.l1 -text "Percentage:"
   entry  $w.f1.n -width 4 -justify right -textvariable in(def_n)
   label  $w.f1.l2 -text "Limit:"
   entry  $w.f1.m -width 4 -justify right -textvariable in(def_m)
   pack   $w.f1.m $w.f1.l2 $w.f1.n $w.f1.l1 $w.f1.default -side right -pady 10
 


   # Create text area to insert operators name, in and out field 
#   text $w.t -relief sunken -bd 1 -state disabled \
             -cursor top_left_arrow
   text $w.t -yscrollcommand "$w.s set" -relief sunken -bd 1 -state disabled \
             -cursor top_left_arrow
   scrollbar $w.s -command "$w.t yview"
   pack $w.s -side right -fill both
   pack $w.t -expand yes 

   set i 1
   foreach item $oper_data {
      frame $w.t.$i 
      pack  $w.t.$i 
      $w.t window create end -window $w.t.$i

      label $w.t.$i.l -text $item -width 50 -anchor w
      pack  $w.t.$i.l -side left

      entry $w.t.$i.n -width 4 -justify right -textvariable oper_n($i)
      entry $w.t.$i.m -width 4 -justify right -textvariable oper_m($i)
      label $w.t.$i.o -width 4 -relief sunken -cursor top_left_arrow

      pack  $w.t.$i.n $w.t.$i.m $w.t.$i.o -padx 3 -side left


       $w.t.$i.o config -text $oper_o($i) -anchor e
      incr i
   }

   frame $w.t.f1 
   pack  $w.t.f1
   $w.t window create end -window $w.t.f1 -pady 10

   bind $w <Any-Motion> {focus %W}
}

#------------------------------------------------------------------------------#
#   STATEMENTS_OPERATOR {}
#   Shows Statement Mutations operators list to select percentage.
#   date: 8/8/00
#   last update: 8/8/00
#------------------------------------------------------------------------------#

proc Statements_Operator {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Statements Unit Mutants"
   wm iconname $w "Stat_Mut"
   wm geometry $w 430x300+10+198
#   wm maxsize  $w 430 300

   global stat_data; # Names of the mutation operators
   global stat_n;    # Percentage of mutations operator
   global stat_m;    # Number of mutants to generate
   global stat_o;    # Number of generated mutants
   global in

   set tot       0;     # Total of generated mutants
   set in(def_n) 100
   set in(def_m) 0

   frame $w.f1
   pack  $w.f1 -fill x -padx 30

   button $w.f1.default -text "Apply Default" -command "
      Apply_Default STAT GENERATE"
   label  $w.f1.l1 -text "Percentage:"
   entry  $w.f1.n -width 4 -justify right -textvariable in(def_n)
   label  $w.f1.l2 -text "Limit:"
   entry  $w.f1.m -width 4 -justify right -textvariable in(def_m)
   pack   $w.f1.m $w.f1.l2 $w.f1.n $w.f1.l1 $w.f1.default -side right -pady 10



   # Create text area to insert operators name, in and out field
#   text $w.t -relief sunken -bd 1 -state disabled \
#             -cursor top_left_arrow
   text $w.t -yscrollcommand "$w.s set" -relief sunken -bd 1 -state disabled \
             -cursor top_left_arrow
   scrollbar $w.s -command "$w.t yview"
   pack $w.s -side right -fill both
   pack $w.t -expand yes

   set i 1
   foreach item $stat_data {
      frame $w.t.$i
      pack  $w.t.$i
      $w.t window create end -window $w.t.$i

      label $w.t.$i.l -text $item -width 40 -anchor w
      pack  $w.t.$i.l -side left

      entry $w.t.$i.n -width 4 -justify right -textvariable stat_n($i)
      entry $w.t.$i.m -width 4 -justify right -textvariable stat_m($i)
      label $w.t.$i.o -width 4 -relief sunken -cursor top_left_arrow

      pack  $w.t.$i.n $w.t.$i.m $w.t.$i.o -padx 3 -side left


      $w.t.$i.o config -text $stat_o($i) -anchor e
      incr i
   }

   frame $w.t.f1
   pack  $w.t.f1
   $w.t window create end -window $w.t.f1 -pady 10

   bind $w <Any-Motion> {focus %W}
}

#------------------------------------------------------------------------------#
#   VARIABLES_OPERATOR {}
#   Shows Statement Mutations operators list to select percentage.
#   date: 8/8/00
#   last update: 8/8/00
#------------------------------------------------------------------------------#

proc Variables_Operator {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Variables Unit Mutants"
   wm iconname $w "Var_Mut"
   wm geometry $w 430x300+10+198
#   wm maxsize  $w 430 300

   global var_data; # Names of the mutation operators
   global var_n;    # Percentage of mutations operator
   global var_m;    # Number of mutants to generate
   global var_o;    # Number of generated mutants
   global in

   set tot       0;     # Total of generated mutants
   set in(def_n) 100
   set in(def_m) 0

   frame $w.f1
   pack  $w.f1 -fill x -padx 30

   button $w.f1.default -text "Apply Default" -command "
      Apply_Default VAR GENERATE"
   label  $w.f1.l1 -text "Percentage:"
   entry  $w.f1.n -width 4 -justify right -textvariable in(def_n)
   label  $w.f1.l2 -text "Limit:"
   entry  $w.f1.m -width 4 -justify right -textvariable in(def_m)
   pack   $w.f1.m $w.f1.l2 $w.f1.n $w.f1.l1 $w.f1.default -side right -pady 10



   # Create text area to insert operators name, in and out field
   text $w.t -relief sunken -bd 1 -state disabled \
             -cursor top_left_arrow
#   text $w.t -yscrollcommand "$w.s set" -relief sunken -bd 1 -state disabled \
#             -cursor top_left_arrow
#   scrollbar $w.s -command "$w.t yview"
#   pack $w.s -side right -fill both
   pack $w.t -expand yes

   set i 1
   foreach item $var_data {
      frame $w.t.$i
      pack  $w.t.$i
      $w.t window create end -window $w.t.$i

      label $w.t.$i.l -text $item -width 40 -anchor w
      pack  $w.t.$i.l -side left

      entry $w.t.$i.n -width 4 -justify right -textvariable var_n($i)
      entry $w.t.$i.m -width 4 -justify right -textvariable var_m($i)
      label $w.t.$i.o -width 4 -relief sunken -cursor top_left_arrow

      pack  $w.t.$i.n $w.t.$i.m $w.t.$i.o -padx 3 -side left


      $w.t.$i.o config -text $var_o($i) -anchor e
      incr i
   }

   frame $w.t.f1
   pack  $w.t.f1
   $w.t window create end -window $w.t.f1 -pady 10

   bind $w <Any-Motion> {focus %W}
}

#------------------------------------------------------------------------------#
#   CONSTANTS_OPERATOR {}
#   Shows Statement Mutations operators list to select percentage.
#   date: 8/8/00
#   last update: 8/8/00
#------------------------------------------------------------------------------#

proc Constants_Operator {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Constants Unit Mutants"
   wm iconname $w "Const_Mut"
   wm geometry $w 430x120+10+198
#   wm maxsize  $w 430 120

   global const_data; # Names of the mutation operators
   global const_n;    # Percentage of mutations operator
   global const_m;    # Number of mutants to generate
   global const_o;    # Number of generated mutants
   global in

   set tot       0;     # Total of generated mutants
   set in(def_n) 100
   set in(def_m) 0

   frame $w.f1
   pack  $w.f1 -fill x -padx 30

   button $w.f1.default -text "Apply Default" -command "
      Apply_Default CONST GENERATE"
   label  $w.f1.l1 -text "Percentage:"
   entry  $w.f1.n -width 4 -justify right -textvariable in(def_n)
   label  $w.f1.l2 -text "Limit:"
   entry  $w.f1.m -width 4 -justify right -textvariable in(def_m)
   pack   $w.f1.m $w.f1.l2 $w.f1.n $w.f1.l1 $w.f1.default -side right -pady 10



   # Create text area to insert operators name, in and out field
   text $w.t -relief sunken -bd 1 -state disabled \
             -cursor top_left_arrow
#   text $w.t -yscrollcommand "$w.s set" -relief sunken -bd 1 -state disabled \
#             -cursor top_left_arrow
#   scrollbar $w.s -command "$w.t yview"
#   pack $w.s -side right -fill both
   pack $w.t -expand yes

   set i 1
   foreach item $const_data {
      frame $w.t.$i
      pack  $w.t.$i
      $w.t window create end -window $w.t.$i

      label $w.t.$i.l -text $item -width 40 -anchor w
      pack  $w.t.$i.l -side left

      entry $w.t.$i.n -width 4 -justify right -textvariable const_n($i)
      entry $w.t.$i.m -width 4 -justify right -textvariable const_m($i)
      label $w.t.$i.o -width 4 -relief sunken -cursor top_left_arrow

      pack  $w.t.$i.n $w.t.$i.m $w.t.$i.o -padx 3 -side left


      $w.t.$i.o config -text $const_o($i) -anchor e
      incr i
   }

   frame $w.t.f1
   pack  $w.t.f1
   $w.t window create end -window $w.t.f1 -pady 10

   bind $w <Any-Motion> {focus %W}
}


#------------------------------------------------------------------------------#
#   MUTANTS_VIEW_WINDOW {}
#   View mutants.
#   date: 03/01/96
#   last update: 07/01/96
#------------------------------------------------------------------------------#


proc Mutants_View_Window {w args} {
   global FIXEDFONT
   global demo
   toplevel $w
   wm title $w "View Mutants"
   wm iconname $w "View_Mut"
   wm geometry $w +10+65

   global out
   global f1;                            # Font
   global mutant;                        # Number of current mutant
   global muta;                          # Mutant's information
   global type0 type1 type2 type3 type4; # Type of the mutants
   set mutant 0;                         # Initializes number of current mutant
  
   set linhas 80
   if { $demo } {
     set linhas  50
   }

   frame $w.f1
   frame $w.f1.f1 
   label $w.f1.f1.l1 -text "  Mutant:"
   entry $w.f1.f1.in -width 7 -textvariable mutant
   pack  $w.f1.f1.l1 $w.f1.f1.in -side left

   label $w.f1.f1.l -text "Type to Show:"
   checkbutton $w.f1.f1.c1 -selectcolor green -cursor hand2 -text Alive      \
                           -variable type0
   checkbutton $w.f1.f1.c2 -selectcolor green -cursor hand2 -text Dead       \
                           -variable type1
   checkbutton $w.f1.f1.c3 -selectcolor green -cursor hand2 -text Anomalous  \
                           -variable type2
   checkbutton $w.f1.f1.c4 -selectcolor green -cursor hand2 -text Equivalent \
                           -variable type3
   checkbutton $w.f1.f1.c5 -selectcolor green -cursor hand2 -text Inactive   \
                           -variable type4
   pack $w.f1.f1.c5 $w.f1.f1.c4 $w.f1.f1.c3 \
        $w.f1.f1.c2 $w.f1.f1.c1 $w.f1.f1.l -side right

   # Button for UP and DOWN of number of the mutant
   button $w.f1.f1.up -text up -command {
      set v .proteum.muta.m.dlg.f1.f1


      $v.dw config -state normal
      if {$mutant >= [expr $out(totmut)-1]} {
         set mutant [expr $out(totmut)-2]
      }
      incr mutant

      if {$mutant == $out(totmut)} {
         $v.up config -state disabled
      }
      Set_Busy ON
      Mount_Show_Mutant 1
      Set_Busy OFF
   }

   button $w.f1.f1.dw -text dw -command {
      set v .proteum.muta.m.dlg.f1.f1

      $v.up config -state normal
      if {$mutant <= 0} {
         set mutant 1
      }
      incr mutant -1
      if {$mutant == 0} {
         $v.dw config -state disabled
      }
      Set_Busy ON
      Mount_Show_Mutant -1
      Set_Busy OFF
   }

   pack $w.f1.f1.up $w.f1.f1.dw -side left

   frame $w.f1.f2
   label $w.f1.f2.l1 -text "   Status:"
   label $w.f1.f2.l2 -width 45 -relief sunken -anchor w -font $f1
   checkbutton $w.f1.f2.c -selectcolor green -text Equivalent -variable equiv \
                          -onvalue 1 -offvalue 0 -cursor hand2
   pack $w.f1.f2.l1 $w.f1.f2.l2 $w.f1.f2.c -side left

   frame $w.f1.f3
   pack  $w.f1.f1 $w.f1.f2 $w.f1.f3 -expand true -fill x -padx 10
   label $w.f1.f3.l1 -text "Operator:"
   label $w.f1.f3.l2 -width 57 -relief sunken -anchor w -font $f1
   pack  $w.f1.f3.l1 $w.f1.f3.l2 -side left

   frame $w.f2
   pack  $w.f1 $w.f2    -side top -expand true -fill x -pady 10

   frame  $w.f2.f1
   frame  $w.f2.f1.f1
   label  $w.f2.f1.f1.l -text "Original Program"
#   button $w.f2.f1.f1.b -text Execute -command "Exemuta ORIGINAL"
   pack   $w.f2.f1.f1.l 
#   pack   $w.f2.f1.f1.l $w.f2.f1.f1.b 

   frame $w.f2.f1.f2 
   pack  $w.f2.f1.f1 $w.f2.f1.f2 -side top
   text      $w.f2.f1.f2.t -yscrollcommand "$w.f2.f1.f2.yscroll set" \
                           -width  $linhas -height 25 -bg white \
                           -cursor top_left_arrow -font $FIXEDFONT
   scrollbar $w.f2.f1.f2.yscroll -command "$w.f2.f1.f2.t yview"
   pack      $w.f2.f1.f2.yscroll -side right -fill y
   pack      $w.f2.f1.f2.t -expand true -fill both

   frame $w.f2.f2
   pack  $w.f2.f1 $w.f2.f2 -side left -padx 5

   frame  $w.f2.f2.f1
   label  $w.f2.f2.f1.l -text "Mutant Program"
#   button $w.f2.f2.f1.b -text Execute -command "Exemuta EXECONE $mutant"
   pack   $w.f2.f2.f1.l 
#   pack   $w.f2.f2.f1.l $w.f2.f2.f1.b 

   frame  $w.f2.f2.f2
   pack   $w.f2.f2.f1 $w.f2.f2.f2 -side top
   text      $w.f2.f2.f2.t -yscrollcommand "$w.f2.f2.f2.yscroll set" \
                           -width $linhas  -height 25 -bg white \
                           -cursor top_left_arrow -font $FIXEDFONT
   scrollbar $w.f2.f2.f2.yscroll -command "$w.f2.f2.f2.t yview"
   pack      $w.f2.f2.f2.yscroll -side right -fill y
   pack      $w.f2.f2.f2.t -expand true -fill both

   frame  $w.f3
   pack   $w.f3 
   button $w.f3.ok -text OK -width 10 -command "
          destroy .proteum.muta.m.dlg
          set ok 1"
   pack   $w.f3.ok -side left -pady 10

   # Mutant number 0 is showed
   if {$mutant == 0} {
      $w.f1.f1.dw config -state disabled
      Set_Busy ON
      Mount_Show_Mutant 0
      Set_Busy OFF
   }

   # Equivalent checkbutton is selected
   bind $w.f1.f2.c <ButtonPress> {
      if {$equiv == 0} {
         Muta EQUIV $mutant
      } else {
         Muta NEQUIV $mutant
      }
   }
 
   # When <return> is pressed in field "number of mutant"
   bind $w.f1.f1.in <Return> {
       set v .proteum.muta.m.dlg.f1.f1

       if {$mutant < 0} {
	  set mutant 0
       } elseif {$mutant >= $out(totmut)} {
	  set mutant [expr $out(totmut)-1]
       }

       if {$mutant == 0} {
          $v.up config -state normal
          $v.dw config -state disabled
          Set_Busy ON
          Mount_Show_Mutant 0
          Set_Busy OFF

       }  elseif {$mutant == [expr $out(totmut)-1]} {
          $v.up config -state disabled
          $v.dw config -state normal
          Set_Busy ON
          Mount_Show_Mutant 0
          Set_Busy OFF

       } else {
          $v.up config -state normal
          $v.dw config -state normal
          Set_Busy ON
          Mount_Show_Mutant 0
          Set_Busy OFF
       }
   }
 
   bind $w <Any-Motion> {focus %W}

   wm protocol $w WM_DELETE_WINDOW {
      set ok 1
      destroy .proteum.muta.m.dlg
   }

   grab $w 
   tkwait variable ok
   grab release $w
}



#------------------------------------------------------------------------------#
#   MOUNT_SHOW_MUTANT {}
#   Mounts mutants and its information, and shows them.
#   date: 04/18/96
#   last update: 06/28/96, 11/18/96, 09/12/97
#------------------------------------------------------------------------------#

proc Mount_Show_Mutant {increment} {
   global test
   global mutant;                                 # Number of current mutant
   global muta;                                   # Information of one mutant
   global out;                                    # General inf. about mutants
   global group1_data group2_data;                # Operators' names
   global stat_data const_data var_data oper_data
   global NGROUP1 NGROUP2;                        # Number of operator per class
   global NCONST NOPERA NSTAT NVAR
   global type0 type1 type2 type3 type4;          # Type of the Mutant

   set nline    0

   # OBS: VERIFICAR SE "mutant" CONTEM UM VALOR VALIDO E TIRAR ESPACOS EM BRANCO

   if {$mutant < 0} {
      set mutant [expr $mutant + 1]
      .proteum.muta.m.dlg.f1.f1.dw config -state disabled
      return
   }

   if {$mutant >= $out(totmut)} {
      set mutant [expr $mutant - 1]
      .proteum.muta.m.dlg.f1.f1.up config -state disabled
      return
   } 

   if {$increment >= 0} {
      set aux "-up"
   } else { 
      set aux "-down"
   }

   if {$type0} {
      set aux " $aux -alive"
   }

   if {$type1} {
      set aux " $aux -dead"
   }

   if {$type2} {
      set aux " $aux -anomalous"
   }

   if {$type3} {
      set aux " $aux -equiv"
   }

   if {$type4} {
      set aux " $aux -inactive"
   }

   set aux "$aux -f $mutant "
   set aux1 [Muta SEARCH $aux]
   set aux2 ""

   if {$increment == 0} {
      set aux [string range $aux [expr [string first "-up" $aux]+3] end]
      set aux " -down $aux"
      set aux2 [Muta SEARCH $aux]
   }

   if {[string length $aux1] != 0 && [string length $aux2] != 0} {
      if {[expr $aux1-$mutant] > [expr $mutant-$aux2]} {
         set aux1 $aux2
      }
   } elseif {[string length $aux2] != 0} {
      set aux1 $aux2
   }

   if {[string length $aux1] != 0} {
      set mutant $aux1
   } else {
      set mutant [expr $mutant-$increment]
   }


   set infomuta [Muta LIST $mutant]
   set v .proteum.muta.m.dlg
   Disjoin_Infomuta $infomuta;         # Disjoins mutation information


   # If mutants is equivalent, selects checkbutton
   if {![string compare $muta(status0) "Equivalent"]} {
      $v.f1.f2.l2 config -text "Alive; $muta(status1)"
      $v.f1.f2.c select
   } else {
      $v.f1.f2.l2 config -text "$muta(status0); $muta(status1)"
      $v.f1.f2.c deselect 
   }

   # If mutant is not alive, disable checkbutton
   if {[string compare $muta(status0) "Alive"] == 0 || \
       [string compare $muta(status0) "Equivalent"] == 0} {
      $v.f1.f2.c config -state normal
   } else {
      $v.f1.f2.c config -state disabled
   }

   # Looks for name of mutation operator
   set lim $NGROUP1

   if {$muta(num_op) < $lim} {
       $v.f1.f3.l2 config -text [lindex $group1_data $muta(num_op)]

   } else {
      set lim [expr $lim + $NGROUP2]

   if {$muta(num_op) < $lim} {
      $v.f1.f3.l2 config -text [lindex $group2_data      \
                               [expr $muta(num_op) + $NGROUP2 - $lim]]
   } else {
      set lim [expr $lim + $NCONST]

   if {$muta(num_op) < $lim} {
      $v.f1.f3.l2 config -text [lindex $const_data      \
                               [expr $muta(num_op) + $NCONST - $lim]]
   } else {
      set lim [expr $lim + $NOPERA]

   if {$muta(num_op) < $lim} {
      $v.f1.f3.l2 config -text [lindex $oper_data      \
                               [expr $muta(num_op) + $NOPERA - $lim]]
   } else {
      set lim [expr $lim + $NSTAT]

   if {$muta(num_op) < $lim} {
      $v.f1.f3.l2 config -text [lindex $stat_data      \
                               [expr $muta(num_op) + $NSTAT - $lim]]
   } else {
      set lim [expr $lim + $NVAR]

   if {$muta(num_op) < $lim} {
      $v.f1.f3.l2 config -text [lindex $var_data      \
                               [expr $muta(num_op) + $NVAR - $lim]]
   } else {
      message "Invalid operator number"
   }}}}}}


   if {![Exemuta BUILD $mutant]} {;        # Mounts mutant 
      return;         # Error
   }
   
   set arqf $test(dir)/muta[Blank_Space_Out $mutant]\_$test(tfile)

   set nline [Load_Src_Muta $arqf $v.f2.f1.f2.t  ]
#   HiLight $v.f2.f1.f2.t 0


   # Loads source code of mutant program

   Load_Src_Muta ${arqf}.c \
            $v.f2.f2.f2.t
#   HiLight $v.f2.f2.f2.t 0
   exec rm $arqf ${arqf}.c
}

 

#------------------------------------------------------------------------------#
#   MUTANTS_EQUIVALENTS {}
#
#   date: 96/03/04
#   last update: 96/03/04
#------------------------------------------------------------------------------#


proc Mutants_Equivalents {w} {
   toplevel $w
   wm title $w "Set/Reset Equivalent Mutants"
   wm iconname $w "Equivalent"
   wm geometry $w +10+65

   global equiv nequiv

   set equiv  ""
   set nequiv ""

   frame $w.f1
   pack  $w.f1 -expand true -pady 5
   label $w.f1.l -text "      Set Equivalent:"
   entry $w.f1.in -width 50 -textvariable equiv
   pack  $w.f1.l $w.f1.in -side left

   frame $w.f2
   pack  $w.f2 -expand true
   label $w.f2.l -text "Set Not Equivalent:"
   entry $w.f2.in -width 50 -textvariable nequiv
   pack  $w.f2.l $w.f2.in -side left
  
   frame  $w.f3
   pack   $w.f3 -expand true
   button $w.f3.confirm -text Confirm -command "
          set confirm 1
          Set_Busy ON
          Set_Equiv_Nequiv .proteum.muta.m.dlg
          Set_Busy OFF"
   button $w.f3.cancel  -text Cancel -command "
          set confirm 0
          destroy $w"
   pack   $w.f3.confirm $w.f3.cancel -side left -pady 10

   bind $w <Any-Motion> {focus %W}
   bind $w.f1.in      <Tab>    [list focus $w.f2.in]
   bind $w.f2.in      <Tab>    [list focus $w.f3.confirm]
   bind $w.f3.confirm <Tab>    [list focus $w.f3.cancel]
   bind $w.f3.cancel  <Tab>    [list focus $w.f1.in]
   bind $w.f1.in      <Return> [list focus $w.f2.in]
   bind $w.f2.in      <Return> "$w.f3.confirm invoke"

   wm protocol $w WM_DELETE_WINDOW {
      set confirm 1
      destroy .proteum.muta.m.dlg
   }
   grab $w 
   tkwait variable confirm
   grab release $w
}



#------------------------------------------------------------------------------#
#   SET_EQUIV_NEQUIV {}
#   Sets mutants for equivalent or not equivalent.
#   date: 04/23/96
#   last update: 04/24/96
#------------------------------------------------------------------------------#

proc Set_Equiv_Nequiv {w} { 

   global equiv nequiv
  
   if {[string compare $equiv ""] != 0} {
      if {![Muta EQUIV $equiv]} {
         return 
      }
   }
   if {[string compare $nequiv ""] != 0} {
      if {![Muta NEQUIV $nequiv]} {
         return
      }
   }
   destroy $w
   return 
}



#------------------------------------------------------------------------------#
#   MUTANTS_EXECUTE_WINDOW {}
#   Execute mutants with test cases.
#   date: 03/04/96
#   last update: 05/20/96, 11/27/96, 09/11/97
#------------------------------------------------------------------------------#


proc Mutants_Execute_Window {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Execute"
   wm iconname $w "Ex_mut"
   wm geometry $w +150+200
   wm minsize  $w 350 70
   if { $FIXEDSIZE } {
   	wm maxsize  $w 350 70
   	wm geometry $w 350x70
   }

   global timeout;              # Timeout for mutant execution
   global nmuta;                # Number of current mutant in execution
   global execcan;              # Flag that determine if Cancel was pushed

   set nmuta  0  
   set execcan 0

   frame $w.f1
   pack  $w.f1 -expand true -pady 5

   label $w.f1.l -text "Executing Mutants..."
   entry $w.f1.out -width 10 -textvariable nmuta -justify center
   pack $w.f1.l $w.f1.out -side left -padx 10

   button $w.f1.cancel -text {Cancel} -command "set execcan 1"
   pack $w.f1.cancel -side top -pady 10

   update
   Exemuta EXEC $timeout
   destroy $w
}

#------------------------------------------------------------------------------#
#   MUTANTS_DUALEXECUTE_WINDOW {}
#   Execute mutants with test cases.
#   date: 03/04/96
#   last update: 05/20/96, 11/27/96, 09/11/97
#------------------------------------------------------------------------------#


proc Mutants_DualExecute_Window {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Dual Execute"
   wm iconname $w "Ex_mut"
   wm geometry $w +150+200
   wm minsize  $w 350 70
   if { $FIXEDSIZE } {
   	wm maxsize  $w 350 70
   	wm geometry $w 350x70
   }

   global timeout;              # Timeout for mutant execution
   global nmuta;                # Number of current mutant in execution
   global execcan;              # Flag that determine if Cancel was pushed

   set nmuta  0  
   set execcan 0

   frame $w.f1
   pack  $w.f1 -expand true -pady 5

   label $w.f1.l -text "Executing Mutants..."
   entry $w.f1.out -width 10 -textvariable nmuta -justify center
   pack $w.f1.l $w.f1.out -side left -padx 10

   button $w.f1.cancel -text {Cancel} -command "set execcan 1"
   pack $w.f1.cancel -side top -pady 10

   update
   Exemuta DUALEXEC $timeout
   destroy $w
}

#------------------------------------------------------------------------------#
#   MUTANTS_SELECT_WINDOW {}
#   Selects mutants to be active. Others are marked as inactive and do not 
#   influence at computing mutation score.
#   date: 11/19/96
#   last update: 11/19/96
#------------------------------------------------------------------------------#

proc Mutants_Select_Window {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Select Mutant/Connections"
   wm iconname $w "Connetions"
   wm geometry $w +10+65
#   wm minsize  $w 450 300
   if { $FIXEDSIZE != 0} {
   	wm maxsize  $w 450 300
   	wm geometry $w 450x300
   }

   global connection
   global MAXCONNECTIONS
   global ALL_CONNEC_SEL
   global ALL_MUTANTS
   global in

   set in(con) ""
   for {} {$MAXCONNECTIONS >= 0 } {set MAXCONNECTIONS \
                                    [expr $MAXCONNECTIONS-1 ]} {
       set connection($MAXCONNECTIONS) ""
   }
   set MAXCONNECTIONS 0

  
   frame  $w.f0    
   pack   $w.f0 -fill x -pady 5

   label  $w.f0.l1 -text "Mutants: from"
   entry  $w.f0.f -width 4 -justify right -textvariable in(from)
   label  $w.f0.l2 -text "to"
   entry  $w.f0.t -width 4 -justify right -textvariable in(to)
   pack   $w.f0.l1 $w.f0.f $w.f0.l2 $w.f0.t -side left

   frame  $w.f1
   pack   $w.f1 -pady 5
   label  $w.f1.l -text "Input Connection:"
   pack   $w.f1.l -anchor w
   entry  $w.f1.e -width 70 -textvariable in(con)
   pack   $w.f1.e -anchor w

   frame  $w.f2    
   pack   $w.f2 -pady 5
   text   $w.f2.t -yscrollcommand "$w.f2.s set" -width 77 \
                   -height 7 -cursor top_left_arrow -state disabled
   scrollbar $w.f2.s -command "$w.f2.t yview"
   pack $w.f2.s -side right -fill both
   pack $w.f2.t -expand true

   Show_Connection $w.f2.t SELECT


   set ALL_CONNEC_SEL 1

   frame  $w.f4    
   pack   $w.f4 -pady 10 -side bottom

   button $w.f4.add -width 5 -text Add -command {
      if {[Add_Connection]} {
         Show_Connection .proteum.muta.m.dlg.f2.t GENERATE
      }
   }


   button $w.f4.delete -width 5 -text Delete -command {
      Delete_Connection
      Show_Connection .proteum.muta.m.dlg.f2.t GENERATE
   }

   button $w.f4.continue -text Continue -command {
      destroy .proteum.muta.m.dlg
      Mutants_Select .proteum.muta.m.dlg
   }

   button $w.f4.cancel -width 5 -text Cancel -command "
      destroy $w
      set confirm 0";         # Variable for user not interact with other window
   pack   $w.f4.add $w.f4.delete $w.f4.continue $w.f4.cancel -side left


   wm protocol $w WM_DELETE_WINDOW {
      set confirm 0
      destroy .proteum.muta.m.dlg
   }
   bind $w <Any-Motion> {focus %W}
   grab $w 
   tkwait variable confirm
   grab release $w
}


#------------------------------------------------------------------------------#
#   MUTANTS_SELECT_WINDOW2 {}
#   Selects mutants to be active. Others are marked as inactive and do not
#   influence at computing mutation score.
#   date: 3/9/00
#   last update: 3/9/00
#------------------------------------------------------------------------------#

proc Mutants_Select_Window2 {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Select Mutants/Unit"
   wm iconname $w "Unitss"
   wm geometry $w +10+65
#   wm minsize  $w 450 300
   if { $FIXEDSIZE != 0} {
   	wm maxsize  $w 450 300
   	wm geometry $w 450x300
   }

   global connection
   global MAXCONNECTIONS
   global ALL_CONNEC_SEL
   global in

   set in(con) ""
   for {} {$MAXCONNECTIONS >= 0 } {set MAXCONNECTIONS \
                                    [expr $MAXCONNECTIONS-1 ]} {
       set connection($MAXCONNECTIONS) ""
   }
   set MAXCONNECTIONS 0


   frame  $w.f0
   pack   $w.f0 -fill x -pady 5

   label  $w.f0.l1 -text "Mutants: from"
   entry  $w.f0.f -width 4 -justify right -textvariable in(from)
   label  $w.f0.l2 -text "to"
   entry  $w.f0.t -width 4 -justify right -textvariable in(to)
   pack   $w.f0.l1 $w.f0.f $w.f0.l2 $w.f0.t -side left

   frame  $w.f1
   pack   $w.f1 -pady 5
   label  $w.f1.l -text "Input unit:"
   pack   $w.f1.l -anchor w
   entry  $w.f1.e -width 70 -textvariable in(con)
   pack   $w.f1.e -anchor w

   frame  $w.f2
   pack   $w.f2 -pady 5
   text   $w.f2.t -yscrollcommand "$w.f2.s set" -width 77 \
                   -height 7 -cursor top_left_arrow -state disabled
   scrollbar $w.f2.s -command "$w.f2.t yview"
   pack $w.f2.s -side right -fill both
   pack $w.f2.t -expand true

   Show_Connection $w.f2.t SELECT


   set ALL_UNIT_SEL 1

   frame  $w.f4
   pack   $w.f4 -pady 10 -side bottom

   button $w.f4.add -width 5 -text Add -command {
      if {[Add_Unit]} {
         Show_Connection .proteum.muta.m.dlg.f2.t GENERATE
      }
   }


   button $w.f4.delete -width 5 -text Delete -command {
      Delete_Connection
      Show_Connection .proteum.muta.m.dlg.f2.t GENERATE
   }

   button $w.f4.continue -text Continue -command {
      destroy .proteum.muta.m.dlg
      Mutants_Select2 .proteum.muta.m.dlg
   }

   button $w.f4.cancel -width 5 -text Cancel -command "
      destroy $w
      set confirm 0";         # Variable for user not interact with other window
   pack   $w.f4.add $w.f4.delete $w.f4.continue $w.f4.cancel -side left


   wm protocol $w WM_DELETE_WINDOW {
      set confirm 0
      destroy .proteum.muta.m.dlg
   }
   bind $w <Any-Motion> {focus %W}
   grab $w
   tkwait variable confirm
   grab release $w
}

#------------------------------------------------------------------------------#
#   MUTANTS_SELECT {}
#   Selects mutants to remain active.
#   date: 11/19/96
#   last update: 11/19/96
#------------------------------------------------------------------------------#

proc Mutants_Select {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Mutant Selection"
   wm iconname $w "Selection"
   wm geometry $w +10+65
   wm minsize  $w 250 100
   if { $FIXEDSIZE } {
   	wm maxsize  $w 250 100
    	wm geometry $w 250x100
   }
   set infogeneral [Report GENERAL 0]

   # If error building report, return
   if {[string length $infogeneral] == 0} {
      return
   } else {
       Disjoin_Infogeneral $infogeneral ALL
   }

   frame $w.f1 
   pack  $w.f1 -fill x -pady 10

   frame  $w.f1.f1 
   frame  $w.f1.f2
   pack   $w.f1.f1 $w.f1.f2 -side left

   label $w.f1.f1.l -text "Classes:" 
   pack  $w.f1.f1.l 


   menubutton $w.f1.f2.r -menu $w.f1.f2.r.m -relief raised -cursor hand2
   pack $w.f1.f2.r -side left  
      menu $w.f1.f2.r.m -tearoff 0

      # if option "Group I" was choosen in menu
      $w.f1.f2.r.m add command -label "Group I" -command { \
         set v .proteum.muta.m.dlg

         if {![winfo exists $v.group1]} {
            $v.f1.f2.l configure -text "Group I"
            Group_I_Sel_Operator $v.group1
         }
         # Destroy window of other class mutation operators
         if {[winfo exists $v.group2]} {
            destroy $v.group2
         }
      }

      # if option "Group II" was choosen in menu
      $w.f1.f2.r.m add command -label "Group II" -command { \
         set v .proteum.muta.m.dlg

         if {![winfo exists $v.group2]} {
            $v.f1.f2.l configure -text "Group II"
            Group_II_Sel_Operator $v.group2    
         }
         # Destroy window of other class mutation operators
         if {[winfo exists $v.group1]} {
            destroy $v.group1
         } 
      }
    
   label  $w.f1.f2.l -text "Group I" 
   pack   $w.f1.f2.l -side left
   Group_I_Sel_Operator $w.group1

   frame  $w.f2    
   pack   $w.f2 -pady 5
   button $w.f2.confirm -text Select -command {
      # Destroy mutation operator window
      set v .proteum.muta.m.dlg  
      if {[winfo exists $v.group1]} {
         destroy $v.group1
      } elseif {[winfo exists $v.group2]} {
         destroy $v.group2
      }  
      set confirm 1;          # Variable for user not interact with other window
      Select_Mutants
   }

   button $w.f2.cancel  -text Cancel -command "
      destroy $w
      set confirm 0";         # Variable for user not interact with other window
   pack   $w.f2.confirm $w.f2.cancel -side left

   wm protocol $w WM_DELETE_WINDOW {
      set confirm 0
      destroy .proteum.muta.m.dlg
   }
 
   bind $w <Any-Motion> {focus %W}

   grab $w 
   tkwait variable confirm
   grab release $w
}


#------------------------------------------------------------------------------#
#   MUTANTS_SELECT2 {}
#   Selects mutants to remain active.
#   date: 3/9/00
#   last update: 3/9/00
#------------------------------------------------------------------------------#

proc Mutants_Select2 {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Mutant Selection"
   wm iconname $w "Selection"
   wm geometry $w +10+65
   wm minsize  $w 250 100
   if { $FIXEDSIZE } {
   	wm maxsize  $w 250 100
    	wm geometry $w 250x100
   }
   set infogeneral [Report GENERAL 0]

   # If error building report, return
   if {[string length $infogeneral] == 0} {
      return
   } else {
       Disjoin_Infogeneral $infogeneral ALL
   }

   frame $w.f1
   pack  $w.f1 -fill x -pady 10

   frame  $w.f1.f1
   frame  $w.f1.f2
   pack   $w.f1.f1 $w.f1.f2 -side left

   label $w.f1.f1.l -text "Classes:"
   pack  $w.f1.f1.l


   menubutton $w.f1.f2.r -menu $w.f1.f2.r.m -relief raised -cursor hand2
   pack $w.f1.f2.r -side left
      menu $w.f1.f2.r.m -tearoff 0

      # if option "Operators" was choosen in menu
      $w.f1.f2.r.m add command -label "Operators" -command { \
         set v .proteum.muta.m.dlg

         if {![winfo exists $v.operators]} {
            $v.f1.f2.l configure -text "Operators"
            Operators_Sel_Operator $v.operators
         }
         # Destroy window of other class mutation operators
         if {[winfo exists $v.statements]} {
            destroy $v.statements
         }
         if {[winfo exists $v.variables]} {
            destroy $v.variables
         }
         if {[winfo exists $v.constants]} {
            destroy $v.constants
         }
      }

      # if option "Statements" was choosen in menu
      $w.f1.f2.r.m add command -label "Statements" -command { \
         set v .proteum.muta.m.dlg

         if {![winfo exists $v.statements]} {
            $v.f1.f2.l configure -text "Statements"
            Statements_Sel_Operator $v.statements
         }
         # Destroy window of other class mutation operators
         if {[winfo exists $v.operators]} {
            destroy $v.operators
         } else {
            message "v.operatorts not exists"
         }
         if {[winfo exists $v.variables]} {
            destroy $v.variables
         }
         if {[winfo exists $v.constants]} {
            destroy $v.constants
         }
      }
      # if option "Variables" was choosen in menu
      $w.f1.f2.r.m add command -label "Variables" -command { \
         set v .proteum.muta.m.dlg

         if {![winfo exists $v.variable]} {
            $v.f1.f2.l configure -text "Variables"
            Variables_Sel_Operator $v.variables
         }
         # Destroy window of other class mutation operators
         if {[winfo exists $v.statements]} {
            destroy $v.statements
         }
         if {[winfo exists $v.operators]} {
            destroy $v.operators
         }
         if {[winfo exists $v.constants]} {
            destroy $v.constants
         }
      }
      # if option "Constants" was choosen in menu
      $w.f1.f2.r.m add command -label "Constants" -command { \
         set v .proteum.muta.m.dlg

         if {![winfo exists $v.constant]} {
            $v.f1.f2.l configure -text "Constants"
            Constants_Sel_Operator $v.constants
         }
         # Destroy window of other class mutation operators
         if {[winfo exists $v.statements]} {
            destroy $v.statements
         }
         if {[winfo exists $v.variables]} {
            destroy $v.variables
         }
         if {[winfo exists $v.operators]} {
            destroy $v.operators
         }
      }



   label  $w.f1.f2.l -text "Operators"
   pack   $w.f1.f2.l -side left
   Operators_Sel_Operator $w.operators

   frame  $w.f2
   pack   $w.f2 -pady 5
   button $w.f2.confirm -text Select -command {
      # Destroy mutation operator window
      set v .proteum.muta.m.dlg
      if {[winfo exists $v.operators]} {
         destroy $v.operators
      }
      if {[winfo exists $v.statements]} {
         destroy $v.statements
      }
      if {[winfo exists $v.variables]} {
         destroy $v.variables
      }
      if {[winfo exists $v.constants]} {
         destroy $v.constants
      }
      set confirm 1;          # Variable for user not interact with other window
      Select_Mutants2
   }

   button $w.f2.cancel  -text Cancel -command "
      destroy $w
      set confirm 0";         # Variable for user not interact with other window
   pack   $w.f2.confirm $w.f2.cancel -side left

   wm protocol $w WM_DELETE_WINDOW {
      set confirm 0
      destroy .proteum.muta.m.dlg
   }

   bind $w <Any-Motion> {focus %W}

   grab $w
   tkwait variable confirm
   grab release $w
}



#------------------------------------------------------------------------------#
#  SELECT_MUTANTS {}
#  Select mutants.
#  date: 11/19/96
#  last update: 11/20/96, 11/25/96
#------------------------------------------------------------------------------#

proc Select_Mutants {} {
   global group1_data group2_data;       # Operator's name
   global ALL_CONNEC_SEL
   global in

   destroy .proteum.muta.m.dlg;          # Destroy last window
   set g_tot 0;                          # Total of generated mutants
 
   Set_Busy ON

   set listconnect [Join_Connection $ALL_CONNEC_SEL]
   set in(from) [Blank_Space_Out $in(from)]
   set in(to) [Blank_Space_Out $in(to)]

   if { $in(from) != "" } {
       set listconnect "$listconnect -f $in(from)"
   }
   if { $in(to) != "" } {
       set listconnect "$listconnect -t $in(to)"
   }

   set menusk false
   set listopt [Join_Operator_Percentage 1]
   if {[string length $listopt] > 0} {
      set listopt "$listopt $listconnect"
      set mutsel [Exemuta SELECT $listopt]
      set menusk true
   }

   set listopt [Join_Operator_Percentage 2]
   if {[string length $listopt] > 0} {
      set listopt "$listopt $listconnect"
      if {$menusk} {
         set listopt "-k $listopt"
      }
      set mutsel [Exemuta SELECT $listopt]
      set menusk true
   }

   Set_Busy OFF

#   message "$g_tot Selected Mutants"
   set infogeneral [Report GENERAL 0]

   # If error building report, return
   if {[string length $infogeneral] == 0} {
      return
   } else {
       Disjoin_Infogeneral $infogeneral PART
   }
}


#------------------------------------------------------------------------------#
#  SELECT_MUTANTS2 {}
#  Select mutants.
#  date: 3/9/00
#  last update: 3/9/00
#------------------------------------------------------------------------------#

proc Select_Mutants2 {} {
   global ALL_UNIT_SEL
   global in

   destroy .proteum.muta.m.dlg;          # Destroy last window
   set g_tot 0;                          # Total of generated mutants

   Set_Busy ON

   set listconnect [Join_Unit_Sel $ALL_UNIT_SEL]
   set in(from) [Blank_Space_Out $in(from)]
   set in(to) [Blank_Space_Out $in(to)]

   if { $in(from) != "" } {
       set listconnect "$listconnect -f $in(from)"
   }
   if { $in(to) != "" } {
       set listconnect "$listconnect -t $in(to)"
   }

   set menusk false
   set listopt [Join_Operator_Percentage 3]
   if {[string length $listopt] > 0} {
      set listopt "$listopt $listconnect"
      set mutsel [Exemuta SELECT $listopt]
      set menusk true
   }

   set listopt [Join_Operator_Percentage 4]
   if {[string length $listopt] > 0} {
      set listopt "$listopt $listconnect"
      if {$menusk} {
         set listopt "-k $listopt"
      }
      set mutsel [Exemuta SELECT $listopt]
      set menusk true
   }
   set listopt [Join_Operator_Percentage 5]
   if {[string length $listopt] > 0} {
      set listopt "$listopt $listconnect"
      if {$menusk} {
         set listopt "-k $listopt"
      }
      set mutsel [Exemuta SELECT $listopt]
      set menusk true
   }

   set listopt [Join_Operator_Percentage 6]
   if {[string length $listopt] > 0} {
      set listopt "$listopt $listconnect"
      if {$menusk} {
         set listopt "-k $listopt"
      }
      set mutsel [Exemuta SELECT $listopt]
      set menusk true
   }

   Set_Busy OFF

#   message "$g_tot Selected Mutants"
   set infogeneral [Report GENERAL 0]

   # If error building report, return
   if {[string length $infogeneral] == 0} {
      return
   } else {
       Disjoin_Infogeneral $infogeneral PART
   }
}


#------------------------------------------------------------------------------#
#   GROUP_I_SEL_OPERATOR {}
#   
#   date: 11/19/96
#   last update: 11/20/96
#------------------------------------------------------------------------------#

proc Group_I_Sel_Operator {w} {
   global FIXEDSIZE
   global in
   toplevel $w
   wm title $w "Group I Mutation/Selection"
   wm iconname $w "GI_Mut"
   wm geometry $w 520x300+20+198
#   wm maxsize  $w 520 300


   global group1_data;  # Names of the mutation operators 
   global group1_s;     # Percentage of mutations operator
   global group1_o;    # Number of generated mutants

   set tot        0;    # Total of generated mutants
   set in(def_s)  100

   frame $w.f1
   pack  $w.f1 -fill x -padx 30

   button $w.f1.default -text "Apply Default" -command "
      Apply_Default GROUP1 SELECT"
   label  $w.f1.l1 -text "Percentage:"
   entry  $w.f1.s -width 4 -justify right -textvariable in(def_s)
   pack   $w.f1.s $w.f1.l1 $w.f1.default -side right -pady 10
 

   # Create text area to insert operators name, in and out field 
   text $w.t -yscrollcommand "$w.s set" -relief sunken -bd 1 -state disabled \
             -cursor top_left_arrow
   scrollbar $w.s -command "$w.t yview"
   pack $w.s -side right -fill both
   pack $w.t -expand yes 

   set i 1
   foreach item $group1_data {
      frame $w.t.$i 
      pack  $w.t.$i 
      $w.t window create end -window $w.t.$i 
      label $w.t.$i.l -text $item -width 58 -anchor w
      pack  $w.t.$i.l -side left

      entry $w.t.$i.group1_s -width 4 -justify right \
      -textvariable group1_s($i)
      label $w.t.$i.o -width 4 -relief sunken -cursor top_left_arrow

      pack  $w.t.$i.group1_s $w.t.$i.o -side left -padx 3
      $w.t.$i.o config -text $group1_o($i) -anchor e

      incr i
   }

   frame $w.t.f1 
   pack  $w.t.f1
   $w.t window create end -window $w.t.f1 -pady 10
   bind $w <Any-Motion> {focus %W}
}



#------------------------------------------------------------------------------#
#   GROUP_II_SEL_OPERATOR {}
#   Shows Operator Mutations operators list to select percentage
#   date: 05/28/96
#   last update: 05/28/96
#------------------------------------------------------------------------------#

proc Group_II_Sel_Operator {w} {
   global FIXEDSIZE
   global in
   toplevel $w
   wm title $w "Group II Mutation/Selection"
   wm iconname $w "GII_Mut"
   wm geometry $w 450x260+20+198
#   wm maxsize  $w 450 260

   global group2_data;  # Names of the mutation operators
   global group2_s;     # Percentage of mutations operator
   global group2_o;    # Number of generated mutants

   set tot        0;    # Total of generated mutants
   set in(def_s)  100

   frame $w.f1
   pack  $w.f1 -fill x -padx 30

   button $w.f1.default -text "Apply Default" -command "
      Apply_Default GROUP2 SELECT"
   label  $w.f1.l1 -text "Percentage:"

   entry  $w.f1.s -width 4 -justify right -textvariable in(def_s)
   pack   $w.f1.s $w.f1.l1 $w.f1.default -side right -pady 10


   # Create text area to insert operators name, in and out field
   text $w.t -relief sunken -bd 1 -state disabled \
             -cursor top_left_arrow
   pack $w.t -expand yes

   set i 1
   foreach item $group2_data {
      frame $w.t.$i
      pack  $w.t.$i
      $w.t window create end -window $w.t.$i
      label $w.t.$i.l -text $item -width 50 -anchor w
      pack  $w.t.$i.l -side left

      entry $w.t.$i.group2_s -width 4 -justify right \
               -textvariable group2_s($i)
      label $w.t.$i.o -width 4 -relief sunken -cursor top_left_arrow

      pack  $w.t.$i.group2_s $w.t.$i.o -side left -padx 3
      $w.t.$i.o config -text $group2_o($i) -anchor e

      incr i
   }

   frame $w.t.f1
   pack  $w.t.f1
   $w.t window create end -window $w.t.f1 -pady 10
   bind $w <Any-Motion> {focus %W}
}

#------------------------------------------------------------------------------#
#   OPERATORS_SEL_OPERATOR {}
#
#   date: 3/9/00
#   last update: 3/9/00
#------------------------------------------------------------------------------#

proc Operators_Sel_Operator {w} {
   global FIXEDSIZE
   global in
   toplevel $w
   wm title $w "Operator Mutation/Selection"
   wm iconname $w "Opera_Mut"
   wm geometry $w 460x300+20+198
#   wm maxsize  $w 460 300


   global oper_data;  # Names of the mutation operators
   global oper_s;     # Percentage of mutations operator
   global oper_o;    # Number of generated mutants

   set tot        0;    # Total of generated mutants
   set in(def_s)  100

   frame $w.f1
   pack  $w.f1 -fill x -padx 30

   button $w.f1.default -text "Apply Default" -command "
      Apply_Default OPERA SELECT"
   label  $w.f1.l1 -text "Percentage:"
   entry  $w.f1.s -width 4 -justify right -textvariable in(def_s)
   pack   $w.f1.s $w.f1.l1 $w.f1.default -side right -pady 10


   # Create text area to insert operators name, in and out field
   text $w.t -yscrollcommand "$w.s set" -relief sunken -bd 1 -state disabled \
             -cursor top_left_arrow
   scrollbar $w.s -command "$w.t yview"
   pack $w.s -side right -fill both
   pack $w.t -expand yes

   set i 1
   foreach item $oper_data {
      frame $w.t.$i
      pack  $w.t.$i
      $w.t window create end -window $w.t.$i
      label $w.t.$i.l -text $item -width 50 -anchor w
      pack  $w.t.$i.l -side left

      entry $w.t.$i.oper_s -width 4 -justify right \
      -textvariable oper_s($i)
      label $w.t.$i.o -width 4 -relief sunken -cursor top_left_arrow

      pack  $w.t.$i.oper_s $w.t.$i.o -side left -padx 3
      $w.t.$i.o config -text $oper_o($i) -anchor e

      incr i
   }

   frame $w.t.f1
   pack  $w.t.f1
   $w.t window create end -window $w.t.f1 -pady 10
   bind $w <Any-Motion> {focus %W}
}


#------------------------------------------------------------------------------#
#   STATEMENTS_SEL_OPERATOR {}
#
#   date: 3/9/00
#   last update: 3/9/00
#------------------------------------------------------------------------------#

proc Statements_Sel_Operator {w} {
   global FIXEDSIZE
   global in
   toplevel $w
   wm title $w "Statement Mutation/Selection"
   wm iconname $w "Stat_Mut"
   wm geometry $w 390x300+20+198
#   wm maxsize  $w 390 300


   global stat_data;  # Names of the mutation operators
   global stat_s;     # Percentage of mutations operator
   global stat_o;    # Number of generated mutants

   set tot        0;    # Total of generated mutants
   set in(def_s)  100

   frame $w.f1
   pack  $w.f1 -fill x -padx 30

   button $w.f1.default -text "Apply Default" -command "
      Apply_Default STAT SELECT"
   label  $w.f1.l1 -text "Percentage:"
   entry  $w.f1.s -width 4 -justify right -textvariable in(def_s)
   pack   $w.f1.s $w.f1.l1 $w.f1.default -side right -pady 10


   # Create text area to insert operators name, in and out field
   text $w.t -yscrollcommand "$w.s set" -relief sunken -bd 1 -state disabled \
             -cursor top_left_arrow
   scrollbar $w.s -command "$w.t yview"
   pack $w.s -side right -fill both
   pack $w.t -expand yes

   set i 1
   foreach item $stat_data {
      frame $w.t.$i
      pack  $w.t.$i
      $w.t window create end -window $w.t.$i
      label $w.t.$i.l -text $item -width 40 -anchor w
      pack  $w.t.$i.l -side left

      entry $w.t.$i.stat_s -width 4 -justify right \
      -textvariable stat_s($i)
      label $w.t.$i.o -width 4 -relief sunken -cursor top_left_arrow

      pack  $w.t.$i.stat_s $w.t.$i.o -side left -padx 3
      $w.t.$i.o config -text $stat_o($i) -anchor e

      incr i
   }

   frame $w.t.f1
   pack  $w.t.f1
   $w.t window create end -window $w.t.f1 -pady 10
   bind $w <Any-Motion> {focus %W}
}



#------------------------------------------------------------------------------#
#   VARIABLES_SEL_OPERATOR {}
#
#   date: 3/9/00
#   last update: 3/9/00
#------------------------------------------------------------------------------#

proc Variables_Sel_Operator {w} {
   global FIXEDSIZE
   global in
   toplevel $w
   wm title $w "Variable Mutation/Selection"
   wm iconname $w "Var_Mut"
   wm geometry $w 390x300+20+198
#   wm maxsize  $w 390 300


   global var_data;  # Names of the mutation operators
   global var_s;     # Percentage of mutations operator
   global var_o;    # Number of generated mutants

   set tot        0;    # Total of generated mutants
   set in(def_s)  100

   frame $w.f1
   pack  $w.f1 -fill x -padx 30

   button $w.f1.default -text "Apply Default" -command "
      Apply_Default VAR SELECT"
   label  $w.f1.l1 -text "Percentage:"
   entry  $w.f1.s -width 4 -justify right -textvariable in(def_s)
   pack   $w.f1.s $w.f1.l1 $w.f1.default -side right -pady 10


   # Create text area to insert operators name, in and out field
   text $w.t -relief sunken -bd 1 -state disabled \
             -cursor top_left_arrow
   pack $w.t -expand yes

   set i 1
   foreach item $var_data {
      frame $w.t.$i
      pack  $w.t.$i
      $w.t window create end -window $w.t.$i
      label $w.t.$i.l -text $item -width 40 -anchor w
      pack  $w.t.$i.l -side left

      entry $w.t.$i.var_s -width 4 -justify right \
      -textvariable var_s($i)
      label $w.t.$i.o -width 4 -relief sunken -cursor top_left_arrow

      pack  $w.t.$i.var_s $w.t.$i.o -side left -padx 3
      $w.t.$i.o config -text $var_o($i) -anchor e

      incr i
   }

   frame $w.t.f1
   pack  $w.t.f1
   $w.t window create end -window $w.t.f1 -pady 10
   bind $w <Any-Motion> {focus %W}
}

#------------------------------------------------------------------------------#
#   CONSTANTS_SEL_OPERATOR {}
#
#   date: 3/9/00
#   last update: 3/9/00
#------------------------------------------------------------------------------#

proc Constants_Sel_Operator {w} {
   global FIXEDSIZE
   global in
   toplevel $w
   wm title $w "Constant Mutation/Selection"
   wm iconname $w "Const_Mut"
   wm geometry $w 390x120+20+198
#   wm maxsize  $w 390 120


   global const_data;  # Names of the mutation operators
   global const_s;     # Percentage of mutations operator
   global const_o;    # Number of generated mutants

   set tot        0;    # Total of generated mutants
   set in(def_s)  100

   frame $w.f1
   pack  $w.f1 -fill x -padx 30

   button $w.f1.default -text "Apply Default" -command "
      Apply_Default CONST SELECT"
   label  $w.f1.l1 -text "Percentage:"
   entry  $w.f1.s -width 4 -justify right -textvariable in(def_s)
   pack   $w.f1.s $w.f1.l1 $w.f1.default -side right -pady 10


   # Create text area to insert operators name, in and out field
   text $w.t -relief sunken -bd 1 -state disabled \
             -cursor top_left_arrow
   pack $w.t -expand yes

   set i 1
   foreach item $const_data {
      frame $w.t.$i
      pack  $w.t.$i
      $w.t window create end -window $w.t.$i
      label $w.t.$i.l -text $item -width 40 -anchor w
      pack  $w.t.$i.l -side left

      entry $w.t.$i.const_s -width 4 -justify right \
      -textvariable const_s($i)
      label $w.t.$i.o -width 4 -relief sunken -cursor top_left_arrow

      pack  $w.t.$i.const_s $w.t.$i.o -side left -padx 3
      $w.t.$i.o config -text $const_o($i) -anchor e

      incr i
   }

   frame $w.t.f1
   pack  $w.t.f1
   $w.t window create end -window $w.t.f1 -pady 10
   bind $w <Any-Motion> {focus %W}
}



#-----------------------------------------------------------------------------#
#   APPLY_DEFAULT {}
#   Apply default value for operators to generate or select mutants.
#   date: 04/06/96
#   last update:  05/28/96, 11/11/96
#------------------------------------------------------------------------------#

proc Apply_Default {class operation} {
   global in;                      # % default

   global group1_n group2_n stat_n const_n var_n oper_n;
   global group1_m group2_m stat_m const_m var_m oper_m;
   global group1_o group2_o stat_o const_o var_o oper_o;
   global group1_s group2_s stat_s const_s var_s oper_s;
   global NGROUP1 NGROUP2 NSTAT NVAR NCONST NOPERA;

   set i 1

   switch -exact $class {
      GROUP1 { 
         while {$i <= $NGROUP1} {;         # Class Group 1

            # if operation is generation of mutants
            if {[string compare $operation GENERATE] == 0} {
                  set group1_n($i) $in(def_n)
                  set group1_m($i) $in(def_m)
            } elseif {[string compare $operation SELECT] == 0} {
               set group1_s($i) $in(def_s)
            }
            incr i
         }
      }
      GROUP2 { 
         while {$i <= $NGROUP2} {;         # Class Group 2

            # if operation id generation od mutants
            if {[string compare $operation GENERATE] == 0} {
                  set group2_n($i) $in(def_n)
                  set group2_m($i) $in(def_m)
            } elseif {[string compare $operation SELECT] == 0} {
               set group2_s($i) $in(def_s)
            }
            incr i
         }   
      }
      OPERA { 
         while {$i <= $NOPERA} {;         # Class u-O

            # if operation id generation od mutants
            if {[string compare $operation GENERATE] == 0} {
                  set oper_n($i) $in(def_n)
                  set oper_m($i) $in(def_m)
            } elseif {[string compare $operation SELECT] == 0} {
               set oper_s($i) $in(def_s)
            }
            incr i
         }   
      }
      VAR {
         while {$i <= $NVAR} {;         # Class u-V

            # if operation id generation od mutants
            if {[string compare $operation GENERATE] == 0} {
                  set var_n($i) $in(def_n)
                  set var_m($i) $in(def_m)
            } elseif {[string compare $operation SELECT] == 0} {
               set var_s($i) $in(def_s)
            }
            incr i
         }
      }
      CONST {
         while {$i <= $NCONST} {;         # Class u-C

            # if operation id generation od mutants
            if {[string compare $operation GENERATE] == 0} {
                  set const_n($i) $in(def_n)
                  set const_m($i) $in(def_m)
            } elseif {[string compare $operation SELECT] == 0} {
               set const_s($i) $in(def_s)
            }
            incr i
         }
      }
      STAT {
         while {$i <= $NSTAT} {;         # Class u-S

            # if operation id generation od mutants
            if {[string compare $operation GENERATE] == 0} {
                  set stat_n($i) $in(def_n)
                  set stat_m($i) $in(def_m)
            } elseif {[string compare $operation SELECT] == 0} {
               set stat_s($i) $in(def_s)
            }
            incr i
         }
      }
   }
   return
}
