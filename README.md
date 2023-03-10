# ROOT dictionary inconsistencies - reproducer

This is a reproducer showcasing some - presumably - inconsistent library/dictionary loading by [ROOT](https://github.com/root-project/root).

The reproducer was tested with ROOT `v6-26-10-alice5` at https://github.com/alisw/root.git.

Tested on
* CentOS7,
* macOS Monterey.

## Build and install

```bash
mkdir $BUILDIR && cd $BUILDIR
# in case you are using a toolchain and if that is not properly exported in alienv do
# this is important for the dictionary generation
export GCC_TOOLCHAIN_ROOT=<path_to_toolchain_dir>
# the ussual CMake
cmake $SOURCEDIR/CMakeLists.txt -DROOT_DIR=$ROOTSYS -DCMAKE_INSTALL_PREFIX=$INSTALLDIR
make && make install
```

## Run the code

```bash
export LD_LIBRARY_PATH=$INSTALLDIR/lib:$LD_LIBRARY_PATH

$INSTALLDIR/bin/executable # This should abort, telling you to provide 2 arguments (path to macro and function in macro)
```

### Expected behaviour
```bash
$INSTALLDIR/bin/executable $SOURCEDIR/macros/macro1.C "myFunc()"
<pointer to a base::below::Base object>
Base
mInt=0
Derived
mInt=1
```

### Observed behaviour
```bash
$INSTALLDIR/bin/executable $SOURCEDIR/macros/macro1.C "myFunc()"

$SOURCEDIR/macros/macro1.C:14:29: error: no type named 'func' in namespace 'base::othernamespace'
      base::othernamespace::func(mInt);
```

At the same time, one can do
```bash
# enter ROOT shell
root[0] .L $SOURCEDIR/macros/macro1.C
```
or
```bash
# enter ROOT shell
root[0] gROOT->LoadMacro($SOURCEDIR/macros/macro1.C)
```
which works just find.

So in the first case there is a problem while actually, somehow ROOT is able to load the macro.
Indeed, looking into the [source code](sub_dir/src/MacroHelper.cxx), I am using (as in O2) in essence `TROOT::LoadMacro` and `TROOT::ProcessLine`.

### The inconsitency

Open `$SOURCEDIR/macros/macro1.C` and uncomment the line `base::othernamespace::A any;`.
Now, running the executable again leads to the expected behaviour.

That points to the fact that something is inconsistent with the library loading on the ROOT side.


## Overall code structure

There are 2 libraries.
`libNamespaceStuff` contains some utility code while `libMacroHelper` provides the capability of dynamically parsing a a macro and calling a specific function in it.
They 2 libraries have nothing to do with each other (e.g. none is linked against the other).

`libNamespaceStuff` defines a class `base::othernamespace::A` as well as functions `base::othernamespace::func` and just `hello`.
In addition, ROOT dictionary code is compiled into a `rootmap` and `pcm` file which are installed along with the libraries.

The `executable` uses `libMacroHelper`'s `GetFromMacro` to load a function from a macro and it is used to extract the object that is returned by the macro function.
At the moment only macro, called `macro1.C`, exists which has only one function called `myFunc()`.
The path to the macro and the function to call are passed as command-line arguments to `executable`.

The macro function `myFunc` in turn uses code compiled into the library `libNamespaceStuff`. 
