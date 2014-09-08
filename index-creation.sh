#!/bin/bash
for i in {0..2000}
do
	cp  Document$i.iwa Index/Document.iwa
	zip -r -Z store Index$i.zip Index	
done
