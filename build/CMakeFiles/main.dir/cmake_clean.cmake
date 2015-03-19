FILE(REMOVE_RECURSE
  "scanner.c"
  "parser.c"
  "parser.h"
  "parser.output"
  "CMakeFiles/main.dir/scanner.c.o"
  "CMakeFiles/main.dir/parser.c.o"
  "CMakeFiles/main.dir/src/main.c.o"
  "CMakeFiles/main.dir/src/cc_misc.c.o"
  "CMakeFiles/main.dir/src/cc_gv.c.o"
  "CMakeFiles/main.dir/src/cc_dict.c.o"
  "CMakeFiles/main.dir/src/cc_list.c.o"
  "CMakeFiles/main.dir/src/cc_tree.c.o"
  "main.pdb"
  "main"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C)
  INCLUDE(CMakeFiles/main.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
