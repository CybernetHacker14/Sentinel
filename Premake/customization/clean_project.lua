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
		os.remove("**.vcxproj")
		os.remove("**.vcxproj.filters")
		os.remove("**.vcxproj.user")
		print("Done")
	end
}