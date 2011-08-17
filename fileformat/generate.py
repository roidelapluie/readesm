#!/usr/bin/python
# -*- coding: utf-8 -*-

## Generate the classes needed to read tachograph data
## Copyright(C) Andreas Gölzer

# remove auto-gen'd files with
# find . \( ! -regex '.*/\..*' ! -name "generate.py" \) | xargs grep -snH "AUTO-GENERATED" --color | cut -f1 -d':' | xargs rm



from xml.etree.ElementTree import ElementTree

tree = ElementTree()
tree.parse('DataDefinitions.xml')

def writeIfDifferent(filename, newContent):
	try:
		f = open(filename,'r')
		content = f.read()
		f.close()
	except IOError:
		content =''
		pass
	if content != newContent:
		f = open(filename,'w')
		f.write(newContent)
		f.close()

def lcfirst(a):
	return a[0].lower() + a[1:]

def writeCodeFile(name, directory, headerContent, codeContent, headerDependencies, codeDependencies):
	autoGenWarning = '\n//This file was AUTO-GENERATED. Make changes in the generator script(' + __file__ + ')' + \
		'\n//the derived classes or in the data definition\n\n';
	headerFile =   '#ifndef '  + name.upper() + '_H\n' + \
		  '#define '  + name.upper() + '_H\n' + \
		  '\n' + autoGenWarning  + \
		  '\n\n' + '\n'.join(['#include ' + f for f in headerDependencies])  + '\n\n' + \
		  headerContent + \
		  '\n#endif\n'
	codeFile = 	codeFile = '#include "' + name + '.h"\n' + \
		  '\n' + autoGenWarning  + \
		  '\n\n' + '\n'.join(['#include ' + f for f in codeDependencies])  + \
		  '\n\n' + codeContent + '\n'

	filename =  directory + '/' + name + '.h'
	writeIfDifferent(filename, headerFile)

	filename =  directory + '/' + name + '.cpp'
	writeIfDifferent(filename, codeFile)

sizes = {
	'TimeReal' : 4,
	'Timespan' : 8,
	'ActivityChangeInfo' : 2
	}

for j in range(3):
	haveSizes = True
	for block in tree.findall('DataType'):
		name=block.get('name')
		if name in sizes:
			continue
		try:
			print name
			size = 0
			for elem in block.find('content'):
				l = elem.get('length')
				if l:
					size += int(l)
				elif elem.tag in sizes:
					size += sizes[elem.tag]
				else:
					raise StopIteration()
			sizes[block.get('name')] = size
		except StopIteration:
			haveSizes = False
			continue
print sizes
if not haveSizes:
	print "Could not determine block sizes for " + ', '.join(block.get('name') if block.get('name') not in sizes else "" for block in tree.findall('DataType')) + ", abandoning"
	exit()

hasToString = set(['TimeReal', 'Timespan', 'RawData', 'LargeNumber'])
for block in tree.findall('DataType'):
	if block.find('toString') is not None:
		hasToString.add(block.get('name'))
print hasToString


for block in tree.findall('CardBlock') + tree.findall('DataType') + tree.findall('VuBlock'):
	initList = block.tag + '(filewalker)'
	output = ''
	elements = ''
	headerDependencies = set(['"' + block.tag + '.h"'])
	codeDependencies = set()
	name = block.get('name')
	print name
	if block.get('hasrefined'):
		name = 'Raw' + name
	title = block.find('title')
	if title is not None:
		title = title.text
	else:
		title = name
	
	offsets = {'CardBlock' : 5, 'VuBlock' : 2, 'DataType' : 0}
	offset = offsets[block.tag]
	offsetextra = ''
	for elem in block.find('content'):
		ename = elem.get('name')
		type = elem.tag
		isLast = False
		fullOffset = 'start + %(offset)i %(offsetextra)s' % vars()
		
		length = elem.get('length')
		if length is not None:
			length = int(length)
		elif type in sizes:
			length = sizes[type]
		elif block.tag == 'CardBlock':
			length = 'size() - (%(offset)i %(offsetextra)s)' % vars()
			isLast = True

		if type == 'string':
			type = 'QString'
			builder = 'fixedString(%s, %i)' % (fullOffset, length)
		elif type == 'cpstring':
			type = 'QString'
			builder = 'codepageStringCombination(%s, %i)' % (fullOffset, length)
		elif type == 'int':
			builder = 'readBigEndianInt%(length)i(%(fullOffset)s)' % vars()
		elif type == 'RawData' or type == 'LargeNumber':
			builder = '%s, %s'  % (fullOffset, str(length))
			headerDependencies.add('"../DataTypes/' + type + '.h"')
		elif type == 'Subblocks':
			subtype = elem.get('type')
			length = elem.get('counterlength')
			subsize = sizes[subtype]
			if not ename:
				ename = lcfirst(subtype + 's')
			if length:
				length=int(length)
				subcount = 'readBigEndianInt%i(%s)' % (length, fullOffset)
				builder = '%(fullOffset)s + %(length)i, %(subcount)s' % vars()
			elif block.tag == 'CardBlock':
				subcount = '(dataSize() + 5 - (%(offset)i %(offsetextra)s)) / %(subsize)i' % vars()
				builder = '%(fullOffset)s, %(subcount)s' % vars()
				isLast = True
			else:
				raise
			offsetextra += ' + ' + ename + '.dataSize()'
			type = 'Subblocks<' + subtype + '>'
			headerDependencies.add('"../DataTypes/Subblocks.h"')
			headerDependencies.add('"../DataTypes/%(subtype)s.h"' % vars())
		else:
			builder = fullOffset
			headerDependencies.add('"../DataTypes/' + type + '.h"')

		table = elem.get('table')
		if table is not None:
			codeDependencies.add('"../formatStrings.h"')
			output += '\no(tr("' + ename + '"), formatStrings::' + table + '(' + ename + '));'
		elif type in hasToString:
			output += '\no(tr("' + ename + '"), ' + ename + '.toString());'
		elif type == 'int':
			unit = elem.get('unit')
			if unit:
				output += '\no(tr("%(ename)s"), QString("%%1 %(unit)s").arg(%(ename)s));' % vars()
			else:
				output += '\no(tr("%(ename)s"), %(ename)s);' % vars()
		elif type == 'QString':
			output += '\no(tr("%(ename)s"), %(ename)s);' % vars()
		else:	
			print ename, type
			output += '\n%(ename)s.printOn(o);' % vars()
		elements += '\n' + type + ' ' + ename + ';'
		initList += ',\n\t' + ename + '(' + builder + ')'
		if not isLast:
			offset += length

	headerContent = 'class ' + name + ' : public ' + block.tag + ' {\n' + \
		  '\t' + 'Q_DECLARE_TR_FUNCTIONS(' + name + ')\npublic:\n' + \
		  '\t' + elements.replace('\n','\n\t') + '\n\n' + \
		  '\t' + name + '(constDataPointer filewalker);\n'
	codeContent = name + '::' + name + '(constDataPointer filewalker) : ' + initList + '\n{}\n\n'

	if block.tag != 'DataType':
		headerContent += '\t' + 'static const int Type = ' + block.get('type') + ';\n' + \
		  '\t' + 'QString name() const;\n'
		codeContent +=  'QString ' + name + '::name() const {\n\treturn tr("' + title + '");\n}\n\n'
	if block.tag != 'CardBlock':
		headerContent += '\tint size() const;\n'
		if block.tag == 'DataType':
			headerContent += '\tstatic const int staticSize = %(offset)i;\n' % vars()
		hasSignature = block.get('requiresSignature')
		if hasSignature:
			offset += 128
		codeContent += 'int %(name)s::size() const{\n\treturn %(offset)i %(offsetextra)s;\n}\n\n' % vars()
	toString = block.find('toString')
	if toString is not None:
		headerContent += '\tQString toString() const;\n'
		toString = toString.text
		codeContent += 'QString %(name)s::toString() const{\n\treturn %(toString)s;\n}\n\n' % vars()
		
	headerContent += '\t' + 'virtual void printOn(reporter& o) const;\n' + \
		  '};\n\n'
	codeContent += 'void ' + name + '::printOn(reporter& o) const {' + output.replace('\n','\n\t') + '\n}\n'

	writeCodeFile(name, block.tag + 's', headerContent, codeContent, headerDependencies, codeDependencies)


#headerDependencies = set(('"CardBlock.h"','<vector>'))
#codeDependencies = set()
#className = 'CardBlockContainer'
#blocklist =  tree.findall('CardBlock')
#headerDependencies |= set('"%s.h"' % block.get('name') for block in blocklist  )
#headerContent = '\n\n' + 'class ' + className + ' {\npublic:' + '\n\t'.join('std::vector<' + block.get('name') + '> ' + lcfirst(block.get('name')) + 's;' for block in blocklist)

#headerContent += '\n\tint blockCount() const;'
#codeContent = '\nint ' + className + '::blockCount() const {\n\treturn ' + ' + '.join(lcfirst(block.get('name')) + '.size()' for block in blocklist) +  ';\n}\n'

#headerContent += '\n\t///add a block to the container, return its size\n\tint addBlock(int type, const constDataPointer& start) const;'
#codeContent += '\nint ' + className + '::addBlock(int type, const constDataPointer& start) const {' + \
	#'\n\tswitch(' + type + '){' + '\n\t\t'.join('case ' + block.get('name') + '::Type: ' + lcfirst(name) + 's.push_back(T(start));\n\t\t\treturn '+ lcfirst(name) + 's.last().size();' for block in blocklist)

className='CardBlock'
blocklist =  tree.findall(className)

headerDependencies = set(('"CardBlock.h"','<QtCore/QSharedPointer>'))
codeDependencies = set('"%s.h"' % block.get('name') for block in blocklist  )

headerContent = 'QSharedPointer<CardBlock> cardBlockFactory(const constDataPointer& start);'
codeContent = headerContent[:-1] + '{\n' + \
	'\tif(start.bytesLeft() < 5) return QSharedPointer<'+className+'>();\n' + \
	'\tswitch(readBigEndianInt2(start)) {\n\t\t' + \
	'\n\t\t'.join('case ' + block.get('name') + '::Type: return QSharedPointer<'+block.get('name') +'>(new ' + block.get('name') + '(start));' for block in blocklist) + '\n' + \
	'\t\tdefault: return QSharedPointer<CardBlock>(new CardBlock(start));\n\t}\n}\n'
writeCodeFile('cardBlockFactory', 'CardBlocks', headerContent, codeContent, headerDependencies, codeDependencies)


className='VuBlock'
blocklist =  tree.findall(className)

headerDependencies = set(('"VuBlock.h"','"VuUnknownBlock.h"','<QtCore/QSharedPointer>'))
codeDependencies = set('"%s.h"' % block.get('name') for block in blocklist  )

headerContent = 'QSharedPointer<VuBlock> vuBlockFactory(const constDataPointer& start);'
codeContent = headerContent[:-1] + '{\n' + \
	'\tif(start.bytesLeft() < 2 || readBigEndianInt1(start) != 0x76) return QSharedPointer<'+className+'>();\n' + \
	'\tswitch(readBigEndianInt1(start + 1)) {\n\t\t' + \
	'\n\t\t'.join('case ' + block.get('name') + '::Type: return QSharedPointer<'+block.get('name') +'>(new ' + block.get('name') + '(start));' for block in blocklist) + '\n' + \
	'\t\tdefault: return QSharedPointer<VuBlock>(new VuUnknownBlock(start));\n\t}\n}\n'
writeCodeFile('vuBlockFactory', 'VuBlocks', headerContent, codeContent, headerDependencies, codeDependencies)

	#name = block.get('name')
	#headerDependencies.add('"' + name + '.h"')
	#elements += '\nstd::vector<' + name + '> '  + lcfirst(name) + 's;'
	#builders += '\ncase ' + name + '::Type: ' + lcfirst(name) + 's.push_back(T(filewalker)); break;'
	

