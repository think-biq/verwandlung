/*
    Utility to extract blendshape / morph target information from FBX files.

    2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
    See license.md for details.

    https://think-biq.com
*/

#include <string>
#include <string_view>
#include <vector>

namespace biq {

bool Compare(const std::string_view& A, const std::string_view& B, bool bUseCase = true);

std::string ToLower(const std::string_view& Input);

namespace Verwandlung {

enum WandelMode {
	Unknown,
	List,
	Export
};

struct WandelParams {
	WandelMode Mode;
	std::vector<std::string> Arguments;

	WandelParams()
		: Mode(WandelMode::Unknown)
		, Arguments()
	{}
};

bool Wandel(const std::string& Filepath, WandelParams Params);

}

}