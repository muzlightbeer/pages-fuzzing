pages-fuzzing
=============

Some (lame) things I made as crash wrangler was not running for me

I usually do this:

1) create a new Pages document
2) change into the document and unzip Index.zip
3) copy the Index/Document.iwa file to your test case generation directory
4) generate a bunch of malformed Document.iwa files. These scripts assume Document%d.iwa naming
5) copy the original Index directory into the test case generation directory
6) run index-creation.sh
7) copy the original pages document (the whole directory) into the test case generation directory, naming it to-mangle.pages
8) run doc-creation.sh
9) run the harness after changing the test case directory path on line 51, may need some other sleep/timing adjustments for your system
