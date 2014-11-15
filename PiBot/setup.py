from distutils.core import setup, Extension

import os

if not os.path.exists('settings.json'):
    import settings
    settings.setup()

# define the extension module
pennapprobot = Extension('pennapprobot', sources=['pennapprobot.c'], libraries=['bcm2835'], library_dirs=['/usr/local/lib'])
pennappcam = Extension('pennappcam', sources=['pennappcam.c'], libraries=['bcm2835'], library_dirs=['/usr/local/lib'])



# run the setup
setup(ext_modules=[pennapprobot])
setup(ext_modules=[pennappcam])

