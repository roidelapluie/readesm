#!/usr/bin/python
# -*- coding: utf-8 -*-

from distutils.core import setup
setup(
	name='loadvu',
	version='2011.11',
	py_modules=['vuserial','timestampsutc','loadvugui'],
	scripts=['loadvu'],
	author='Andreas Gölzer',
	author_email='andreas@goelzer.de',
	data_files=[],
	url='http://readesm.sf.net/',
	long_description='New trucks in the European Union are required to have a digital tachograph unit on board.  This script downloads data from vehicle units attached to the computer with a serial cable.',
	license='GPL 3+',
	requires=['PyQt4','serial'],
	classifiers=['Programming Language :: Python']
)