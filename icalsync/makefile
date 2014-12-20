# Detect host environment
UNAME := $(shell uname)
ISMINGW = $(shell uname | grep -E -c "MINGW32")

# If the CC environment variable is not set, simply set it to gcc
ifeq ($(CC),)
	CC = gcc
endif

# If the CXX environment variable is not set, simply set it to g++
ifeq ($(CXX),)
	CXX = g++
endif

# We can use the BUILD_DIR environment variable to set the location of the output files
ifeq ($(BUILD_DIR),)
	BUILD_DIR = ./build
endif

# We can use the PREFIX environment variable to control the installation directory
ifeq ($(PREFIX),)
	PREFIX = /usr/local
endif

# Under MinGW, we want to use GCC and statically link with the standard libraries
EXE_EXT =
CXXFLAGS = -std=c++11
LDFLAGS = -lsimple-base -lb64
ifeq ($(ISMINGW),1)
	CC = gcc
	CXX = g++
	EXE_EXT = .exe
	LDFLAGS += -mwindows -s -static-libgcc -static-libstdc++
endif

# Under OSX, we use clang++ as the compiler and ensure we link against libstdc++, and the CoreServices and CoreFoundation frameworks
ifeq ($(UNAME), Darwin)
	CC = clang
	CXX = clang++
	LDFLAGS += -lstdc++ -framework CoreServices -framework CoreFoundation
endif

# Object files
OBJECT_FILES = $(BUILD_DIR)/obj/icalsync.o $(BUILD_DIR)/obj/DropboxDirectoryDetection.o $(BUILD_DIR)/obj/ConfigParser.o $(BUILD_DIR)/obj/TemplateManager.o $(BUILD_DIR)/obj/ReportGenerator.o

# Template HTML object files
TEMPLATE_FILES = $(BUILD_DIR)/obj/events_template_html.o $(BUILD_DIR)/obj/tasks_template_html.o

all: dirs $(BUILD_DIR)/bin/icalsync$(EXE_EXT)
	@echo Done!

$(BUILD_DIR)/bin/icalsync$(EXE_EXT): $(OBJECT_FILES) $(TEMPLATE_FILES)
	$(CXX) -o $@ $(OBJECT_FILES) $(TEMPLATE_FILES) $(CXXFLAGS) $(LDFLAGS)

$(BUILD_DIR)/obj/icalsync.o: ./source/icalsync.cpp ./source/ConfigParser.h ./source/DropboxDirectoryDetection.h ./source/ReportGenerator.h ./source/TemplateManager.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BUILD_DIR)/obj/DropboxDirectoryDetection.o: ./source/DropboxDirectoryDetection.cpp ./source/DropboxDirectoryDetection.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BUILD_DIR)/obj/ConfigParser.o: ./source/ConfigParser.cpp ./source/ConfigParser.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BUILD_DIR)/obj/TemplateManager.o: ./source/TemplateManager.cpp ./source/TemplateManager.h ./source/ConfigParser.h $(BUILD_DIR)/gen/events_template_html.h $(BUILD_DIR)/gen/tasks_template_html.h
	$(CXX) -c $< -o $@ $(CXXFLAGS) -I$(BUILD_DIR)/gen

$(BUILD_DIR)/obj/ReportGenerator.o: ./source/ReportGenerator.cpp ./source/ReportGenerator.h ./source/ConfigParser.h ./source/FileChangeMonitor.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Embedded template HTML files

$(BUILD_DIR)/obj/events_template_html.o: $(BUILD_DIR)/gen/events_template_html.c
	$(CC) -c $< -o $@

$(BUILD_DIR)/gen/events_template_html.h: $(BUILD_DIR)/gen/events_template_html.c
$(BUILD_DIR)/gen/events_template_html.c: ./source/templates/events_template.html
	compile_file $<
	mv ./source/templates/events_template_html.c $(BUILD_DIR)/gen/events_template_html.c
	mv ./source/templates/events_template_html.h $(BUILD_DIR)/gen/events_template_html.h

$(BUILD_DIR)/obj/tasks_template_html.o: $(BUILD_DIR)/gen/tasks_template_html.c
	$(CC) -c $< -o $@

$(BUILD_DIR)/gen/tasks_template_html.h: $(BUILD_DIR)/gen/tasks_template_html.c
$(BUILD_DIR)/gen/tasks_template_html.c: ./source/templates/tasks_template.html
	compile_file $<
	mv ./source/templates/tasks_template_html.c $(BUILD_DIR)/gen/tasks_template_html.c
	mv ./source/templates/tasks_template_html.h $(BUILD_DIR)/gen/tasks_template_html.h

dirs:
	@test -d $(BUILD_DIR) || mkdir $(BUILD_DIR)
	@test -d $(BUILD_DIR)/gen || mkdir $(BUILD_DIR)/gen
	@test -d $(BUILD_DIR)/obj || mkdir $(BUILD_DIR)/obj
	@test -d $(BUILD_DIR)/bin || mkdir $(BUILD_DIR)/bin

install_dirs:
	@test -d $(PREFIX) || mkdir $(PREFIX)
	@test -d $(PREFIX)/bin || mkdir $(PREFIX)/bin

install: install_dirs
	cp -r $(BUILD_DIR)/bin/* $(PREFIX)/bin/
	chmod 777 $(PREFIX)/bin/icalsync$(EXE_EXT)

clean:
	rm $(BUILD_DIR)/obj/*.o
