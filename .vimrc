map \c :!clear && [ -e build ] && rm -r build && python setup.py waf_configure
map \bd :!clear && python setup.py waf_build_debug
map \br :!clear && python setup.py waf_build_release
map \bh :!clear && behave 
map \bw :!clear && behave --tags=wip
map \rp :!clear && python %
map \rtd :!clear && LD_LIBRARY_PATH=build/debug build/debug/tests/snn-tests
map \rtr :!clear && LD_LIBRARY_PATH=build/release build/release/tests/snn-tests
