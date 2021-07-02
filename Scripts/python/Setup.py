import os
import platform
import json
import subprocess

from enum import Enum

from SetupPython import PythonConfiguration as PythonRequirements
from SetupPremake import PremakeConfiguration as PremakeRequirements
from SetupCMake import CMakeConfiguration as CMakeRequirements

buildSystemList = [
	'Premake',
	'CMake'
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
	stateDataPresent = IsStateDataPresent()
	dictDataPresent = False
	usedBuildSystem = None

	if stateDataPresent:
		for entry in buildSystemList:
			if buildSystemDict[entry]:
				dictDataPresent = True
				usedBuildSystem = entry
				break

	if not stateDataPresent or (stateDataPresent and not dictDataPresent):
		i = 1
		for entry in buildSystemList:
			print(f"{i} - Generate Project files with {entry}")
			i += 1
		print("")
		reply = int(str(input("Enter an option: ")).lower().strip()[:1])

		if 1 <= reply <= len(buildSystemList):
			print(f"\n{buildSystemList[reply-1]} selected.\n")
			ExecuteBuildSystemOperation(buildSystemList[reply-1], Action.GENERATE)
			SaveStateDataAsJson(buildSystemList[reply-1])
		else:
			print("\nInvalid option selected. Aborting...")
			return
	else:
		ConstructRegenOrCleanUI(usedBuildSystem)


def ConstructRegenOrCleanUI(buildsystem):
	print(f"1 - Regenerate Project files with {buildsystem}")
	print(f"2 - Clean {buildsystem} generated project files")
	print("")
	reply = str(input("Enter an option: ")).lower().strip()[:1]

	if not reply == '1' and not reply == '2':
		print("\n Invalid option selected. Aborting...")
		return

	if reply == '1':
		print(f"\nRegenerating with {buildsystem}...")
		ExecuteBuildSystemOperation(buildsystem, Action.GENERATE)
		SaveStateDataAsJson(buildsystem)
	elif reply == '2':
		print(f"\nCleaning project files...\n")
		ExecuteBuildSystemOperation(buildsystem, Action.CLEAN)
		SaveStateDataAsJson(buildsystem, generated=False)


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

	if (buildsystem == 'Premake' and not premakeInstalled) or (buildsystem == 'CMake' and not cmakeInstalled):
		print(f"{buildsystem} installation not found\n\n")
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
