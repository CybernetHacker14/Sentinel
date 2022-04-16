@echo off

mode con: cols=100 lines=30

echo * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
echo *              Welcome to Sentinel Game Engine Setup                  *
echo *                                                                     *
echo * This utility automatically generates project files and sets up the  *
echo * necessary environment parameters, so that the user can build the    *
echo * engine from source and be able to use and develop at his perusal    *
echo * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
echo:
echo * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
echo * For the duration of this setup, it is expected of the user to, at   *
echo * certain points of the setup, give the required input so that the    *
echo * utility can properly download, install and generate the necessary   *
echo * files, as per the user's choice. This input can vary as either      *
echo * confirmation choices, or as configuration choices.                  *
echo * Proper indication will be given to the user regarding the same.     *
echo * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
echo:
PAUSE
cls

python python/Setup.py

PAUSE
