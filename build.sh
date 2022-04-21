mkdir -p bin &&
pushd Lexicalli && ./build.sh && popd && cp Lexicalli/bin/calli bin/calli && 
pushd Syntan && ./build.sh && popd && cp Syntan/bin/syntan bin/syntan && 
g++ src/*.cpp -Llib -Iinclude -lKirbParse_Static -o bin/jazzy -Wno-write-strings