mkdir -p bin &&
pushd Lexicalli && ./build.sh && popd && cp Lexicalli/bin/calli bin/calli && 
pushd Syntan && ./build.sh && popd && cp Syntan/bin/syntan bin/syntan && 
pushd KirbParse && cmake . && make && popd && cp KirbParse/src/kirbparse.h include/ && cp KirbParse/lib/libKirbParse_Static.a lib/
g++ src/*.cpp -Llib -Iinclude -lKirbParse_Static -o bin/jazzy -Wno-write-strings