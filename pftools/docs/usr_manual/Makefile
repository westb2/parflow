default:
	cd ../../../docs/ && make html

#	pandoc --template=$(HOME)/.pandoc/default.latex user_manual.tex -o sphinx/source/user_manual.rst
convert:
	pandoc user_manual.tex -o ../../../docs/source/user_manual.rst
