from setuptools import setup

setup(
   name='sonic-platform',
   version='1.0',
   description='SONiC platform API implementation',
   license='Apache 2.0',
   packages=[
       'sonic_platform',
   ],
   package_dir={'sonic_platform': 'sonic_platform'},
   classifiers=[
        'Development Status :: 3 - Alpha',
        'Environment :: Plugins',
        'Intended Audience :: Developers',
        'Intended Audience :: Information Technology',
        'Intended Audience :: System Administrators',
        'License :: OSI Approved :: Apache Software License',
        'Natural Language :: English',
        'Operating System :: OpenBMC linux',
        'Programming Language :: Python :: 3.9',
        'Topic :: Utilities',
   ],
   keywords='sonic SONiC platform PLATFORM'
)
