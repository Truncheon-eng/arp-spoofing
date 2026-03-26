INC_DIR := ./include
DISCOVER_DIR := ./bin
DISCOVER_SRC_DIR := ./src
DISCOVER_OBJ_DIR := ./obj

DISCOVER_EXE := $(DISCOVER_DIR)/discover.out
DISCOVER_SRC := $(wildcard $(DISCOVER_SRC_DIR)/*.cpp)
DISCOVER_OBJ := $(patsubst $(DISCOVER_SRC_DIR)/%.cpp, $(DISCOVER_OBJ_DIR)/%.o, $(DISCOVER_SRC))

CXX := g++
CXXFLAGS := -I$(INC_DIR)
DBGFLAGS := -g -DDEBUG_H


.PHONY:
	debug_discover
	discover
	clean_discover

debug_discover: CXXFLAGS += $(DBGFLAGS)

debug_discover: $(DISCOVER_EXE)

clean_discover:
	@if [ -d "$(DISCOVER_OBJ_DIR)" ]; then rm -rf $(DISCOVER_OBJ_DIR); fi
	@if [ -f "$(DISCOVER_EXE)" ]; then rm "$(DISCOVER_EXE)" ; fi

discover: $(DISCOVER_EXE)

$(DISCOVER_DIR) $(DISCOVER_OBJ_DIR):
	@mkdir -p $@

$(DISCOVER_EXE): $(DISCOVER_OBJ) | $(DISCOVER_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lnet -lpcap

$(DISCOVER_OBJ_DIR)/%.o: $(DISCOVER_SRC_DIR)/%.cpp | $(DISCOVER_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@