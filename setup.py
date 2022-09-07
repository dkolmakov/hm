from setuptools import setup
from os import path, makedirs, chdir, pardir

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as build_ext_orig

# Get the long description
cwd = path.abspath(path.dirname(__file__))
with open(path.join(cwd, 'docs', 'python_package.md'), encoding='utf-8') as f:
    long_description = f.read()


class CMakeExtension(Extension):
    def __init__(self, name):
        super().__init__(name, sources=[])


class build_ext(build_ext_orig):

    def run(self):
        for ext in self.extensions:
            self.build_cmake(ext)
        super().run()

    def build_cmake(self, ext):
        makedirs(self.build_temp, exist_ok=True)
        ext_dir = self.get_ext_fullpath(ext.name)
        par_ext_dir = path.abspath(path.join(ext_dir, pardir))
        makedirs(ext_dir, exist_ok=True)
        
        cmake_args = [
            '-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=' + par_ext_dir,
        ]

        build_args = [
            '--', '-j4'
        ]

        chdir(self.build_temp)
        self.spawn(['cmake', cwd] + cmake_args)
        if not self.dry_run:
            self.spawn(['cmake', '--build', '.'] + build_args)
            
        chdir(cwd)


setup(name='history-manager',
      version='0.1.6',
      description='Command line history manager for bash',
      long_description=long_description,
      long_description_content_type='text/markdown',
      url='http://github.com/dkolmakov/hm',
      author='Dmitrii Kolmakov',
      author_email='dimannadivane@gmail.com',
      license='Apache 2.0',
      packages=['hm_initializer'],
      ext_modules=[CMakeExtension('hm-db')],
      cmdclass={
        'build_ext': build_ext,
      },
      entry_points = {
        'console_scripts': ['history-manager-init=hm_initializer:main'],
      },
      zip_safe=False)


