#! /bin/bash
dir=builtins
OS_ARCH=$(uname -s)
if [ $OS_ARCH = SunOS ] ; then
	M_ARCH=$(isainfo -n)
elif [ $OS_ARCH = Linux ] ; then
	M_ARCH=$(uname -m)
fi

for fn in $(ls ${dir}) ; do
	echo $dir/$fn
	../../../bin/${OS_ARCH}_${M_ARCH}/64/aml_test -d -f $dir/$fn -m 
done
