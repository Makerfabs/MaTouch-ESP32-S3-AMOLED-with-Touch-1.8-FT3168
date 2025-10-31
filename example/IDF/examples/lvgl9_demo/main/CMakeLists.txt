set(SOURCES 
            "display/lcd_display.c"
            "main.c"


            )

set(INCLUDE_DIRS "." "display")

# Add board files according to BOARD_TYPE
# Set default assets if the board uses partition table V2
if(CONFIG_BOARD_TYPE_MATOUCH_AI_28_S3)
    set(BOARD_TYPE "matouch-ai-28-s3")
elseif(CONFIG_BOARD_TYPE_MATOUCH_3_16_S3)
    set(BOARD_TYPE "matouch-3-16-s3")
elseif(CONFIG_BOARD_TYPE_MATOUCH_1_8_S3)
    set(BOARD_TYPE "matouch-1-8-s3")
endif()

file(GLOB BOARD_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD_TYPE}/*.c
)
list(APPEND SOURCES ${BOARD_SOURCES})

file(GLOB MARKET_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/for_market/${BOARD_TYPE}/*.c
)
list(APPEND SOURCES ${MARKET_SOURCES})

file(GLOB MARKET_ASSET_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/for_market/${BOARD_TYPE}/assets/*.c
)
list(APPEND SOURCES ${MARKET_ASSET_SOURCES})


idf_component_register(SRCS ${SOURCES}
                    # EMBED_FILES ${LANG_SOUNDS} ${COMMON_SOUNDS}
                    INCLUDE_DIRS ${INCLUDE_DIRS}
                    # WHOLE_ARCHIVE
                    )