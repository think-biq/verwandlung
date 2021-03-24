/*
    Utility to extract blendshape / morph target information from FBX files.

    2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
    See license.md for details.

    https://think-biq.com
*/

#include <iostream>
#include <ostream>
#include <map>
#include <string>
#include <vector>
#include <locale>
#include <algorithm>
#include <span>

#include <SmallFBX.h>
#include <wandel.hpp>

#include <macos/attr.h>

ATTRIBUTE_HIDDEN
void Investigate(const sfbx::BlendShapeChannel* Channel) {
	const auto& Data = Channel->getShapeData();
	std::cerr << "Found channel: " << Channel->getName() << " containing " << Data.size() << " entries." << std::endl;
}

ATTRIBUTE_HIDDEN
void Investigate(const sfbx::span<sfbx::Deformer*>& Deformers) {
	std::cerr << "Detected " << Deformers.size() << " deformers." << std::endl;
	for (size_t DeformerIndex = 0; DeformerIndex < Deformers.size(); ++DeformerIndex) {
		sfbx::Deformer* Deformer = Deformers[DeformerIndex];
		std::cerr << "Deformer: " << Deformer << " seems legit ..." << std::endl;
		const sfbx::Deformer& Keks = *Deformer;
		std::cerr << "???: " << GetObjectSubClassName(Keks.getSubClass()) << std::endl;
		if (sfbx::ObjectSubClass::BlendShape == Keks.getSubClass()) {
			std::cerr << "Class check ok ..." << std::endl;
			std::cerr << "BlendShape: " << Deformer->getName() << std::endl;
			if (auto* BlendshapeDeformer = static_cast<sfbx::BlendShape*>(Deformer))
			{
				const auto& Channels = BlendshapeDeformer->getChannels();
				for (size_t ChannelIndex = 0; ChannelIndex < Channels.size(); ++ChannelIndex) {
					Investigate(Channels[ChannelIndex]);
				}
			}
			else {
				std::cerr << "Failed to fetch deformer as blendshapes :/" << std::endl;
			}
		}
	}
}

ATTRIBUTE_HIDDEN
void LookForBlendshape(const sfbx::Geometry* Geometry) {
	if (nullptr == Geometry) {
		return;
	}

	std::cerr << "Found geometry. Checking blendshapes ... (" << Geometry << ")" << std::endl;
	bool bHasDeformers = Geometry->hasDeformer();
	std::cerr << (bHasDeformers ? "Deformers are present, checking ..." : "No deformers present, skipping ...") << std::endl;
	if (false == bHasDeformers) {
		return;
	}

	const auto& Deformers = Geometry->getDeformers();
	Investigate(Deformers);
}

ATTRIBUTE_HIDDEN
void LookForMeshesAndBlendshapes(const sfbx::Object* Node, size_t Level = 0) {
	std::cout << "Checking node: " << Node->getName() << " with types: "
					<< sfbx::GetObjectClassName(Node->getClass())
					<< " / " << sfbx::GetObjectSubClassName(Node->getSubClass())
					<< std::endl;

	if (sfbx::ObjectSubClass::BlendShapeChannel == Node->getSubClass()) {
		if (const auto* Shape = static_cast<const sfbx::Shape*>(Node)) {
			Investigate(static_cast<const sfbx::BlendShapeChannel*>(Node));
		}
	}

	const auto& Children = Node->getChildren();
	for (size_t ChildIndex = 0; ChildIndex < Children.size(); ++ChildIndex) {
		const auto* Child = Children[ChildIndex];
		LookForMeshesAndBlendshapes(Child, Level + 1);
	}
}

ATTRIBUTE_HIDDEN
bool ExportCurvesFrom(const sfbx::AnimationLayer* Layer, sfbx::ObjectSubClass ClassToConsider) {
	bool bHasBlendshapes = false;
	const auto& CurveNodes = Layer->getAnimationCurveNodes();
	std::cout << "{ \"count\": " << CurveNodes.size() << ", \"shapes\": [";
	for (size_t CurveNodeIndex = 0; CurveNodeIndex < CurveNodes.size(); ++CurveNodeIndex) {
		const auto& CurveNode = CurveNodes[CurveNodeIndex]; // BlendShapeChannel

		const auto& CurveName = CurveNode->getName();
		const auto& Curves = CurveNode->getAnimationCurves();
		if (ClassToConsider == CurveNode->getAnimationTarget()->getSubClass()) {
			if (bHasBlendshapes) {
				std::cout << ", ";
			}

			bHasBlendshapes = true;

			std::cout << "{" << "\"name\": \"" << CurveName << "\", \"target\": \"" << CurveNode->getAnimationTarget()->getChild()->getName() << "\", \"curves\": [";
			for (size_t CurveIndex = 0; CurveIndex < Curves.size(); ++CurveIndex) {
				const auto& Curve = Curves[CurveIndex];
				std::cout << "{" << "\"name\": \"" << Curve->getName() << "\"";
				std::cout << "," << "\"link\": \"" << Curve->getLinkName() << "\"";
				std::cout << ", \"start_time\": " << Curve->getStartTime();
				std::cout << ", \"end_time\": " << Curve->getStopTime();
				{
					std::cout << ", \"times\": [";
					const auto& Values = Curve->getTimes();
					for (size_t ValueIndex = 0; ValueIndex < Values.size(); ++ValueIndex) {
						std::cout << Values[ValueIndex];
						if ((ValueIndex + 1) < Values.size()) {
							std::cout << ", ";
						}
					}
					std::cout << "]";
				}
				{
					std::cout << ", \"values\": [";
					const auto& Values = Curve->getRawValues();
					for (size_t ValueIndex = 0; ValueIndex < Values.size(); ++ValueIndex) {
						std::cout << Values[ValueIndex] * Curve->getUnitConversion();
						if ((ValueIndex + 1) < Values.size()) {
							std::cout << ", ";
						}
					}
					std::cout << "]";
				}
				std::cout << "}";
				if ((CurveIndex + 1) < Curves.size()) {
					std::cout << ", ";
				}
			}
			std::cout << "]}";
		}
	}
	std::cout << "]}";

	return bHasBlendshapes;
}

ATTRIBUTE_HIDDEN
void ShowInfo(const sfbx::Object* Object, std::ostream& o, bool bRecurse = false, size_t Level = 0, bool bExcludeSelf = false, bool bAppendLinebreak = true) {
	bool bValidObject = nullptr != Object;

	if (false == bExcludeSelf) {
		for (size_t LevelIndex = 0; LevelIndex < Level; ++LevelIndex)
			o << "\t";

		if (false == bValidObject) {
			o << "Null Object!";
		}
		else {
			o << "Object[" << Object->getID() << "]: " << Object->getName()
				<< " (" << sfbx::GetObjectClassName(Object->getClass())
				<< "/" << sfbx::GetObjectSubClassName(Object->getSubClass())
				<< ")"
				;
		}

		if (bAppendLinebreak)
			o << std::endl;
	}

	if (bValidObject && bRecurse) {
		const auto& Children = Object->getChildren();
		for (size_t ChildIndex = 0; ChildIndex < Children.size(); ++ChildIndex) {
			ShowInfo(Children[ChildIndex], o, true, Level + 1);
		}
	}
}

ATTRIBUTE_HIDDEN
bool ListBlendshapesFrom(const sfbx::AnimationLayer* Layer, sfbx::ObjectSubClass ClassToConsider, size_t Verbosity = 0) {
	bool bHasBlendshapes = false;
	std::ostream& o = std::cout;
	const auto& CurveNodes = Layer->getAnimationCurveNodes();
	for (size_t CurveNodeIndex = 0; CurveNodeIndex < CurveNodes.size(); ++CurveNodeIndex) {
		const auto& CurveNode = CurveNodes[CurveNodeIndex];
		const auto& CurveName = CurveNode->getName();
		const auto& Curves = CurveNode->getAnimationCurves();
		if (ClassToConsider == CurveNode->getAnimationTarget()->getSubClass()) {
			
			bHasBlendshapes = true;

			if (0 < Verbosity) {
				o << "MATCH: ";
			}
			o << CurveNode->getAnimationTarget()->getChild()->getName() << std::endl;
			if (0 < Verbosity) {
				ShowInfo(CurveNode, o, false, 0, false, false); o << ": " << std::endl;
				o << "Target info: "; ShowInfo(CurveNode->getAnimationTarget(), o, false, 0, false, true);
				o << "\tTarget Parent: "; ShowInfo(CurveNode->getAnimationTarget()->getParent(), o, false, 0, false, true);
				o << "\tTarget Children: " << std::endl;
				ShowInfo(CurveNode->getAnimationTarget(), o, 1 < Verbosity, 2, false, true);
			}
		}
	}

	return bHasBlendshapes;
}

ATTRIBUTE_HIDDEN
inline bool CompareCharactersCaseInsesitive(char a, char b) {
    return std::tolower(a) == std::tolower(b);
}

ATTRIBUTE_HIDDEN
inline bool CompareCharacters(char a, char b) {
    return a == b;
}

namespace biq {

bool Compare(const std::string_view& A, const std::string_view& B, bool bUseCase) {
	if (false == (A.size() == B.size())) {
		return false;
	}

   	return std::equal(A.begin(), A.end(), B.begin()
        , bUseCase ? CompareCharacters : CompareCharactersCaseInsesitive
        );
}

std::string ToLower(const std::string_view& Input) {
	auto& f = std::use_facet<std::ctype<char>>(std::locale());
	std::string Result(Input);
	f.tolower(&Result[0], &Result[0] + Result.size());

	return Result;
}

namespace Verwandlung {

bool Wandel(const std::string& Filepath, WandelParams Params) {
	std::cerr << "Opening file at: " << Filepath << std::endl;

	sfbx::DocumentPtr doc = sfbx::MakeDocument(Filepath);

	const sfbx::span<sfbx::AnimationStack*>& as = doc->getAnimationStacks();
	if (0 == as.size()) {
		std::cerr << "No animation stack present!" << std::endl;
		return false;
	}

	std::cerr << "Animation stacks: " << as.size() << std::endl;

	const sfbx::AnimationStack* PrimeStack = as[0];
	const sfbx::span<sfbx::AnimationLayer*>& Layers = PrimeStack->getAnimationLayers();
	if (0 == Layers.size()) {
		std::cerr << "No animation layer present!" << std::endl;
		return false;
	}

	std::cerr << "Animation layers: " << Layers.size() << std::endl;

	sfbx::ObjectSubClass MatchType = sfbx::ObjectSubClass::BlendShapeChannel;
	if (0 < Params.Arguments.size()) {
		if (biq::Compare(Params.Arguments[0], "morph")) {
			MatchType = sfbx::ObjectSubClass::BlendShapeChannel;
		}
		else if (biq::Compare(Params.Arguments[0], "bone")) {
			MatchType = sfbx::ObjectSubClass::LimbNode;
		}
		else if (biq::Compare(Params.Arguments[0], "mesh")) {
			MatchType = sfbx::ObjectSubClass::Mesh;
		}
		else {
			std::cerr << "Unknown match type "
				<< sfbx::GetObjectSubClassName(MatchType)
				<< " setting to morps ..." << std::endl;
		}
	}

	bool bSuccess = true;
	const sfbx::AnimationLayer* PrimeLayer = Layers[0];
	switch(Params.Mode) {
		case WandelMode::List:
			bSuccess = ListBlendshapesFrom(PrimeLayer, MatchType, 0);
		break;
		case WandelMode::Export:
			bSuccess = ExportCurvesFrom(PrimeLayer, MatchType);
		break;
		default:
			std::cerr << "Unknown mode :/" << std::endl;
			bSuccess = false;
	}

	return bSuccess;
}

}}