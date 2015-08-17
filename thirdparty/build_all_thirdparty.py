#!/usr/bin/python

import argparse
import os
import multiprocessing
import os,shutil,subprocess,sys
import glob,time,threading

def full_generator_name(short):
	if short == "msvc":
		return "NMake Makefiles JOM"
	elif short == "gcc":
		if os.name == "nt":
			return "MinGW Makefiles"
		else:
			return "Unix Makefiles"
	return short

def build_tool(short):
	if short == "NMake Makefiles JOM":
		return "jom"
	elif short == "MinGW Makefiles":
		return "mingw32-make"
	else:
		return "make"

class cmake_project:
	def __init__(self,name,args,cmake_file_dir=None):
		self.args = args
		self.name = name
		self.src_dir = os.path.join(args["src_root"],name)
		self.build_dir = os.path.join(args["build_root"],name)
		self.install_dir = os.path.join(args["install_root"],name)
		
		self.cmake_file_dir = self.src_dir
		if cmake_file_dir != None:
			self.cmake_file_dir = os.path.join(self.src_dir,cmake_file_dir)

		self.cmake_flags = []
		self.cmake_flags.append("-DCMAKE_INSTALL_PREFIX:PATH="+self.install_dir)
	
	def build(self):
		cmake_flags = self.cmake_flags + ["-DCMAKE_BUILD_TYPE:STRING="+self.args["build_type"],"-G"+self.args["generator"],self.cmake_file_dir]
		
		if not os.path.isdir(self.build_dir):
			os.makedirs(self.build_dir)

		#run cmake
		cmake_process = subprocess.Popen(["cmake"]+cmake_flags, cwd=self.build_dir, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		self.cmake_messages = cmake_process.communicate()
		if cmake_process.returncode == 0:
			#build
			build_process = subprocess.Popen([self.args["build_tool"],"-j"+str(self.args["number_of_cores"])],cwd=self.build_dir, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
			self.build_messages = build_process.communicate()
			if build_process.returncode == 0:
				#install
				install_process = subprocess.Popen([self.args["build_tool"],"install"], cwd=self.build_dir, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
				self.install_messages = install_process.communicate()
				if install_process.returncode != 0:
					print(self.name + " install failed")
					print(self.install_messages[0])
					print(self.install_messages[1])
					return False
			else:
				print(self.name + " build failed")
				print(self.build_messages[0])
				print(self.build_messages[1])
				return False
		else:
			print(self.name + " cmake failed")
			print(self.cmake_messages[0])
			print(self.cmake_messages[1])
			return False
		return True

def main():
	build_types = ["Release","Debug"]
	toolchains = ["gcc"]
	if os.name == "nt":
		toolchains = ["msvc"] + toolchains

	parser = argparse.ArgumentParser(description='Build thirdpary libraries')
	parser.add_argument("--toolchain","-tc",metavar="'toolchain'",choices=toolchains,type=str,default=toolchains[0],help="toolchain used to compile the libraries.")
	parser.add_argument("--src_root",'-sr',metavar="'directory'",type=str,help="the directory where the libraries source code is.",default=".src")
	parser.add_argument("--build_root","-br",metavar="'directory'",type=str,help="the directory to build the libraries in.",default=".build")
	parser.add_argument("--install_root","-ir",metavar="'directory'",type=str,help="the directory to install the libraries in.",default="")
	parser.add_argument("--build_type","-bt",metavar=str(tuple(build_types)),type=str,choices=build_types,help="the build type to compile the libraries with.",default=build_types[0])
	parser.add_argument("--number_of_cores","-j",type=int,help="the number of cores to compile the libraries with.",default=multiprocessing.cpu_count())

	args = vars(parser.parse_args())
	args["generator"] = full_generator_name(args["toolchain"])
	args["build_tool"] = build_tool(args["generator"])
	args["src_root"] = os.path.abspath(args["src_root"])
	args["build_root"] = os.path.abspath(args["build_root"])
	args["install_root"] = os.path.abspath(args["install_root"])

	zlib = cmake_project("zlib",args)

	#physfs
	physfs = cmake_project("physfs",args)

	#freetype
	freetype = cmake_project("freetype",args)

	#libpng
	libpng = cmake_project("libpng",args)
	libpng.cmake_flags.append("-DZLIB_ROOT:PATH="+zlib.install_dir)

	#theora
	theora = cmake_project("theora",args)

	#vorbis
	vorbis = cmake_project("vorbis",args)

	#jsoncpp
	jsoncpp = cmake_project("jsoncpp",args)

	#googlemock
	googlemock = cmake_project("googlemock",args)

	#openal
	openal_soft = cmake_project("openal-soft",args)

	#SDL
	SDL = cmake_project("SDL",args)
	SDL.cmake_flags.append("-DSDL_DLOPEN:BOOL=TRUE")
	SDL.cmake_flags.append("-DSDL_SHARED:BOOL=TRUE")
	SDL.cmake_flags.append("-DSDL_STATIC:BOOL=FALSE")

	#bullet3
	bullet3 = cmake_project("bullet3",args)
	bullet3.cmake_flags.append("-DINSTALL_LIBS:BOOL=TRUE")
	bullet3.cmake_flags.append("-DBUILD_BULLET2_DEMOS:BOOL=FALSE")
	bullet3.cmake_flags.append("-DBUILD_CPU_DEMOS:BOOL=FALSE")
	bullet3.cmake_flags.append("-DBUILD_OPENGL3_DEMOS:BOOL=FALSE")
	bullet3.cmake_flags.append("-DBUILD_UNIT_TESTS:BOOL=FALSE")

	#angelscript
	angelscript = cmake_project("angelscript",args,"sdk/angelscript/projects/cmake")

	#recastnavigation
	recastnavigation = cmake_project("recastnavigation",args)

	#assimp
	assimp = cmake_project("assimp",args)
	assimp.cmake_flags.append("-DZLIB_ROOT:PATH="+zlib.install_dir)
	assimp.cmake_flags.append("-DASSIMP_BUILD_ASSIMP_TOOLS:BOOL=FALSE")
	assimp.cmake_flags.append("-DASSIMP_BUILD_TESTS:BOOL=FALSE")
	assimp.cmake_flags.append("-DASSIMP_ENABLE_BOOST_WORKAROUND:BOOL=FALSE")
	assimp.cmake_flags.append("-DASSIMP_INSTALL_PDB:BOOL=FALSE")

	#mojoshader
	mojoshader = cmake_project("mojoshader",args)

	#librocket
	os.environ["FREETYPE_DIR"] = freetype.install_dir
	librocket = cmake_project("libRocket",args,"libRocket/Build")
	assimp.cmake_flags.append("-DFREETYPE_DIR:PATH="+freetype.install_dir)

	#glloadgen
	glloadgen = cmake_project("glloadgen",args)

	projects = [
			zlib,
		#	physfs,
		##	freetype,
		##	libpng,
		#	theora,
		#	vorbis,
			jsoncpp,
		#	googlemock,
		##	openal_soft,
			SDL,
		##	bullet3,
		#	angelscript,
		#	recastnavigation,
			assimp
		#	mojoshader,
		#	librocket
		]

	for project in projects:
		project.build()
if __name__ == '__main__':
	main()
