if(NOT TARGET numeric)
    ensure_submodule_initialized(numeric)
    add_subdirectory(numeric)
endif()
