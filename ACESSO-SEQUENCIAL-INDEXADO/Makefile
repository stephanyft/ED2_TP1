CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -pedantic -Iinlcudes
CPPFLAGS := -MMD -MP

TARGET := programa
BUILD_DIR := build

SOURCES := app/main.c src/paginacao.c
OBJECTS := $(SOURCES:%.c=$(BUILD_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPENDENCIES)
