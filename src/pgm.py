# coding=utf-8 
import Image
import os.path
import glob

# python function
# Function: Convert a jpg file to pgm format file
# Parameter: jpg_file: the name of the jpg file to be converted
# pgm_dir: The directory where pgm files are stored
def jpg2pgm( jpg_file, pgm_dir ):
    # First open the jpg file
    jpg = Image.open( jpg_file)
    # resize to 200 * 250, bilinear interpolation
    jpg = jpg.resize( (200,250), Image.BILINEAR)
    # Call the python function os.path.join, os.path.splitext, os.path.basename to generate the target pgm file name
    name =(str)(os.path.join( pgm_dir, os.path.splitext( os.path.basename(jpg_file) )[0] ))+".pgm"
    # Create target pgm file
    jpg.save( name)

# Put all jpg files in the current working directory, or cd {directory where jpg files are stored}
for jpg_file in glob.glob("./*.jpg"):
    jpg2pgm( jpg_file, "./")

if __name__ == "__main__":
    jpg2pgm("./Nike-By-You.jpg", "./")