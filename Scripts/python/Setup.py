import os
import platform
import json
import subprocess

from enum import Enum

from SetupPython import PythonConfiguration as PythonRequirements
from SetupPremake import PremakeConfiguration as PremakeRequirements
from SetupCMake import CMakeConfiguration as CMakeRequirements

buildSystemList = [
	'premake',
	'cmake'
]


class Action(Enum):
	GENERATE = 0
	CLEAN = 1


stateDataDirectoryPath = './Scripts/__state__'
stateDataFilePath = f"{stateDataDirectoryPath}/state.json"

jsonFormatRoot = 'build-systems'
jsonBuildSystemAttribute = 'build-system'
jsonGeneratedAttribute = 'generated'

buildSystemDict = dict.fromkeys(buildSystemList, False)


def main():
	# Make sure everything we need for setup has been installed
	PythonRequirements.Validate()

	os.chdir('./../')  # Change from Scripts directory to root

	if IsStateDataPresent():
		print(f"State data found at {os.path.abspath(stateDataFilePath)}\n")
		LoadStateDataFromJson()
	else:
		print(f"State Data not found. Running first time setup...\n")

	ConstructOptionUI()


def ConstructOptionUI():
	if not IsStateDataPresent() or (IsStateDataPresent() and not buildSystemDict['premake'] and not buildSystemDict['cmake']):
		print("1 - Generate Project files with Premake")
		print("2 - Generate Project files with CMake")
		print("")
		reply = str(input("Enter an option: ")).lower().strip()[:1]

		if not reply == '1' and not reply == '2':
			print("\nInvalid option selected. Aborting...")
			return

		if reply == '1':
			print("\nPremake selected.\n")
			ExecuteBuildSystemOperation('premake', Action.GENERATE)
			SaveStateDataAsJson('premake')
		elif reply == '2':
			print("\nCMake selected.\n")
			ExecuteBuildSystemOperation('cmake', Action.GENERATE)
			SaveStateDataAsJson('cmake')

	else:
		if buildSystemDict['premake']:
			print("1 - Regenerate Project files with Premake")
			print("2- Clean Premake generated project files")
			print("")
			reply = str(input("Enter an option: ")).lower().strip()[:1]

			if not reply == '1' and not reply == '2':
				print("\n Invalid option selected. Aborting...")
				return

			if reply == '1':
				print("\nRegenerating with Premake...\n")
				ExecuteBuildSystemOperation('premake', Action.GENERATE)
				SaveStateDataAsJson('premake')
			elif reply == '2':
				print("\nCleaning project files...\n")
				ExecuteBuildSystemOperation('premake', Action.CLEAN)
				SaveStateDataAsJson('premake', generated=False)

		elif buildSystemDict['cmake']:
			print("1 - Regenerate Project files with CMake")
			print("2 - Clean CMake generated project files")
			print("")
			reply = str(input("Enter an option: ")).lower().strip()[:1]

			if not reply == '1' and not reply == '2':
				print("\n Invalid option selected. Aborting...")
				return

			if reply == '1':
				print("\nRegenerating with CMake...\n")
				ExecuteBuildSystemOperation('cmake', Action.GENERATE)
				SaveStateDataAsJson('cmake')
			elif reply == '2':
				print("\nCleaning project files...\n")
				ExecuteBuildSystemOperation('cmake', Action.CLEAN)
				SaveStateDataAsJson('cmake', generated=False)


def IsStateDataPresent():
	return IsFilePresent(stateDataFilePath)


def IsFilePresent(filePath):
	return os.path.exists(os.path.abspath(filePath))


def SaveStateDataAsJson(selectedBuildSystem, generated=True):
	buildSystemDict[selectedBuildSystem] = generated

	data = {jsonFormatRoot: []}
	for entry in buildSystemDict:
		data[jsonFormatRoot].append({
			jsonBuildSystemAttribute: entry,
			jsonGeneratedAttribute: buildSystemDict[entry]
		})

	filepath = os.path.abspath(stateDataFilePath)
	os.makedirs(os.path.dirname(filepath), exist_ok=True)

	with open(filepath, 'w') as outfile:
		outfile.write(json.dumps(data, indent=2))


def LoadStateDataFromJson():
	filepath = os.path.abspath(stateDataFilePath)

	with open(filepath) as json_file:
		data = json.load(json_file)

		for entry in data[jsonFormatRoot]:
			buildSystemDict[entry[jsonBuildSystemAttribute]] = entry[jsonGeneratedAttribute]


def ExecuteBuildSystemOperation(buildsystem, operation):
	if not isinstance(operation, Action):
		raise TypeError("Parameter 'operation' must be of type Action")

	premakeInstalled = PremakeRequirements.Validate()
	cmakeInstalled = CMakeRequirements.Validate()

	if buildsystem == 'premake' and not premakeInstalled:
		print("Premake installation not found\n\n")
		return False
	elif buildsystem == 'cmake' and not cmakeInstalled:
		print("CMake installation not found\n\n")
		return False

	if operation == Action.GENERATE:
		filepath = f"./Scripts/bat/{platform.system()}-{buildsystem}-GenerateProjectFiles.bat"
	elif operation == Action.CLEAN:
		filepath = f"./Scripts/bat/{platform.system()}-{buildsystem}-RemoveProjectFiles.bat"
	else:
		print("Unknown build system operation encountered. Aborting...\n\n")
		return False

	print("Running build system...\n")
	if not IsFilePresent(filepath):
		print(filepath)
		print("Appropriate script file not found. Check your Scripts directory")
		return False
	else:
		subprocess.call([os.path.abspath(filepath), "nopause"])
		return True


if __name__ == "__main__":
	main()
