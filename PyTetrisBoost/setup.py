from distutils.core import setup, Extension

import os

dirlist = [x for x in os.listdir() if x[-4:] == ".cpp"]


module1 = Extension('PyTetrisBoost',
    include_dirs = ['E:\\Programs\\boost_1_72_0\\boost_1_72_0',
                    'C:\\Dev\\SDL2\\include'],
    library_dirs = ['E:\\Programs\\boost_1_72_0\\boost_1_72_0\\stage\\lib',
                    'C\\Dev\\SDL2\\lib\\x64'],
    sources = dirlist)

setup(
    name = 'PyTetrisBoost',
    version = '1.0',
    description = 'PyTetris made with boost',
    ext_modules = [module1],
    data_files=[('', ['SDL2.dll'])]
    )
