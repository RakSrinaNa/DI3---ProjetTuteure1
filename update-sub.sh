#!/bin/bash

endScriptError() {
    qdbus $dbusRef close 2> /dev/null > /dev/null
    exit 1
}


if [[ -z "$(which wget)" ]]; then
    echo "wget not found."
    exit 1
fi


#if find . -maxdepth 0 -empty | read; then

    dir="../$(basename $(pwd))-$(date +%F#%T|tr ':' '-')"

    if [[ -d "$dir" ]]; then
        echo "Can not create backup ! $dir already exists."
        endScriptError
    fi

    echo "Backuping to $dir..."
    mkdir "$dir"
    cp -r . "$dir/" || (echo "Can not create backup !"; endScriptError)
    rmdir "$dir" 2>/dev/null >/dev/null
#fi

arch=$(uname -m)

rm -rf update
mkdir -p update

wget -q --no-check-certificate https://storage.projectsforge.org/update/c/base-$arch.tar.gz -O update/base.tar.gz || endScriptError
wget -q --no-check-certificate https://storage.projectsforge.org/update/c/doc.tar.gz -O update/doc.tar.gz 2>&1 || endScriptError
wget -q --no-check-certificate https://storage.projectsforge.org/update/c/user-$arch.tar.gz -O update/user.tar.gz || endScriptError

tar -xzf update/base.tar.gz || endScriptError
tar -xzf update/doc.tar.gz || endScriptError
echo "XXXXXXXXXXXXXXXXXXX"
tar -xzkf update/user.tar.gz
echo "Error between here and XXXXXXXXXXXXXXXXXXX are normal !!"
# || endScriptError

rm -rf update

exit 0
