.PHONY: configure build clean run

VCPKG_ROOT := C:/dev/vcpkg
BUILD_DIR := build/x64-windows
EXECUTABLE := $(BUILD_DIR)/Debug/hello_sfml.exe

configure:
	@set VCPKG_ROOT=$(VCPKG_ROOT) && cmake --preset x64-windows

build: configure
	@cmake --build --preset x64-windows

clean:
	@if exist build rmdir /s /q build

run: build
	@$(EXECUTABLE)

all: build