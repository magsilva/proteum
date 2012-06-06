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
#   INTER.TCL
#   Makes the interface between the user interface and executable files that
#   implements the funcionality of Proteum Tool.
#   author: Elisa Yumi Nakagawa
#   date: 03/28/96
#   last update: 05/20/96, 09/20/96, 11/04/96
#------------------------------------------------------------------------------#




#------------------------------------------------------------------------------#
#   PTESTE {}
#   Calls executable file PTEST that creates and handles program test files.
#   date: 03/28/96
#   last update: 03/29/96, 09/20/96, 11/04/96, 11/14/96
#------------------------------------------------------------------------------#

proc Pteste {operation} {
   global test   
   global MAXFUNCTIONS
   global index_func
   global functions
   global PROTEUMHOME

   switch -exact $operation {

      CREATE {

         set coma "\nPROTEUM> pteste -create -S $test(src) -E $test(exec) \
-D $test(dir) -C \"$test(comp)\" -$test(type) $test(tfile)"
         feedback $coma
         if [catch {exec $PROTEUMHOME/pteste -create \
                                                -S $test(src) \
                                                -E $test(exec) \
                                                -D $test(dir) \
                                                -C $test(comp) \
                                                 -$test(type) \
                                                $test(tfile)} rpteste] {
                message $rpteste
                return 0
          }
          feedback $rpteste
          return 1
      }
      
      LIST {
         set coma "\nPROTEUM> ptest -l $test(tfile)"
         feedback $coma
         if [catch {exec $PROTEUMHOME/pteste -l             \
                                -D $test(dir)  \
                                $test(tfile)} rpteste] {
               message $rpteste
               return 0
            } 
            feedback $rpteste
            return $rpteste
      }
   }
}


 
#------------------------------------------------------------------------------#
#   Li {}
#   Calls executable file LI that transforms a C program into a intermediate
#   representation called LI.
#   date: 04/09/96
#   last update: 04/09/96, 09/16/96, 09/17/96, 11/04/96
#------------------------------------------------------------------------------#

proc Li {operation args} {
   global test
   global in
   global MAXFUNCTIONS
   global functions
   global index_func
   global PROTEUMHOME

#   set test(dir) $in(dir)
#   set test(src) $in(src)

   switch -exact $operation {

      NEWSES {
         set coma "\nPROTEUM> li -D $test(dir) -P __$test(src) $test(src) \
__$test(src)"
         feedback $coma
         if [catch {exec $PROTEUMHOME/li -P __$test(src)  \
                            -D $test(dir)                 \
                            $test(src)                    \
                            __$test(src)} rli] {;   
             message $rli
             return 0
         }
         feedback $rli
      }
      LISTFUNC {
         set coma "\nPROTEUM> li -l -D $test(dir) __$test(src) __$test(src)"
         feedback $coma
         if [catch {exec $PROTEUMHOME/li -l     \
                            -D $test(dir)       \
                            __$test(src)        \
                            __$test(src)} rli] {;  # rli has list of functions
             message $rli
             return 0
         } else {
            feedback $rli
            set rli $rli\n         
            set first [string first \n $rli]
            set end [string last \n $rli]
            set i 0
            while {$first != $end} {
               set first [string first \n $rli]
               set end [string last \n $rli]
               set functions($i) [string range $rli 0 [expr $first-1]]
               set rli [string range $rli [expr $first+1] $end]
               incr i
            }
            set MAXFUNCTIONS $i
         }
      }
   }
   return 1
}


#------------------------------------------------------------------------------#
#   TCASE {}
#   Calls executable file TCASE that creates and handles test cases file.
#   date: 04/09/96
#   last update: 04/15/96, 11/04/96
#------------------------------------------------------------------------------#

proc Tcase {operation args} {
   global in;          # In data
   global test;        # Test program information
   global ntcase;      # Number of current test case
   global PROTEUMHOME

   switch -exact $operation {

      CREATE {
         set coma "\nPROTEUM> tcase -create -D $test(dir) $test(tfile)"
         feedback $coma
         if [catch {exec $PROTEUMHOME/tcase -create             \
                               -D $test(dir)                    \
                               $test(tfile)} rtcase] {
            message $rtcase;       # Error creation test case
            return 0
         } else {
            feedback $rtcase;
            return 1
         }
      }

      ADD {
# OBS: O PROGRAMA TCASE ESTA' RETORNANDO ALGUM CODIGO QUE FAZ COM QUE O CATCH
# ACUSE ALGUM ERRO. ASSIM SEMPRE O RAMO THEN DO IF ESTA' SENDO EXECUTADO.

         set coma "\nPROTEUM> tcase -add -D $test(dir) -E $test(exec) \
-p [lindex $args 0] -EE __$test(src)_inst -trace $test(tfile)"
         feedback $coma
         if [catch {exec xterm -e $PROTEUMHOME/tcase -add                 \
                                  -DD $test(dir)                 \
                                  -E $test(exec)                 \
                                  -p [lindex $args 0]            \
                                  -D $test(dir)                  \
                                  -EE __$test(src)_inst          \
                                  -trace                         \
                                  $test(tfile) } rtcase] {
            message $rtcase
            return 0
         } else {
            feedback $rtcase
            return 1
         }
      }

      VIEW {
         set coma "\nPROTEUM> tcase -l -D $test(dir) -x [lindex $args 0] \
$test(tfile)"
         feedback $coma
         if [catch {exec $PROTEUMHOME/tcase -l                  \
                               -x [lindex $args 0]              \
                               -D $test(dir)                    \
                               $test(tfile)} rtcase] {

            message $rtcase; # Error view test case
            return 0
         } else {
            feedback $rtcase
            return $rtcase;  # OBS: RETORNA A SAIDA PARA O PROGRAMA CHAMADOR
         }
      }

      ENABLE {
         set coma "\nPROTEUM> tcase -e -D $test(dir) -x $ntcase \
$test(tfile)"
         feedback $coma
         if [catch {exec $PROTEUMHOME/tcase -e                  \
                               -x $ntcase                       \
                               -D $test(dir)                    \
                               $test(tfile)} rtcase] {

            message $rtcase; # Error enable test case
            return 0
         } else {
            feedback $rtcase
            return 1
         }
      }

      DISABLE {
         set coma "\nPROTEUM> tcase -i -D $test(dir) -x $ntcase \
$test(tfile)"
         feedback $coma
         if [catch {exec $PROTEUMHOME/tcase -i                  \
                               -x $ntcase                       \
                               -D $test(dir)                    \
                               $test(tfile)} rtcase] {

            message $rtcase; # Error disable test case
            return 0
         } else {
            feedback $rtcase
            return 1
         }
      }

      DELETE {
         set coma "\nPROTEUM> tcase -e -D $test(dir) -f [lindex $args 0] \
-t [lindex $args 1] $test(tfile)"
         feedback $coma
         if [catch {exec $PROTEUMHOME/tcase -d                  \
                               -f [lindex $args 0]              \
                               -t [lindex $args 1]              \
                               -D $test(dir)                    \
                               $test(tfile)} rtcase] {

            message $rtcase; # Error delection test case
            return 0
         } else {
            feedback $rtcase
            return 1
         }
      }

      IMPORT {
         switch -exact [lindex $args 0] {

            PROTEUM {
               set coma "\nPROTEUM> tcase -proteum -D $test(dir) \
-DD [lindex $args 1]  -I [lindex $args 2] -f [lindex $args 3] \
-t [lindex $args 4] -E $test(exec) -EE __$test(src)_inst -trace $test(tfile)"
               feedback $coma
               catch {exec $PROTEUMHOME/tcase -proteum      \
                                 -DD [lindex $args 1]       \
                                 -I  [lindex $args 2]       \
                                 -f  [lindex $args 3]       \
                                 -t  [lindex $args 4]       \
                                 -DE $test(dir)             \
                                 -E  $test(exec)            \
                                 -D  $test(dir)             \
                                 -EE __$test(src)_inst      \
                                 -trace                     \
                                 $test(tfile)} rtcase
               feedback $rtcase
               return $rtcase;     # Number of imported test case from Proteum
            }

            POKE {
               set coma "\nPROTEUM> tcase -poke -D $test(dir) \
-DD [lindex $args 1] -f [lindex $args 2] \
-t [lindex $args 3] -E $test(exec) -EE __$test(src)_inst -trace $test(tfile)"
               feedback $coma
               catch {exec $PROTEUMHOME/tcase -poke         \
                                 -DD [lindex $args 1]       \
                                 -f  [lindex $args 2]       \
                                 -t  [lindex $args 3]       \
                                 -D  $test(dir)             \
                                 -DE $test(dir)             \
                                 -E  $test(exec)            \
                                 -EE __$test(src)_inst      \
                                 -trace                     \
                                 $test(tfile)} rtcase
               feedback $rtcase
               return $rtcase;     # Number of imported test case from Poke-tool
            }

            ASCII {
	       set tmp ""
	       if { [string length [lindex $args 2] ] != 0} {
		   set tmp " -I  [lindex $args 2]"
	       }
	       if { [string length [lindex $args 3] ] != 0} {
		   set tmp " $tmp -p  [lindex $args 3]"
	       }
               set coma "\nPROTEUM> tcase -proteum -D $test(dir) \
-DD [lindex $args 1]  $tmp -f [lindex $args 4] \
-t [lindex $args 5] -E $test(exec) -EE __$test(src)_inst -trace $test(tfile)"
               feedback $coma
               catch {eval exec $PROTEUMHOME/tcase -ascii               \
                                 -DD [lindex $args 1] \
				$tmp 		      \
                                 -f  [lindex $args 4] \
                                 -t  [lindex $args 5] \
                                 -D  $test(dir)       \
				 -E  $test(exec)      \
                                 -EE __$test(src)_inst      \
                                 -trace                     \
                                 $test(tfile)} rtcase
               feedback $rtcase
               return $rtcase;    # Number of imported test case from Ascii
            }
         }
      }
   }
   return
}


#------------------------------------------------------------------------------#
#   LIST_GOOD {}
#   Calls executable file LIST_GOOD that disables or deletes non-effective 
#   test cases.
#   date: 11/07/96
#   last update: 11/07/96
#------------------------------------------------------------------------------#

proc List_Good {operation args} {
   global test;        # Test program information
   global PROTEUMHOME

   switch -exact $operation {

      DISABLE {
         set coma "\nPROTEUM> list-good -i -D $test(dir) $test(tfile)"
         feedback $coma
         if [catch {exec $PROTEUMHOME/list-good -i       \
                                      -D $test(dir)      \
                                      $test(tfile)} rlistgood] {
            feedback $rlistgood;
            return 0
         } else {
            feedback $rlistgood
            return 1
         }
      }
           
      DELETE {
         set coma "\nPROTEUM> list-good -d -D $test(dir) $test(tfile)"
         feedback $coma
          if [catch {exec $PROTEUMHOME/list-good -d         \
                                      -D $test(dir)        \
                                      $test(tfile)} rlistgood] {
            feedback $rlistgood;
            return 0
         } else {
            feedback $rlistgood
            return 1
         }     
      }
   }
   return
}



#------------------------------------------------------------------------------#
#   Muta {}
#   Calls executable file MUTA that creates and handles files of mutants
#   descriptors and executable file OPMUTA that applies mutation operators 
#   to source file.
#   date: 04/09/96
#   last update: 04/23/96, 11/04/96, 11/18/96
#------------------------------------------------------------------------------#

proc Muta {operation args} {
   global test
   global PROTEUMHOME
   global MAXCONNECTIONS
   global ALL_CONNEC_GEN
   global ONLY_SRC_FUNC
   global connection

   switch -exact $operation {

      CREATE {
         set coma "\nPROTEUM> muta -create -D $test(dir) $test(tfile)"
         feedback $coma
         if [catch {exec $PROTEUMHOME/muta -create     \
                           -D $test(dir)               \
                           $test(tfile)} rmuta] {
            message $rmuta;      # Error creation mutants file descriptor
            return 0
         } else {
            feedback $rmuta
            return 1
         }
      }

      ADD {
         set coma "\nPROTEUM> muta-gen -D $test(dir) $args \
$test(tfile)"
         feedback $coma
         if [catch {eval exec $PROTEUMHOME/muta-gen [lindex $args 0]   \
                                                   -D $test(dir) -q     \
                                                   $test(tfile)} rmuta] {
            feedback $rmuta
            set nmuta 0
            set nx [scan $rmuta "%d" nmuta]
            return $nmuta
          } else {
            feedback $rmuta
            set nmuta 0
            set nx [scan $rmuta "%d" nmuta]
            return $nmuta
          }
      }

      LIST {
         set coma "\nPROTEUM> muta  -D $test(dir) -x [lindex $args 0] \
$test(tfile)"
         feedback $coma
         if [catch {exec $PROTEUMHOME/muta -l             \
                                -x [lindex $args 0]       \
                                -D $test(dir)             \
                                $test(tfile)} rmuta] {
             message $rmuta;  # Error taking information about mutant
             return 0
         } else {
             feedback $rmuta
             return $rmuta;   # Returns information about mutant
         }

      }

      SEARCH {
	 set var ""
	 foreach value $args {append var $value}
	 if [catch {eval exec $PROTEUMHOME/muta $var      \
			 -D $test(dir)                    \
			 $test(tfile)} rmuta] {
             message $rmuta;  # Error taking information about mutant
             return 0
         } else {
             return $rmuta;   # Returns information about mutant
         }

      }

      EQUIV {
         set coma "\nPROTEUM> muta -equiv -D $test(dir) \
 -x [lindex $args 0] $test(tfile)"
          feedback $coma
          if [catch {exec $PROTEUMHOME/muta -equiv         \
                              -x [lindex $args 0]         \
                              -D $test(dir)               \
                              $test(tfile)} rmuta] {
            message $rmuta;   # Error 
            return 0
         } else {
            feedback $rmuta
            return 1
         }
      }
      NEQUIV {
         set coma "\nPROTEUM> muta -nequiv -D $test(dir) \
 -x [lindex $args 0] $test(tfile)"
          feedback $coma
          if [catch {exec $PROTEUMHOME/muta -nequiv               \
                              -x [lindex $args 0] \
                              -D $test(dir)         \
                              $test(tfile)} rmuta] {
            message $rmuta;   # Error 
            return 0
         } else {
            feedback $rmuta
            return 1
         }
      }
      VERIFY {

         # ESSA FUNCAO AINDA NAO ESTA DISPONIVEL NOS SCRIPTS DA PROTEUM V. 1.2.3
 
      }
    
   }
   return
}


#------------------------------------------------------------------------------#
#   EXEMUTA {}
#   Executes, selects or builds mutants.
#   date: 04/18/96
#   last update: 07/02/96, 11/04/96
#------------------------------------------------------------------------------#

proc Exemuta {operation args} {
   global test;
   global output;
   global TMP_DIR_FILE;
   global PROTEUMHOME
   global ALL_MUTANTS
   global in
   global MAXCONNECTIONS
   global ALL_CONNEC_SEL
   global connection
   global nmuta
   global execcan

   switch -exact $operation {

      EXEC {;         # Executes all mutants
	 set fileid [open "|$PROTEUMHOME/exemuta -exec    \
                                 -T [lindex $args 0]      \
                                 -D $test(dir)            \
                                 -v .                     \
                                 -trace                   \
                                 $test(tfile)" r]
	 if {$fileid > 0} {
	    set nmuta 0
	    while {[eof $fileid] != 1} {
	       set data [read $fileid 1]
	       if {$execcan != 0} {
	 	  break
	       }
	       if {$data == "."} {
		  set nmuta [expr $nmuta+1]
	       } else {

# OBS: esse break foi comentado pois o exemuta da ProteumIM retorna alem do 
# "." outros caracteres como "Group of..."
#                  break

               }
	       update
	    }
	    catch "close $fileid" 
	    return 0
	 } else {
	    return 1
	 }
      }

      DUALEXEC {;         # Executes all mutants mode dual
	 set fileid [open "|$PROTEUMHOME/exemuta -exec    \
                                 -T [lindex $args 0]      \
                                 -D $test(dir)            \
                                 -v .                     \
                                 -dual                    \
                                 -trace                   \
                                 $test(tfile)" r]
	 if {$fileid > 0} {
	    set nmuta 0
	    while {[eof $fileid] != 1} {
	       set data [read $fileid 1]
	       if {$execcan != 0} {
	 	  break
	       }
	       if {$data == "."} {
		  set nmuta [expr $nmuta+1]
	       } else {

# OBS: esse break foi comentado pois o exemuta da ProteumIM retorna alem do 
# "." outros caracteres como "Group of..."
#                  break

               }
	       update
	    }
	    catch "close $fileid" 
	    return 0
	 } else {
	    return 1
	 }
      }

      SELECT {;        # Selects mutants

          set coma "\nPROTEUM> exemuta -select [lindex $args 0] \
-D $test(dir) $test(tfile)"
          feedback $coma
          if [catch "exec $PROTEUMHOME/exemuta -select            \
                                           [lindex $args 0]    \
                                           -D $test(dir)       \
                                           $test(tfile)" rexemuta] {
                  message $rexemuta
                  return 0
          } else {
             feedback $rexemuta
             return 1
          }

      }
      BUILD {;         # Builds one mutant
          set coma "\nPROTEUM> exemuta -build-view -D $test(dir) \
-x [lindex $args 0] $test(tfile)"
          feedback $coma
          if [catch {exec $PROTEUMHOME/exemuta -build-view               \
                                              -x [lindex $args 0]   \
                                              -D $test(dir)         \
                                              $test(tfile)} rexemuta] {
            message "$rexemuta !"
            return 0
         } else {
            feedback $rexemuta
            return 1
         }
      }

      ORIGINAL {
         if [catch {exec $test(dir)/$test(exec) &} rexemuta] {
            message "$rexemuta !"
            return 0
         } else {
            return 1
         }

      }

      EXECONE {
#         Exemuta BUILD [lindex $args 0]
         set exec_name "muta[lindex $args 0]_$test(tfile)"
         if [catch {exec $test(dir)/$exec_name &} rexemuta] {
            message "$rexemuta !"
            return 0
         } else {
            puts $rexemuta
            return 1
         }


      }
   }

}


#------------------------------------------------------------------------------#
#   REPORT {}
#   Build a file with a report about test cases effectiveness.
#   date: 04/25/96
#   last update: 04/25/96, 11/04/96
#------------------------------------------------------------------------------#

proc Report {operation args} {

   global test
   global PROTEUMHOME

   switch -exact $operation {
      TCASE {
        set coma "PROTEUM> report -tcase -D $test(dir) \
-L [lindex $args 0] $test(tfile)"
        feedback $coma
        if [catch {exec $PROTEUMHOME/report -tcase              \
                                            -L [lindex $args 0] \
                                            -D $test(dir)       \
                                            $test(tfile)} rreport] {
           message $rreport;  #OBS: DESCOMENTAR ESSA LINHA DEPOIS
           return 0;    #OBS: DESCOMENTAR ESSA LINHA DEPOIS
        } else {
           feedback $rreport
           return 1
        }
      }
      GENERAL {
        if [catch {exec $PROTEUMHOME/report -g           \
                               -D $test(dir)             \
                               $test(tfile)} rreport] {
           message $rreport
           return 0     
        } else {
           return $rreport
        }
      }  
   }   
}


#------------------------------------------------------------------------------#
#   INSTRUM {}
#   Instrumentation.
#   date: 11/06/96
#   last update: 11/06/96
#------------------------------------------------------------------------------#

proc Instrum {} {
   global test
   global PROTEUMHOME

   set coma "\nPROTEUM> instrum -D $test(dir) -EE $test(tfile) __$test(src)"
   feedback $coma
   if [catch {exec $PROTEUMHOME/instrum  \
                   -D $test(dir)         \
                   -EE $test(tfile)      \
                   __$test(src)} rinstrum] {
      message $rinstrum
      return 0     
   } else {
      feedback $rinstrum
      # depois usar instrum -build para compilar
      set coma "\nPROTEUM> instrum -build -D $test(dir) __$test(src) $test(tfile)"
      feedback $coma
      if [catch {exec $PROTEUMHOME/instrum -build        \
                     -D $test(dir)                     \
                      __$test(src)                     \
                      $test(tfile) } rinstrum] {
         message $rinstrum
         return 0
      }
      feedback $rinstrum
      return 1
   }
}
