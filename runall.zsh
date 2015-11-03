#!/bin/zsh

set -e
setopt err_return

appDir=$(cd $0:h && print $PWD)

integer ntests=3

export LANG=C

function die { echo 1>&2 $*; exit 1 }

() {
    local cmd
    for cmd in g++ lscpu sha1sum objdump rpm; do
	(($+commands[$cmd])) || die "Can't find $cmd"
    done
}

() {
    local lscpu=$1; shift
    
    cat $lscpu | sha1sum | read hash _
    
    export BENCHMARK_ID=$(date -Iseconds)

    (($#hash == 40)) || die "Can't get proper hash for lscpu output"

    resTop=$appDir/_results
    [[ -d $resTop ]] || {
	mkdir -p $resTop
	(cd $resTop && git init && git checkout -b host/$HOSTNAME)
    }

    resDir=$resTop/$hash

    mkdir -p $resDir

    local cf=$resDir/config.env
    if [[ -r $cf ]]; then
	source $cf
    fi

    echo Saving result to _results/$hash

    cat $lscpu > $resDir/lscpu.output

    local src base out i
    for src in $appDir/[a-z]*_bench.cpp; do
	echo Building ${src##$appDir/}
	if $appDir/buildcpp.zsh $src; then
	    base=$resDir/$src:t:r
	    out=$base.ltsv
	    echo Running ${${src:r}##$appDir/} $ntests times...
	    for ((i=0; i < $ntests; i++)); do
		$src:r </dev/null >> $out
	    done
	    echo >> $out
	    objdump -CS $src:r > $base.S
	else
	    echo "Can't build $src"
	fi
    done

    (cd $resTop && git add -A && git commit -m "BENCHMARK_ID: $BENCHMARK_ID")

} =(echo -n $'uname:\t'; uname -m -s -r;
    echo -n $'glibc:\t'; rpm -q glibc.x86_64;
    echo -n $'g++:\t'; rpm -qf =g++;
    echo;
    LANG=C lscpu | grep -v '^CPU MHz:')
