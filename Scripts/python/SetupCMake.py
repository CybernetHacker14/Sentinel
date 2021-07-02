import subprocess
import importlib.util as importlib_util

class CMakeConfiguration:
	@classmethod
	def Validate(cls):
		for packageName in ["cmake"]:
			if not cls.__ValidateCMake(packageName):
				print("CMake is not installed")
				return False

			print("CMake Python package detected")
			return True

	@classmethod
	def __ValidateCMake(cls, packageName):
		if importlib_util.find_spec(packageName) is None:
			return cls.__InstallCMake(packageName)
		return True

	@classmethod
	def __InstallCMake(cls, packageName):
		permissionGranted = False
		while not permissionGranted:
			reply = str(
				input("Would you like to install CMake package '{0:s}'? [Y/N]: ".format(packageName))).lower().strip()[
					:1]
			if reply == 'n':
				return False
			permissionGranted = (reply == 'y')

		print(f"Installing {packageName} module...")
		subprocess.check_call(['python', '-m', 'pip', 'install', packageName])

		return cls.__ValidateCMake(packageName)

	if __name__ == "__main__":
		CMakeConfiguration.Validate()
