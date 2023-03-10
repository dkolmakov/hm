from os import path, pardir

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as build_ext_orig

# Get the long description
cwd = path.abspath(path.dirname(__file__))
with open(path.join(cwd, 'docs', 'python_package.md'), encoding='utf-8') as f:
    long_description = f.read()

HM_DB_VERSION = "1.1.0"


class build_ext(build_ext_orig):
    def write_version_file(self):
        with open('src/version.hpp.in', 'r') as f:
            fdata = f.read()

        fdata = fdata.replace('@hm_VERSION@', HM_DB_VERSION)

        with open('src/version.hpp', 'w') as f:
            f.write(fdata)

    def build_extension(self, ext):
        sources = ext.sources
        # sort to make the resulting .so file build reproducible
        sources = sorted(sources)

        ext_path = self.get_ext_fullpath(ext.name)
        par_ext_dir = path.abspath(path.join(ext_path, pardir))

        self.write_version_file()

        extra_args = ["-std=c++11", "-fPIC", "-fpic", "-O3",
                      "-DSQLITE_OMIT_LOAD_EXTENSION", "-DSQLITE_MAX_WORKER_THREADS=0", "-DSQLITE_ENABLE_FTS5"]

        objects = self.compiler.compile(sources,
                                        output_dir=self.build_temp,
                                        include_dirs=ext.include_dirs,
                                        extra_postargs=extra_args,
                                        depends=ext.depends)

        self.compiler.link_executable(
            objects, path.join(par_ext_dir, ext.name),
            extra_postargs=extra_args,
            target_lang="c++")


setup(name='history-manager',
      version=HM_DB_VERSION,
      description='Command line history manager for bash',
      long_description=long_description,
      long_description_content_type='text/markdown',
      url='http://github.com/dkolmakov/hm',
      author='Dmitrii Kolmakov',
      author_email='dimannadivane@gmail.com',
      license='Apache 2.0',
      packages=['hm_initializer'],
      ext_modules=[
          Extension(
              name='hm-db',
              sources=[
                  "src/main.cpp",
                  "thirdparty/sqlite3/sqlite3.c"
              ],
              include_dirs=[
                  "src",
                  "thirdparty/sqlite3",
                  "thirdparty/apathy",
                  "thirdparty/clipp"
              ]
          )
      ],
      cmdclass={'build_ext': build_ext},
      entry_points={
          'console_scripts': ['hm-init=hm_initializer:main'],
      },
      zip_safe=False)


