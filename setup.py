from setuptools import setup
from os import path

# Get the long description
cwd = path.abspath(path.dirname(__file__))
with open(path.join(cwd, 'docs', 'python_package.md'), encoding='utf-8') as f:
    long_description = f.read()

setup(name='history-manager',
      version='0.1.2',
      description='Command line history manager for bash',
      long_description=long_description,
      long_description_content_type='text/markdown',
      url='http://github.com/dkolmakov/hm',
      author='Dmitrii Kolmakov',
      author_email='dimannadivane@gmail.com',
      license='Apache 2.0',
      packages=['hm_initializer'],
      entry_points = {
        'console_scripts': ['hm-init=hm_initializer:main'],
      },
      zip_safe=False)


