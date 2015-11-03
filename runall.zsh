#!/bin/zsh

set -e
setopt err_return

appDir=$(cd $0:h && print $PWD)


function die { echo 1>&2 $*; exit 1 }

() {
    local cmd
    for cmd in g++ lscpu sha1sum; do
	(($+commands[$cmd])) || die "Can't find $cmd"
    done
}

() {
    local lscpu=$1; shift
    
    cat $lscpu | sha1sum | read hash _

    (($#hash == 40)) || die "Can't get proper hash for lscpu output"

    resDir=$appDir/_results/$hash

    mkdir -p $resDir

    echo Saving result to _results/$hash

    cat $lscpu > $resDir/lscpu.output

    local src out i
    for src in $appDir/[a-z]*_bench.cpp; do
	echo Building $src
	if $appDir/buildcpp.zsh $src; then
	    out=$resDir/$src:t:r.ltsv
	    rm -f $out
	    echo Running $src:r 3 times...
	    for ((i=0; i < 3; i++)); do
		$src:r </dev/null >> $out
	    done
	else
	    echo "Can't build $src"
	fi
    done

} =(LANG=C lscpu | grep -v '^CPU MHz:')
