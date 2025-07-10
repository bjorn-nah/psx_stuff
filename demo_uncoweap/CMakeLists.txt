# PSn00bSDK example CMake script
# (C) 2021 spicyjpeg - MPL licensed

cmake_minimum_required(VERSION 3.21)

project(
	Shadow25
	LANGUAGES    C CXX ASM
	VERSION      1.0.0
	DESCRIPTION  "Shadow Party 2025"
	HOMEPAGE_URL ""
)

file(GLOB _scenes scene*.c)
psn00bsdk_add_executable(shadow25 GPREL disp.c 3dStuff.c modplayer.c ${_scenes} main.c)

psn00bsdk_target_incbin(shadow25 PRIVATE _binary_dark_saucisse_start dark_saucisse.hit)

psn00bsdk_add_cd_image(
	iso      # Target name
	shadow25 # Output file name (= template.bin + template.cue)
	iso.xml  # Path to config file
	DEPENDS shadow25 system.cnf
)
