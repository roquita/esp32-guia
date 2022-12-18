set(srcs "main.c" "max7219/max7219.c" "rest_server.c")

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "." "max7219"
                   )


set(WEB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../front") 
spiffs_create_partition_image(www ${WEB_SRC_DIR} FLASH_IN_PROJECT)                 