INC_DIR := ./include
SPOOF_DIR := ./bin
SPOOF_SRC_DIR := ./src
SPOOF_OBJ_DIR := ./obj

SPOOF_EXE := $(SPOOF_DIR)/spoofing.out
SPOOF_SRC := $(wildcard $(SPOOF_SRC_DIR)/*.c)
SPOOF_OBJ := $(patsubst $(SPOOF_SRC_DIR)/%.c, $(SPOOF_OBJ_DIR)/%.o, $(SPOOF_SRC))

CXX := gcc
CXXFLAGS := -I$(INC_DIR)
DBGFLAGS := -g -DDEBUG_H


.PHONY:
	debug_spoofing
	spoofing
	clean_spoofing

debug_spoofing: CXXFLAGS += $(DBGFLAGS)

debug_spoofing: $(SPOOF_EXE)

clean_spoofing:
	@if [ -d "$(SPOOF_OBJ_DIR)" ]; then rm -rf $(SPOOF_OBJ_DIR); fi
	@if [ -f "$(SPOOF_EXE)" ]; then rm "$(SPOOF_EXE)" ; fi

spoofing: $(SPOOF_EXE)

$(SPOOF_DIR) $(SPOOF_OBJ_DIR):
	@mkdir -p $@

$(SPOOF_EXE): $(SPOOF_OBJ) | $(SPOOF_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lnet

$(SPOOF_OBJ_DIR)/%.o: $(SPOOF_SRC_DIR)/%.c | $(SPOOF_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@