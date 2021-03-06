########################
###### GAME STACK ###### 
########################

FIND_PACKAGE( OpenCV REQUIRED )
FIND_PACKAGE( SDL REQUIRED ) #<- how is it wrote???

ADD_LIBRARY(gameStack SHARED GameMachine.cxx FactoryGameMachine.cxx TrainingSection.cxx
TrainingDirector.cxx)

FILE(COPY ${PROJECT_SOURCE_DIR}/gameLogic/Moves.xml 
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

FILE(COPY ${PROJECT_SOURCE_DIR}/gui/guichan-0.8.2/examples/rpgfont.bmp
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	
########################
##### directories ######
######################## 
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/gameStack")
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/gameStack/menu")
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/gameStack/controller")
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/recognitionEngine")
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/gui")
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/gameLogic")

ADD_DEPENDENCIES(gameStack GameElement.h)
ADD_DEPENDENCIES(gameStack controllerLib menuLib gameLogic recognitionEngine timersNHSF)


ADD_SUBDIRECTORY(menu)
ADD_SUBDIRECTORY(controller)
ADD_SUBDIRECTORY(timers)

########################
### executable test ####
########################
ADD_EXECUTABLE(gameStackTest gameStackTest.cxx)
TARGET_LINK_LIBRARIES(gameStackTest ${OpenCV_LIBS} gameStack gameLogic menuLib controllerLib gui
evaluationFunctions processingFunctions
guichan guichan_sdl deb camLib recognitionEngine engineStrategies engineModules timersNHSF )
