
#!/bin/sh
dir=$PWD
allfiles=`ls $dir/*.cpp`
for f in $allfiles
do
    clang-tidy -p=$PWD -header-filter=$dir $f -config="{Checks: '-*,readability-*', CheckOptions: [{key: readability-function-size.LineThreshold, value: 200}]}"
done
