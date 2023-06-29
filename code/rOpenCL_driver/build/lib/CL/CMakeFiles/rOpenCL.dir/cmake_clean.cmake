file(REMOVE_RECURSE
  "librOpenCL.pdb"
  "librOpenCL.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/rOpenCL.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
