

file(GLOB_RECURSE C_SOURCES *.c)
file(GLOB_RECURSE CPP_SOURCES *.cpp)

idf_component_register(SRCS ${C_SOURCES}  ${CPP_SOURCES} 
                   INCLUDE_DIRS "."
                     )

                     #[[
idf_register_component( SRCS "lcd.c"
                        INCLUDE_DIRS "."
)
]]