# Detect host environment
UNAME := $(shell uname)
ISMINGW = $(shell uname | grep -E -c "MINGW32")

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
LDFLAGS = -lsimple-base -lical -lpthread
ifeq ($(ISMINGW),1)
	CXX = g++
	EXE_EXT = .exe
	LDFLAGS += -s -static-libgcc -static-libstdc++
endif

# Under OSX, we use clang++ as the compiler and ensure we link against libstdc++
ifeq ($(UNAME), Darwin)
	CXX = clang++
	LDFLAGS += -lstdc++
endif

# Object files
OBJECT_FILES = $(BUILD_DIR)/obj/ics2html.o $(BUILD_DIR)/obj/ComponentWrapper.o $(BUILD_DIR)/obj/PropertyWrapper.o $(BUILD_DIR)/obj/ComponentAggregatorBase.o $(BUILD_DIR)/obj/HtmlGeneratorBase.o $(BUILD_DIR)/obj/HtmlReportGenerator.o $(BUILD_DIR)/obj/ReportImplementationFactory.o $(BUILD_DIR)/obj/EventAggregator.o $(BUILD_DIR)/obj/TaskAggregator.o $(BUILD_DIR)/obj/EventHtmlGenerator.o $(BUILD_DIR)/obj/TaskHtmlGenerator.o

all: dirs $(BUILD_DIR)/bin/ics2html$(EXE_EXT)
	@echo Done!

$(BUILD_DIR)/bin/ics2html$(EXE_EXT): $(OBJECT_FILES)
	$(CXX) -o $@ $(OBJECT_FILES) $(CXXFLAGS) $(LDFLAGS)

# General components

$(BUILD_DIR)/obj/ics2html.o: ./source/ics2html.cpp ./source/HtmlReportGenerator.h ./source/ComponentAggregatorBase.h ./source/HtmlGeneratorBase.h ./source/ical/ComponentWrapper.h ./source/ical/PropertyWrapper.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BUILD_DIR)/obj/ComponentAggregatorBase.o: ./source/ComponentAggregatorBase.cpp ./source/ComponentAggregatorBase.h ./source/ical/ComponentWrapper.h ./source/ical/PropertyWrapper.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BUILD_DIR)/obj/HtmlGeneratorBase.o: ./source/HtmlGeneratorBase.cpp ./source/HtmlGeneratorBase.h ./source/ical/ComponentWrapper.h ./source/ical/PropertyWrapper.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BUILD_DIR)/obj/HtmlReportGenerator.o: ./source/HtmlReportGenerator.cpp ./source/HtmlReportGenerator.h ./source/ComponentAggregatorBase.h ./source/HtmlGeneratorBase.h ./source/ical/ComponentWrapper.h ./source/ical/PropertyWrapper.h ./source/ReportImplementationFactory.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BUILD_DIR)/obj/ReportImplementationFactory.o: ./source/ReportImplementationFactory.cpp ./source/ReportImplementationFactory.h ./source/ComponentAggregatorBase.h ./source/HtmlGeneratorBase.h ./source/ical/ComponentWrapper.h ./source/ical/PropertyWrapper.h ./source/aggregators/EventAggregator.h ./source/aggregators/TaskAggregator.h ./source/generators/EventHtmlGenerator.h ./source/generators/TaskHtmlGenerator.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# iCal Wrappers

$(BUILD_DIR)/obj/ComponentWrapper.o: ./source/ical/ComponentWrapper.cpp ./source/ical/ComponentWrapper.h ./source/ical/PropertyWrapper.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BUILD_DIR)/obj/PropertyWrapper.o: ./source/ical/PropertyWrapper.cpp ./source/ical/PropertyWrapper.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Aggregators

$(BUILD_DIR)/obj/EventAggregator.o: ./source/aggregators/EventAggregator.cpp ./source/aggregators/EventAggregator.h ./source/ComponentAggregatorBase.h ./source/ical/ComponentWrapper.h ./source/ical/PropertyWrapper.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BUILD_DIR)/obj/TaskAggregator.o: ./source/aggregators/TaskAggregator.cpp ./source/aggregators/TaskAggregator.h ./source/ComponentAggregatorBase.h ./source/ical/ComponentWrapper.h ./source/ical/PropertyWrapper.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Generators

$(BUILD_DIR)/obj/EventHtmlGenerator.o: ./source/generators/EventHtmlGenerator.cpp ./source/generators/EventHtmlGenerator.h ./source/HtmlGeneratorBase.h ./source/ical/ComponentWrapper.h ./source/ical/PropertyWrapper.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BUILD_DIR)/obj/TaskHtmlGenerator.o: ./source/generators/TaskHtmlGenerator.cpp ./source/generators/TaskHtmlGenerator.h ./source/HtmlGeneratorBase.h ./source/ical/ComponentWrapper.h ./source/ical/PropertyWrapper.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

dirs:
	@test -d $(BUILD_DIR) || mkdir $(BUILD_DIR)
	@test -d $(BUILD_DIR)/obj || mkdir $(BUILD_DIR)/obj
	@test -d $(BUILD_DIR)/bin || mkdir $(BUILD_DIR)/bin

install_dirs:
	@test -d $(PREFIX) || mkdir $(PREFIX)
	@test -d $(PREFIX)/bin || mkdir $(PREFIX)/bin

install: install_dirs
	cp -r $(BUILD_DIR)/bin/* $(PREFIX)/bin/
	chmod 777 $(PREFIX)/bin/ics2html$(EXE_EXT)

clean:
	rm $(BUILD_DIR)/obj/*.o
