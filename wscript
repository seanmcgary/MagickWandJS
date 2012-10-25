#!/usr/bin/env python
import subprocess
import re

def set_options(ctx):
	ctx.tool_options('compiler_cxx')

def configure(ctx):
	ctx.check_tool('compiler_cxx')
	ctx.check_tool('node_addon')
	

	sep = re.compile('\s+')
	#magickLibs = sep.split(subprocess.Popen("pkg-config --cflags --libs MagickWand", stdout=subprocess.PIPE, shell=True).communicate()[0].split("\n")[0].rstrip(" "))
	#print(magickLibs)
	#ctx.env.append_unique('CPPFLAGS', [magickLibs])
	
	ctx.env.append_unique('CPPFLAGS', ["-fopenmp", "-I/usr/local/include/ImageMagick", "-L/usr/local/lib", "-lMagickWand", "-lMagickCore"])

def build(ctx):
	t = ctx.new_task_gen('cxx', 'shlib', 'node_addon')

	t.source = ['./lib/MagickWandJSNative/magickWandJs.cpp']

	t.target = 'magickWandJs'
