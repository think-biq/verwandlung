/*
    Utility to extract blendshape / morph target information from FBX files.

    2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
    See license.md for details.

    https://think-biq.com
*/

#include <iostream>
#include <string>

#include <wandel.hpp>

int main (int ArgC, char** ArgV) {
	//std::locale::global(std::locale("en_US.utf8"));

	std::string Filepath;
	biq::Verwandlung::WandelParams Params;

	int LastArgIndex = ArgC - 1;
	if (1 < ArgC) {
		if (2 < ArgC) {
			std::string option(ArgV[1]);
			if (biq::Compare(option, "--list")) {
				Params.Mode = biq::Verwandlung::WandelMode::List;
			}
			else if (biq::Compare(option, "--export")) {
				Params.Mode = biq::Verwandlung::WandelMode::Export;
			}
			else {
				Params.Mode = biq::Verwandlung::WandelMode::Unknown;
			}

			if (2 < LastArgIndex) {
				// Fetch which type of export: morph, bone, mesh
				Params.Arguments.push_back(biq::ToLower(std::string(ArgV[2])));
			}
		}

		Filepath = std::string(ArgV[LastArgIndex]);
	}

	if (Filepath.empty()) {
		std::cerr << "No file input!" << std::endl;
		return 0;
	}

	bool bSuccess = biq::Verwandlung::Wandel(Filepath, Params);

	return bSuccess ? 0 : 3;
}