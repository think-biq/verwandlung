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
	biq::Verwandlung::WandelMode Mode;

	if (1 < ArgC) {
		if (2 < ArgC) {
			std::string option(ArgV[1]);
			if (biq::Compare(option, "--list")) {
				Mode = biq::Verwandlung::WandelMode::List;
			}
			else if (biq::Compare(option, "--export")) {
				Mode = biq::Verwandlung::WandelMode::Export;
			}
			else {
				Mode = biq::Verwandlung::WandelMode::Unknown;
			}
		}

		Filepath = std::string(ArgV[ArgC - 1]);
	}

	if (Filepath.empty()) {
		std::cerr << "No file input!" << std::endl;
		return 0;
	}

	bool bSuccess = biq::Verwandlung::Wandel(Filepath, Mode);

	return bSuccess ? 0 : 3;
}