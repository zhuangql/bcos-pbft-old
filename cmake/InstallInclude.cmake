set(DESTINATION_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}/bcos-pbft")
install(
    DIRECTORY "framework"
    DESTINATION "${DESTINATION_INCLUDE_DIR}"
    FILES_MATCHING PATTERN "*.h"
)

install(
    DIRECTORY "core"
    DESTINATION "${DESTINATION_INCLUDE_DIR}"
    FILES_MATCHING PATTERN "*.h"
)

install(
    DIRECTORY "pbft"
    DESTINATION "${DESTINATION_INCLUDE_DIR}"
    FILES_MATCHING PATTERN "*.h"
)