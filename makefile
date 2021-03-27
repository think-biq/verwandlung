#
#    Utility to inspect FBX files.
#
#    2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
#    See license.md for details.
#
#    https://think-biq.com

FILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
PROJECT_DIR := $(shell dirname $(FILE_PATH))
PROJECT_NAME := $(notdir $(patsubst %/,%,$(dir $(FILE_PATH))))

DEBUG_FLAG :=
BUILD_PATH = "${PROJECT_DIR}/build"
PYBIND_WANDEL_MODULE_NAME = python_wandel

ifeq '$(findstring ;,$(PATH))' ';'
	OS = "win"
	VENV_BIN_DIR = Scripts
	STD_FLAG = "/std:c++17 /EHa"
	EXE_NAME = Verwandlung.exe
	BUILD_PATH_RELEASE = "${BUILD_PATH}/Release"
	EXE_PATH_RELEASE = "${BUILD_PATH_RELEASE}/${EXE_NAME}"
	BUILD_PATH_DEBUG = "${BUILD_PATH}/Debug"
	EXE_PATH_DEBUG = "${BUILD_PATH_DEBUG}/${EXE_NAME}"
	LLDB = lldb
	PYTHON = python
	PYTHON_EXECUTABLE := "${shell $(PYTHON) -c "import sys; print(sys.executable)"}"
else
	OS = "unix-y"
	VENV_BIN_DIR = bin
	STD_FLAG = "--std=c++17"
	EXE_NAME = Verwandlung
	BUILD_PATH_RELEASE = "${BUILD_PATH}"
	EXE_PATH_RELEASE = "${BUILD_PATH_RELEASE}/${EXE_NAME}"
	BUILD_PATH_DEBUG = "${BUILD_PATH}"
	EXE_PATH_DEBUG = "${BUILD_PATH_DEBUG}/${EXE_NAME}"
	LLDB = lldb
	PYTHON := python3
	PYTHON_EXECUTABLE := $(shell which $(PYTHON))
endif

CMD_ACTIVATE_VENV = . "$(PROJECT_DIR)/$(VENV_BIN_DIR)/activate"


default: build


$(VENV_BIN_DIR): # creates target which only gets triggered if directory does not exist
	@echo "Creating venv ..."
	@$(PYTHON) -m venv "$(PROJECT_DIR)"

venv-upgrade: $(VENV_BIN_DIR)
	@echo "Upgrading venv ..."
	@$(PYTHON) -m venv --upgrade "$(PROJECT_DIR)"
	@$(CMD_ACTIVATE_VENV); $(PYTHON) -m pip install -U -r "$(PROJECT_DIR)/requirements.txt"

venv-remove: 
	@echo "Removing venv ..."
	@rm -rf "$(PROJECT_DIR)/bin"
	@rm -rf "$(PROJECT_DIR)/include"
	@rm -rf "$(PROJECT_DIR)/lib"
	@rm -rf "$(PROJECT_DIR)/pyvenv.cfg"

list-venv-version:
	@$(CMD_ACTIVATE_VENV); $(PYTHON) -V

list-venv-packages:
	@$(CMD_ACTIVATE_VENV); $(PYTHON) -m pip list

setup-env: venv-upgrade
	mkdir -p ./tmp ./bin
	mkdir -p ./build
	@echo PROJECT_DIR:$(PROJECT_DIR)
	@echo OS:$(OS)
	@echo STD_FLAG:$(STD_FLAG)
	@echo EXE_PATH: $(EXE_PATH)
	@echo PYTHON_EXECUTABLE: $(PYTHON_EXECUTABLE)

setup-cmake-release:
	pushd ${BUILD_PATH}; $(CMD_ACTIVATE_VENV); cmake -DCMAKE_BUILD_TYPE="Release" \
		-DPYTHON=$(PYTHON) \
		-DPYTHON_EXECUTABLE=$(PYTHON_EXECUTABLE) \
		-DCMAKE_CXX_FLAGS=$(STD_FLAG) ..

setup-cmake-debug:
	pushd ${BUILD_PATH}; $(CMD_ACTIVATE_VENV); cmake -DCMAKE_BUILD_TYPE="Debug" \
		-DPYTHON=$(PYTHON) \
		-DPYTHON_EXECUTABLE=$(PYTHON_EXECUTABLE) \
		-DCMAKE_CXX_FLAGS=$(STD_FLAG) ..

setup-release: setup-env setup-cmake-release

setup-debug: setup-env setup-cmake-debug

build-release: setup-release
	pushd ${BUILD_PATH}; $(CMD_ACTIVATE_VENV); cmake --build . --config Release

build-debug: setup-debug
	pushd ${BUILD_PATH}; $(CMD_ACTIVATE_VENV); cmake --build . --config Debug

build-wheel-release:
	@pushd "${BUILD_PATH_RELEASE}/src/${PYBIND_WANDEL_MODULE_NAME}/package"; \
		$(CMD_ACTIVATE_VENV); $(PYTHON) setup.py bdist_wheel

build-wheel-debug:
	echo "WOOT?"
	@pushd "${BUILD_PATH_DEBUG}/src/${PYBIND_WANDEL_MODULE_NAME}/package"; \
		$(CMD_ACTIVATE_VENV); $(PYTHON) setup.py bdist_wheel

clean:
	@echo "Removing build artefacts ..."
	@rm -rf "$(PROJECT_DIR)/tmp"
	@rm -rf "$(PROJECT_DIR)/build"

clean-all: venv-remove clean

build: build-release build-wheel-release

run: build-release
	$(EXE_PATH) --list ./etc/fbx/OldFace.fbx

debug: build-debug
	$(LLDB) -- $(EXE_PATH_RAW) --list ./etc/fbx/Gunan_animiated.fbx