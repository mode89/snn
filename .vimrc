map \c :!clear && [ -e build ] && rm -r build && python setup.py waf_configure
map \bd :!clear && python setup.py waf_build_debug
map \br :!clear && python setup.py waf_build_release
map \bh :!clear && behave 
map \bw :!clear && behave --tags=wip
map \r :!clear && python %
