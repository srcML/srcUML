msg:
	@echo "all: build, compile, run, open"
	@echo "run: compile and run"
	@echo "compile: "
	@echo "build_ogdf: "
	@echo "clean: "

all: build_ogdf run
	eog play.svg

run: compile
	./play orig_files/cft/cft.dot play.svg

compile:
	clang++ play.cpp -D NDEBUG -std=c++11 -oplay -Xlinker ../../../builds/ogdf_build/libOGDF.a -Xlinker ../../../builds/ogdf_build/libCOIN.a -pthread
	
build_ogdf:
	cmake ../../../ogdf ../../../builds/ogdf_build
	make --directory=../../../builds/ogdf_build/ 
	sudo make --directory=../../../builds/ogdf_build/ install

clean:
	rm -f play
	