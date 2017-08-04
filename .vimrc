map \c :!clear && [ -e build ] && rm -r build && python setup.py waf_configure
map \b :!clear && python setup.py waf_build
map \br :!clear && behave 
map \bw :!clear && behave --tags=wip
map \r :!clear && python %
