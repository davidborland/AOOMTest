CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

PROJECT( AOOMTest )

SET( EXECUTABLE_OUTPUT_PATH "${AOOMTest_BINARY_DIR}/bin" )


#######################################
# Include Haggis
#######################################

FIND_PATH( HAGGIS_SRC_DIR Renci/RenciFrame.h )
FIND_PATH( HAGGIS_BIN_DIR Haggis.sln )

INCLUDE_DIRECTORIES( ${HAGGIS_SRC_DIR}/Graphics
                     ${HAGGIS_SRC_DIR}/Media
                     ${HAGGIS_SRC_DIR}/Quat
                     ${HAGGIS_SRC_DIR}/Utilities )
LINK_DIRECTORIES( ${HAGGIS_BIN_DIR}/Graphics
                  ${HAGGIS_BIN_DIR}/Media
                  ${HAGGIS_BIN_DIR}/Quat
                  ${HAGGIS_BIN_DIR}/Utilities )

SET( HAGGIS_LIBS Graphics.lib Media.lib Quat.lib Utilities.lib )



#######################################
# Include glut
#######################################

FIND_PATH( GLUT_DIR glut32.lib )

INCLUDE_DIRECTORIES( ${GLUT_DIR} )
LINK_DIRECTORIES( ${GLUT_DIR} )

SET( GLUT_LIB glut32.lib )


#######################################
# Include glew
#######################################

FIND_PATH( GLEW_DIR lib/glew32.lib )

INCLUDE_DIRECTORIES( ${GLEW_DIR}/include )
LINK_DIRECTORIES( ${GLEW_DIR}/lib )

SET( GLEW_LIB glew32.lib )


#######################################
# Include DevIL
#######################################

FIND_PATH( DEVIL_DIR lib/DevIL.lib )

INCLUDE_DIRECTORIES( ${DEVIL_DIR}/include )
LINK_DIRECTORIES( ${DEVIL_DIR}/lib )

SET( DEVIL_LIBS DevIL.lib ILU.lib ILUT.lib )


#######################################
# Include AOOMTest code
#######################################

SET( SRC AOOMTest.cpp 
         AOOMGraphics.h AOOMGraphics.cpp )
ADD_EXECUTABLE( AOOMTest MACOSX_BUNDLE ${SRC} )
TARGET_LINK_LIBRARIES( AOOMTest ${HAGGIS_LIBS} ${GLUT_LIB} ${GLEW_LIB} ${DEVIL_LIBS} )   