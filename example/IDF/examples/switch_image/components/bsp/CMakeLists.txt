
set(SOURCES)
set(INCLUDE_DIRS)
set(REQUIRES_LIST)

# Check if CONFIG_BSP_SPIFFS is enabled
if(CONFIG_BSP_SPIFFS)
    list(APPEND SOURCES "bsp_spiffs/bsp_spiffs.c")
    list(APPEND INCLUDE_DIRS "bsp_spiffs")
    list(APPEND REQUIRES_LIST "spiffs")

    # 获取 spiffs 组件的目录和 include 列表，确保追加的是绝对路径
    idf_component_get_property(SPIFFS_DIR spiffs COMPONENT_DIR)
    idf_component_get_property(SPIFFS_INCLUDES spiffs INCLUDE_DIRS)
    if(SPIFFS_INCLUDES)
        foreach(INC ${SPIFFS_INCLUDES})
            if(IS_ABSOLUTE "${INC}")
                list(APPEND INCLUDE_DIRS "${INC}")
            else()
                list(APPEND INCLUDE_DIRS "${SPIFFS_DIR}/${INC}")
            endif()
        endforeach()
    else()
        if(DEFINED ENV{IDF_PATH})
            list(APPEND INCLUDE_DIRS "$ENV{IDF_PATH}/components/spiffs/include")
        endif()
    endif()
endif()

message(STATUS "Component SOURCES: ${SOURCES}")
message(STATUS "Component INCLUDE_DIRS: ${INCLUDE_DIRS}")
message(STATUS "Component REQUIRES_LIST: '${REQUIRES_LIST}'")

idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS ${INCLUDE_DIRS}
                    REQUIRES ${REQUIRES_LIST}
                    )
