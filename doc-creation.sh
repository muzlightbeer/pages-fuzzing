#!/bin/bash

for i in {0..2000}
do
	cp Index$i.zip to-mangle.pages/Index.zip
	cp -r to-mangle.pages/ doc$i.pages
done
