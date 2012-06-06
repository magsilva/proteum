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
#   IREPORT.TCL
#   Procedures related with REPORTS options.
#   author: Elisa Yumi Nakagawa
#   date: 04/27/96
#   last update: 03/07/96
#------------------------------------------------------------------------------#


#------------------------------------------------------------------------------#
#   REPORT_TCASE_WINDOW
#   Report of test case.
#   date: 02/26/96
#   last update: 04/05/96
#------------------------------------------------------------------------------#


proc Report_Tcase_Window {w} {
   global FIXEDSIZE
   toplevel $w
   wm title $w "Test Case Report"
   wm iconname $w "Test Case"
   wm geometry $w +10+65
   wm minsize  $w 300 350
   if { $FIXEDSIZE } {
   	wm maxsize  $w 300 350
   	wm geometry $w 300x350
   }

   global test
   global tcase0 tcase1 tcase2 tcase3 tcase4 tcase5 tcase6 tcase7 tcase8 tcase9

   set tcase0 0
   set tcase1 0
   set tcase2 0
   set tcase3 0
   set tcase4 0
   set tcase5 0
   set tcase6 0
   set tcase7 0
   set tcase8 0
   set tcase9 0

   frame $w.f1 
   pack  $w.f1 -fill x -pady 5
   frame $w.f1.f1 
   pack  $w.f1.f1 -fill x 
   label $w.f1.f1.label -text "Header:" 
   pack  $w.f1.f1.label -side left
   frame $w.f1.f2 
   pack  $w.f1.f2 -fill x
   checkbutton $w.f1.f2.0 -text "Mutants per Operator" -variable tcase0 \
          -selectcolor green -cursor hand2
   pack  $w.f1.f2.0 -anchor w


   frame $w.f2 
   pack  $w.f2 -fill x -pady 5
   frame $w.f2.f1 
   pack  $w.f2.f1 -fill x 
   label $w.f2.f1.label -text "Body:" 
   pack  $w.f2.f1.label -side left

   set b_data {{} {Effective Test Cases Only} \
                  {Number of Not Executed Mutants} \
                  {Number of Alive Mutants} \
                  {Number of Mutants by each "Causa Mortis"} \
                  {Total Number of Dead Mutants} \
                  Enabled/Disabled \
                  {Parameters, Return Code, Execution Time} \
                  Input \
                  Output}
   

   frame $w.f2.f2 
   pack  $w.f2.f2 -fill x
   foreach i {1 2 3 4 5 6 7 8 9} {
      checkbutton $w.f2.f2.$i -text [lindex $b_data $i] -variable tcase$i \
          -selectcolor green -cursor hand2
      pack $w.f2.f2.$i -anchor w
   }

   # Setting default
   $w.f1.f2.0 select
   $w.f2.f2.1 select
   $w.f2.f2.5 select

   frame  $w.f3 
   pack   $w.f3 -pady 5
   button $w.f3.confirm -text Confirm -command "
          set confirm 1
          destroy $w
          Show_RTcase_Window $test(dir)/$test(tfile).lst"
   button $w.f3.cancel  -text Cancel  -command "
          set confirm 0
          destroy $w"
   pack   $w.f3.confirm $w.f3.cancel -side left 

   bind $w <Any-Motion> {focus %W}
   bind $w <Return> ".proteum.report.m.dlg.f3.confirm invoke"

   wm protocol $w WM_DELETE_WINDOW {
      set confirm 0
      destroy .proteum.report.m.dlg
   }
   grab $w
   tkwait variable confirm
   grab release $w
}
 


#------------------------------------------------------------------------------#
#   SHOW_RTCASE_WINDOW
#   Show test case report.
#   date: 02/27/96
#   last update: 05/02/96
#------------------------------------------------------------------------------#

proc Show_RTcase_Window {wintitle} {
   global FIXEDFONT
   toplevel .r
   wm title .r $wintitle 
   wm iconname .r "RTCase"
   wm geometry .r 80x37
   wm geometry .r +10+65
   wm minsize  .r 70 37
   

   global test
   global tcase0 tcase1 tcase2 tcase3 tcase4 \
          tcase5 tcase6 tcase7 tcase8 tcase9
 
   set level [expr $tcase1 *   1 + \
                   $tcase2 *   2 + \
                   $tcase3 *   4 + \
                   $tcase4 *   8 + \
                   $tcase5 *  16 + \
                   $tcase6 *  32 + \
                   $tcase7 *  64 + \
                   $tcase8 * 128 + \
                   $tcase9 * 256]

   # If error building report, return
   if {![Report TCASE $level]} {
      return
   }


# OBS: NAO E' USADO A INFORMACAO DE "Operator's Percentage" NO MODULOS (SCRIPT)
# PARA COLOCAR OU NAO A PORCENTAGEM DOS OPERADORES DE MUTACAO APLICADOS.
# POR DEFAULT, O MODULO (SCRIPT) SEMPRE COLOCA A LISTA DE OPERADORES APLICADOS
# E SUA PORCENTAGEM.


   frame .r.f1
   pack  .r.f1 -expand true -fill both
   text  .r.f1.text -yscrollcommand ".r.f1.yscroll set" -setgrid true \
         -width 100 -height 37 -bg white -borderwidth 2 \
         -cursor top_left_arrow -font $FIXEDFONT
   scrollbar .r.f1.yscroll -command ".r.f1.text yview"
   pack  .r.f1.yscroll -side right -fill both -expand true
   pack  .r.f1.text -fill both -expand true
   
   # Loads report about test cases effectiveness 
   Load_Src $test(dir)/$test(tfile).lst .r.f1.text 
   
   frame  .r.f2 
   pack   .r.f2
   button .r.f2.ok -text {   OK   } -command "
          set ok 1
          destroy .r"
   pack   .r.f2.ok -pady 10

   wm protocol .r WM_DELETE_WINDOW {
      set ok 0
      destroy .r
   }
   grab .r
   tkwait variable ok
   grab release .r
}


