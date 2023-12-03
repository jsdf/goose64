
# defines source files
include ./common.makefile

# defines machine-specific variables for opengl build
include ./gldefs.makefile

print-all-vars:
    $(foreach var,$(.VARIABLES),$(info $(var) = $($(var))))
