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
#   GERAL.TCL
#   General functions.
#   author: Elisa Yumi Nakagawa
#   date: 03/21/96
#   last update:  05/29/96, 11/20/96, 11/22/96
#------------------------------------------------------------------------------#




#------------------------------------------------------------------------------#
#   EXISTS_INST_FILE {}
#   Verifies if instrumented file and prove file exist.
#   date: 11/06/96
#   last update: 11/06/96
#------------------------------------------------------------------------------#

proc Exists_Inst_File {} {  
   global test 
   
   # take off ".c" of source filename
   set tmpfile [string trimright $test(src) ".c"] 

   set instfile __"$tmpfile"_inst.c
   set ppfile   __"$tmpfile"_pp.c

   if {[file isfile $test(dir)/$instfile] || [file isfile $test(dir)/$ppfile]} {
      return 1
   } else {
      return 0
   }
}


#------------------------------------------------------------------------------#
#   Set_Directory
#   Return directory name if it is not set.
#   date: 03/21/96
#   last update: 03/21/96
#------------------------------------------------------------------------------#

proc Set_Directory {dir} {  

   if {[string length $dir] == 0} {
      return [pwd]
   } else {
      return $dir
   }
}



#------------------------------------------------------------------------------#
#   Set_Timeout
#   Return timeout default if there is not defined value.
#   date: 03/25/96
#   last update: 03/25/96
#------------------------------------------------------------------------------#

proc Set_Timeout {timeout} {

   if {[string length $timeout] == 0} {
      return {5}
   } else {
      return $timeout
   }
}


#------------------------------------------------------------------------------#
#    BLANK_SPACE_OUT {}
#    Take out blank spaces of a given string.
#    date: 03/22/96
#    last update: 03/22/96
#------------------------------------------------------------------------------#

proc Blank_Space_Out {str} {

   regsub -all { } $str {} tmp
   return $tmp
}


#------------------------------------------------------------------------------#
#    LOAD_FILE_PTM {}
#    Load Test Data in File .PTM.
#    date: 03/27/96
#    last update: 03/27/96, 09/20/96 
#------------------------------------------------------------------------------#

proc Load_File_PTM {} {  
   global test
 
   set test(func) all
   set ptm $test(dir)/$test(tfile).PTM
 
   if [catch {open $ptm r} fid] {;     # open file .PTM for reading
      message "Cannot open file $ptm !"
      return
   } else {
      foreach i {1 2 3 4} {;  # HEADER LINES
         gets $fid line
      }
      gets $fid test(src) 
      gets $fid test(exec) 
      gets $fid test(comp) 
      gets $fid test(type)
      gets $fid test(anom)
      gets $fid tmpfunc 

      close $fid
   }
}


#------------------------------------------------------------------------------#
#   DISJOIN_INFOMUTA {}
#   Disjoins informations about mutant information.
#   date: 04/19/96
#   last update: 04/18/96, 11/18/96, 11/22/96
#------------------------------------------------------------------------------#

proc Disjoin_Infomuta {infomuta} {
   global muta;             # Mutant's data


      if {[scan $infomuta "\nMUTANT # %d                      \n 
                          Status %s %s                        \n
	                  Causa Mortis: %s - Test Case %d     \n
                          Descriptor size.: %d                \n
                          Calling function starts at: %d      \n
                          Called function starts at: %d       \n
                          Sequencial numbering: %d %d         \n
                          Last test case used: %d             \n
                          Operator: %d %s                     \n
                          Descriptor:                         \n
		                  Tipo: %d                    \n
		                  Program graph node: %d      \n
                          Offset: %d, get out %d characters"  \
                          mutant                              \
                          muta(status0)                       \
                          muta(status1)                       \
                          muta(mortis)                        \
                          muta(mtcase)                        \
                          muta(dsize)                         \
                          muta(calling)                       \
                          muta(called)                        \
                          muta(seqnum0)                       \
                          muta(seqnum1)                       \
                          muta(tcase)                         \
                          muta(num_op)                        \
                          muta(name_op)                       \
                          muta(type)                          \
                          muta(node)                          \
                          muta(d_init0)                       \
                          muta(d_init1)] != 17} {
             message "Problem with Mutant Information !"
             return 1
   }
   return 0
}


#------------------------------------------------------------------------------#
#   DISJOIN_INFOGENERAL {}
#   Disjoins general informations.
#   date: 06/05/97
#   last update: 06/05/97
#------------------------------------------------------------------------------#

proc Disjoin_Infogeneral {infogeneral type} {
   global out
   global maxtcase
   global group1_n group2_n stat_n const_n var_n oper_n;
   global group1_m group2_m stat_m const_m var_m oper_m;
   global group1_o group2_o stat_o const_o var_o oper_o;
   global NGROUP1 NGROUP2 NSTAT NVAR NCONST NOPERA;

   switch -exact $type {
      PART {

         if {[scan $infogeneral "\n%d                      
                                 \n%d
                                 \n%d
                                 \n%d
                                 \n%s
                                 \n%d
                                 \n%d"                   \
                                 maxtcase                \
                                 out(totmut)             \
                                 out(actmut)             \
                                 out(equmut)             \
                                 out(score)              \
                                 out(livmut)             \
                                 out(anomut)] != 7} {
            message "Problem Restauring Information !"
            return
         } 
      } 
      ALL {

         if {[scan $infogeneral "\n%d                      
                                 \n%d
                                 \n%d
                                 \n%d
                                 \n%s
                                 \n%d
                                 \n%d"                   \
                                 maxtcase                \
                                 out(totmut)             \
                                 out(actmut)             \
                                 out(equmut)             \
                                 out(score)              \
                                 out(livmut)             \
                                 out(anomut)] != 7} {
            message "Problem Restauring Information !"
            return
         } else {

            set tmpstring [string range $infogeneral 1 \
                          [expr [string length $infogeneral]-1]]
            set nextpos [string first "\n" $tmpstring]
 

            # Takes out general information
            for {set i 1} {$i <= 7} {incr i} {
               set tmpnumber [string range $tmpstring 0 [expr $nextpos-1]]
               set tmpstring [string range $tmpstring [expr $nextpos+1] \
                        [expr [string length $tmpstring]-1]]
               set nextpos [string first "\n" $tmpstring]
            }


            # Takes out group I operator information
            for {set i 1} {$i <= $NGROUP1} {incr i} {
               if {[scan $tmpstring "%d" group1_o($i)] != 1} {
                  message "Problem Restauring Information !"
                  return
               }
               set tmpstring [string range $tmpstring [expr $nextpos+1] \
                        [expr [string length $tmpstring]-1]]
               set nextpos [string first "\n" $tmpstring]

            }

            # Takes out group II operator information
            for {set i 1} {$i <= $NGROUP2} {incr i} {
               if {[scan $tmpstring "%d" group2_o($i)] != 1} {
                  message "Problem Restauring Information !"
                  return
               }
               set tmpstring [string range $tmpstring [expr $nextpos+1] \
                        [expr [string length $tmpstring]-1]]
               set nextpos [string first "\n" $tmpstring]

            }
            # Takes out group u-c operator information
            for {set i 1} {$i <= $NCONST} {incr i} {
               if {[scan $tmpstring "%d" const_o($i)] != 1} {
                  message "Problem Restauring Information !"
                  return
               }
               set tmpstring [string range $tmpstring [expr $nextpos+1] \
                        [expr [string length $tmpstring]-1]]
               set nextpos [string first "\n" $tmpstring]

            }
            # Takes out group u-o operator information
            for {set i 1} {$i <= $NOPERA} {incr i} {
               if {[scan $tmpstring "%d" oper_o($i)] != 1} {
                  message "Problem Restauring Information !"
                  return
               }
               set tmpstring [string range $tmpstring [expr $nextpos+1] \
                        [expr [string length $tmpstring]-1]]
               set nextpos [string first "\n" $tmpstring]

            }
            # Takes out group u-s operator information
            for {set i 1} {$i <= $NSTAT} {incr i} {
               if {[scan $tmpstring "%d" stat_o($i)] != 1} {
                  message "Problem Restauring Information !"
                  return
               }
               set tmpstring [string range $tmpstring [expr $nextpos+1] \
                        [expr [string length $tmpstring]-1]]
               set nextpos [string first "\n" $tmpstring]

            }
            # Takes out group u-v operator information
            for {set i 1} {$i <= $NVAR} {incr i} {
               if {[scan $tmpstring "%d" var_o($i)] != 1} {
                  message "Problem Restauring Information !"
                  return
               }
               set tmpstring [string range $tmpstring [expr $nextpos+1] \
                        [expr [string length $tmpstring]-1]]
               set nextpos [string first "\n" $tmpstring]

            }
         }                              
      }
   }
}


#------------------------------------------------------------------------------#
#   DISJOIN_INF_TCASE {}
#   Disjoin information about test case.
#   date: 04/26/96
#   last update: 04/26/96, 11/04/96
#------------------------------------------------------------------------------#

proc Disjoin_Inf_Tcase {inf} {
   global tcase;    # Information about test case
 
   set str1 {}
   set str2 {}
   set num  0
   set tcase(in) {};      # Inicializes test case input
   set tcase(out) {};     # Inicializes test case output
   set tcase(err) {};

   set tmp $inf

 
   set pos [string first \n $tmp];                  # Position of first eoline
   set end [string last \n $tmp];                   # Position of last eoline

      set tmp [string range $tmp [expr $pos+1] $end]
      set pos [string first \n $tmp];     # Position of next end of line

      set tmp [string range $tmp [expr $pos+1] $end]      
      set pos [string first \n $tmp];     # Position of next end of line

      set tmp [string range $tmp [expr $pos+2] $end]      
      set tcase(status) [string range $tmp 0 [expr $pos-1] ]
      set tmp [string range $tmp [expr $pos+2] $end]

      set pos [string first \n $tmp];     # Position of next end of line
      set aux [string first "(CPU):" $tmp]
      set tcase(exec) [string range $tmp [expr $aux+6] [expr $pos - 1]]
      set tmp [string range $tmp [expr $pos+2] $end]

      set pos [string first \n $tmp];     # Position of next end of line
      set aux [string first "(total):" $tmp]
      set tcase(total) [string range $tmp [expr $aux+8] [expr $pos - 1]]
      set tmp [string range $tmp [expr $pos+2] $end]

      set pos [string first \n $tmp];     # Position of next end of line
      set aux [string first "Code:" $tmp]
      set tcase(rcode) [string range $tmp [expr $aux+5] [expr $pos - 1]]
      set tmp [string range $tmp [expr $pos+2] $end]

      set pos [string first \n $tmp];     # Position of next end of line
      set aux [string first "Parameters:" $tmp]
      set tcase(param) [string range $tmp [expr $aux+11] [expr $pos - 1]]
      set tmp [string range $tmp [expr $pos+2] $end]

      set pos [string first \n $tmp];     # Position of next end of line
      set aux [string first "Size:" $tmp]
      set tcase(insize) [string range $tmp [expr $aux+5] [expr $pos - 1]]
      set tmp [string range $tmp [expr $pos+2] $end]

      set pos [string first \n $tmp];     # Position of next end of line
      set aux [string first "Size:" $tmp]
      set tcase(outsize) [string range $tmp [expr $aux+5] [expr $pos - 1]]
      set tmp [string range $tmp [expr $pos+2] $end]

      set pos [string first \n $tmp];     # Position of next end of line
      set aux [string first "Size:" $tmp]
      set tcase(stdsize) [string range $tmp [expr $aux+5] [expr $pos - 1]]
      set tmp [string range $tmp [expr $pos+2] $end]

      set pos [string first \n $tmp];     # Position of next end of line
      set tmp [string range $tmp [expr $pos+1] $end]
      set tcase(in) [string range $tmp 0 [expr $tcase(insize)]]
      set tmp [string range $tmp [expr $tcase(insize)+1] $end]

      set aux [string first "Output:" $tmp]
      set tmp [string range $tmp $aux $end]
      set pos [string first \n $tmp];     # Position of next end of line
      set tmp [string range $tmp [expr $pos+1] $end]
      set tcase(out) [string range $tmp 0 $tcase(outsize)]
      set tmp [string range $tmp [expr $tcase(outsize)+1] $end]

      set aux [string first "Stderr:" $tmp]
      set tmp [string range $tmp $aux $end]
      set pos [string first \n $tmp];     # Position of next end of line
      set tmp [string range $tmp [expr $pos+1] $end]
      set tcase(err) [string range $tmp 0 $tcase(stdsize)]
      set tmp [string range $tmp [expr $tcase(stdsize)+1] $end]
}


#------------------------------------------------------------------------------#
#   INIT_VARIABLES {}
#   Initializes variables for next test session.
#   date: 11/08/96
#   last update: 11/08/96, 11/20/96
#------------------------------------------------------------------------------#

proc Init_Variables {} {
   global group1_n;  # percentage to generate
   global group2_n;  # percentage to generate
   global oper_n;  # percentage to generate
   global const_n;  # percentage to generate
   global var_n;  # percentage to generate
   global stat_n;  # percentage to generate
   global group1_m;  # max number of mutants to generate
   global group2_m;  # max number of mutants to generate
   global oper_m;  # max number of mutants to generate
   global const_m;  # max number of mutants to generate
   global var_m;  # max number of mutants to generate
   global stat_m;  # max number of mutants to generate
   global group1_s;  # percentage to select
   global group2_s;  # percentage to select
   global oper_s;  # percentage to select
   global const_s;  # percentage to select
   global var_s;  # percentage to select
   global stat_s;  # percentage to select

   global NGROUP1;   # Number of operator in group1
   global NGROUP2;   # Number of operator in group2
   global NOPERA;    # Number of operators in u-O
   global NSTAT;
   global NVAR;
   global NCONST;
   global type0 type1 type2 type3 type4;    # Type of mutant for show
   global MAXCONNECTIONS;   # max number of connections
   global ALL_CONNEC_GEN;   # 1 if all connections, 0 otherwise (generation)
   global ALL_UNIT_GEN;   # 1 if all connections, 0 otherwise (generation)
   global ALL_CONNEC_SEL;   # 1 if all connections, 0 otherwise (generation)
   global ALL_UNIT_SEL;   # 1 if all connections, 0 otherwise (generation)


 
   for {set i 1} {$i <= $NGROUP1} {incr i 1} {
      set group1_n($i) 0
      set group1_m($i) 0 
      set group1_s($i) 0 
   }
   for {set i 1} {$i <= $NGROUP2} {incr i 1} {
      set group2_n($i) 0
      set group2_m($i) 0 
      set group2_s($i) 0 
   }
   for {set i 1} {$i <= $NOPERA} {incr i 1} {
      set oper_n($i) 0
      set oper_m($i) 0 
      set oper_s($i) 0 
   }
   for {set i 1} {$i <= $NSTAT} {incr i 1} {
      set stat_n($i) 0
      set stat_m($i) 0
      set stat_s($i) 0
   }
   for {set i 1} {$i <= $NVAR} {incr i 1} {
      set var_n($i) 0
      set var_m($i) 0
      set var_s($i) 0
   }
   for {set i 1} {$i <= $NCONST} {incr i 1} {
      set const_n($i) 0
      set const_m($i) 0
      set const_s($i) 0
   }
   set type0 1
   set type1 1
   set type2 1
   set type3 1
   set type4 0

   set MAXCONNECTIONS 0
   set ALL_CONNEC_GEN 1
   set ALL_UNIT_GEN 1
   set ALL_CONNEC_SEL 1
   set ALL_UNIT_SEL 1

   set infogeneral [Report GENERAL 0]

   # If error building report, return 
   if {[string length $infogeneral] == 0} {
      return
   } else {
       Disjoin_Infogeneral $infogeneral ALL
   }  
}


#------------------------------------------------------------------------------#
#   TAKES_OPERATOR_NAME {}
#   Takes the "sigla" of operator.
#   date: 11/11/96
#   last update: 11/11/96
#------------------------------------------------------------------------------#

proc Takes_Operator_Name {op} {

   if {[scan $op "%s - %s" sigla extname] != 2} {

       message "Problem Taking Operator Name !"
       return 0
   } else {
       return $sigla
   }
}



#------------------------------------------------------------------------------#
#   JOIN_OPERATOR_PERCENTAGE {}
#   Joints the operators and percentages.
#   date: 11/25/96
#   last update: 11/25/96
#------------------------------------------------------------------------------#

proc Join_Operator_Percentage {grp} {
   global group1_data group2_data
   global group1_s group2_s
   global oper_data stat_data var_data const_data
   global oper_s stat_s var_s const_s

   set list ""
   set i 1
   if {$grp == 1} {;     # Operator of group 1
      foreach op $group1_data {
         set operator [Takes_Operator_Name $op];      # Takes operator's name
         set list "$list -I-$operator $group1_s($i)"        
         incr i
      }
   } elseif {$grp == 2} {;            # Operator of group 2
      foreach op $group2_data {
         set operator [Takes_Operator_Name $op];      # Takes operator's name
         set list "$list -II-$operator $group2_s($i)"        
      }
   } elseif {$grp == 3} {;            # Operator of group u-o
      foreach op $oper_data {
         set operator [Takes_Operator_Name $op];      # Takes operator's name
         set list "$list -u-$operator $oper_s($i)"
      }
   } elseif {$grp == 4} {;            # Operator of group u-s
      foreach op $stat_data {
         set operator [Takes_Operator_Name $op];      # Takes operator's name
         set list "$list -u-$operator $stat_s($i)"
      }
   } elseif {$grp == 5} {;            # Operator of group u-v
      foreach op $var_data {
         set operator [Takes_Operator_Name $op];      # Takes operator's name
         set list "$list -u-$operator $var_s($i)"
      }
   } elseif {$grp == 6} {;            # Operator of group u-c
      foreach op $const_data {
         set operator [Takes_Operator_Name $op];      # Takes operator's name
         set list "$list -u-$operator $const_s($i)"
      }
   }
   return $list
}


#------------------------------------------------------------------------------#
#   JOIN_OPERATOR_PERCENTAGE_MAXMUT {}
#   Joints the operators, percentages and max number of mutants in each point.
#   date: 11/22/96
#   last update: 11/22/96, 11/25/96
#------------------------------------------------------------------------------#

proc Join_Operator_Percentage_Maxmut {grp} {
   global group1_data group2_data oper_data stat_data var_data const_data
   global group1_n    group2_n oper_n stat_n var_n const_n
   global group1_m    group2_m oper_m stat_m var_m const_m

   set list ""

   set i 1
   if {$grp == 1} {;     # Operator of group 1
      foreach op $group1_data {
         if {$group1_n($i) != 0} {
            set operator [Takes_Operator_Name $op];      # Takes operator's name
            set list "$list -I-$operator $group1_n($i) $group1_m($i)"
         }
      incr i
      }
   } elseif {$grp == 2} {;            # Operator of group 2
      foreach op $group2_data {
         if {$group2_n($i) != 0} {
            set operator [Takes_Operator_Name $op];      # Takes operator's name
            set list "$list -II-$operator $group2_n($i) $group2_m($i)"
         }
      incr i
      }
   } elseif {$grp == 3} {;            # Operator of group u-o
      foreach op $oper_data {
         if {$oper_n($i) != 0} {
            set operator [Takes_Operator_Name $op];      # Takes operator's name
            set list "$list -u-$operator $oper_n($i) $oper_m($i)"
         }
      incr i
      }
   } elseif {$grp == 4} {;            # Operator of group u-s
      foreach op $stat_data {
         if {$stat_n($i) != 0} {
            set operator [Takes_Operator_Name $op];      # Takes operator's name
            set list "$list -u-$operator $stat_n($i) $stat_m($i)"
         }
      incr i
      }
   } elseif {$grp == 5} {;            # Operator of group u-v
      foreach op $var_data {
         if {$var_n($i) != 0} {
            set operator [Takes_Operator_Name $op];      # Takes operator's name
            set list "$list -u-$operator $var_n($i) $var_m($i)"
         }
      incr i
      }
   } elseif {$grp == 6} {;            # Operator of group u-c
      foreach op $const_data {
         if {$const_n($i) != 0} {
            set operator [Takes_Operator_Name $op];      # Takes operator's name
            set list "$list -u-$operator $const_n($i) $const_m($i)"
         }
      incr i
      }
   }
   return $list
}


proc Join_Connection {alls} {
global connection MAXCONNECTIONS;

set list "";

#   if {$alls == 1} {
#        return " "
#   }

   for { set i 0 } {$i < $MAXCONNECTIONS} {incr i} {
      set list "$list -c $connection($i) "
   }
   return $list;
}


proc Join_Unit {alls} {
global connection MAXCONNECTIONS;

set list "";

#   if {$alls == 1} {
#        return " "
#   }

   for { set i 0 } {$i < $MAXCONNECTIONS} {incr i} {
      set list "$list -unit $connection($i) "
   }
   return $list;
}


proc Join_Unit_Sel {alls} {
global connection MAXCONNECTIONS;

set list "";

#   if {$alls == 1} {
#        return " "
#   }

   for { set i 0 } {$i < $MAXCONNECTIONS} {incr i} {
      set list "$list -c $connection($i) -all "
   }
   return $list;
}
