-- Clean action trigger for cleaning generated project and build files
newaction {
	trigger = "clean",
	description = "Cleans generated project and build files",
	execute = function()
		print("Cleaning files...")
		os.rmdir("./bin")
		os.rmdir("./bin-int")
		os.rmdir("./.vs")
		
		os.remove("*.sln")

		os.remove("Sentinel/*.vcxproj")
		os.remove("Sentinel/*.vcxproj.filters")
		os.remove("Sentinel/*.vcxproj.user")

		os.remove("Sentinel/Vendor/cglm/*.vcxproj")
		os.remove("Sentinel/Vendor/cglm/*.vcxproj.filters")
		os.remove("Sentinel/Vendor/cglm/*.vcxproj.user")

		os.remove("Sentinel/Vendor/glfw/*.vcxproj")
		os.remove("Sentinel/Vendor/glfw/*.vcxproj.filters")
		os.remove("Sentinel/Vendor/glfw/*.vcxproj.user")

		os.remove("Premake/*.vcxproj")
		os.remove("Premake/*.vcxproj.filters")
		os.remove("Premake/*.vcxproj.user")

		os.remove("Sandbox/*.vcxproj")
		os.remove("Sandbox/*.vcxproj.filters")
		os.remove("Sandbox/*.vcxproj.user")

		print("Done")
	end
}