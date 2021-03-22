/*
    Utility to extract blendshape / morph target information from FBX files.

    2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
    See license.md for details.

    https://think-biq.com
*/

#include <string>
#include <string_view>

namespace biq {

bool Compare(const std::string_view& A, const std::string_view& B, bool bUseCase = true);

std::string ToLower(const std::string_view& Input);

namespace Verwandlung {

enum WandelMode {
	Unknown,
	List,
	Export
};

bool Wandel(const std::string& Filepath, WandelMode Mode);

}

}