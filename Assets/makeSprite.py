import sys
import os
import subprocess
import shutil

spriteName = sys.argv[ 1 ]
specificDirectory = "sizeBaseSloped"
spriteBaseDirectory = "." + os.sep + spriteName + os.sep
spriteSpecificDirectory = spriteBaseDirectory + specificDirectory
subprocess.call( "isoVoxel " + spriteName + ".vox 64 64 64 1 outline=none " )
subprocess.call( "SpriteSheetPacker " + spriteSpecificDirectory + " " + spriteBaseDirectory + " -f json" )
toDelete = []
for i in next( os.walk( spriteBaseDirectory ) )[ 1 ]: 
    shutil.rmtree( spriteBaseDirectory + i )
