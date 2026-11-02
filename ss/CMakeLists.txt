if(NOT TARGET numeric)
    ariadne_add_submodule(numeric)
endif()

# Test-only submodule, excluded from the production interface target.
ariadne_setup_testing()
