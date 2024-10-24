PROJECT=3dviewer
DIR_PROJECT=viewer

all: build

create:
	cmake cmake_src/ -B build/ -D CMAKE_BUILD_TYPE=Realise

build: clean create
	make -C build

run: install
ifeq ($(shell uname),Linux) # Linux
	open realise/$(PROJECT)
endif
ifeq ($(shell uname),Darwin) # MacOS
	open realise/$(PROJECT).app
endif

install: build
	-rm -rf realise
	-mkdir realise
ifeq ($(shell uname),Linux) # Linux
	cp -r build/$(PROJECT) realise
endif
ifeq ($(shell uname),Darwin) # MacOS
	cp -r build/$(PROJECT).app realise
endif

dvi: clean
	echo "create doc" 
	bash support/run.sh

uninstall:
	-rm -rf realise

clean:
	-rm -rf build* dvi 
	-rm cmake_src/CMakeLists.txt.user
	make -C $(DIR_PROJECT)_back clean
	make -C $(DIR_PROJECT)_back  clean_test

dist: 
	-rm -rf $(PROJECT)
	mkdir $(PROJECT)
	cp -r $(DIR_PROJECT)_front $(PROJECT)/$(DIR_PROJECT)_front
	cp -r $(DIR_PROJECT)_back $(PROJECT)/$(DIR_PROJECT)_back
	cp -r cmake_src $(PROJECT)/cmake_src
	cp -r support $(PROJECT)/support
	cp -r sample $(PROJECT)/sample
	cp Doxyfile $(PROJECT)
	cp Makefile $(PROJECT)
	cp README.md $(PROJECT)
	tar -czvf $(PROJECT).tar.gz $(PROJECT)/
	-rm -rf $(PROJECT)

test:
	make -C $(DIR_PROJECT)_back test

test_leaks:
	make -C $(DIR_PROJECT)_back test_leaks

gcov_report:
	make -C $(DIR_PROJECT)_back gcov_report

clang: clean
# ifeq ("","$(wildcard ./.clang-format)") # если файла нет, копируем его.
# 	cp -f ../materials/linters/.clang-format ./.clang-format
# endif
	find . -type f -name "*.c" -o -name "*.h" -o -name "*.cpp" | xargs clang-format -style=Google -n
#	rm -f ./.clang-format

sample:
	curl -O https://outtrip.ru/index.php/s/YA9ET2PBRkZEswc/download/sample.tar.gz
	tar -xvzf sample.tar.gz 
	rm sample.tar.gz

.PHONY: dvi sample
