from setuptools import setup

setup(name='hm',
      version='0.1',
      description='Command line history manager for bash',
      url='http://github.com/dkolmakov/hm',
      author='Dmitrii Kolmakov',
      author_email='dimannadivane@gmail.com',
      license='Apache 2.0',
      packages=['hm_initializer'],
      entry_points = {
        'console_scripts': ['hm-init=hm_initializer:main'],
      },
      zip_safe=False)


