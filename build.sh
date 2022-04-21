pushd Lexicalli && ./build.sh && popd && cp Syntan/bin/syntan bin/syntan && \
pushd Syntan && ./build.sh && popd && cp Lexicalli/bin/calli bin/calli # && \ 