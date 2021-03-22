/*
    Utility to extract blendshape / morph target information from FBX files.

    2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
    See license.md for details.

    https://think-biq.com
*/

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <locale>
#include <algorithm>
#include <span>

#include <SmallFBX.h>
#include <wandel.hpp>

void Investigate(const sfbx::BlendShapeChannel* Channel) {
	const auto& Data = Channel->getShapeData();
	std::cerr << "Found channel: " << Channel->getName() << " containing " << Data.size() << " entries." << std::endl;
}

void Investigate(const sfbx::span<sfbx::Deformer*>& Deformers) {
	std::cerr << "Detected " << Deformers.size() << " deformers." << std::endl;
	for (size_t DeformerIndex = 0; DeformerIndex < Deformers.size(); ++DeformerIndex) {
		sfbx::Deformer* Deformer = Deformers[DeformerIndex];
		assert(Deformer);
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
		assert(Child);
		LookForMeshesAndBlendshapes(Child, Level + 1);
	}
}

bool ExportBlendshapesFrom(const sfbx::AnimationLayer* Layer) {
	bool bHasBlendshapes = false;
	const auto& CurveNodes = Layer->getAnimationCurveNodes();
	std::cout << "{ \"count\": " << CurveNodes.size() << ", \"shapes\": [";
	for (size_t CurveNodeIndex = 0; CurveNodeIndex < CurveNodes.size(); ++CurveNodeIndex) {
		const auto& CurveNode = CurveNodes[CurveNodeIndex];
		const auto& CurveName = CurveNode->getName();
		const auto& Curves = CurveNode->getAnimationCurves();
		if (sfbx::ObjectSubClass::BlendShapeChannel == CurveNode->getAnimationTarget()->getSubClass()) {
			bHasBlendshapes = true;
			std::cout << "{" << "\"name\": \"" << CurveName << "\", \"target\": \"" << CurveNode->getAnimationTarget()->getChild()->getName() << "\", \"curves\": [";
			for (size_t CurveIndex = 0; CurveIndex < Curves.size(); ++CurveIndex) {
				const auto& Curve = Curves[CurveIndex];
				std::cout << "{" << "\"name\": \"" << Curve->getName() << "\"";
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
			if ((CurveNodeIndex + 1) < CurveNodes.size()) {
				std::cout << ", ";
			}
		}
	}
	std::cout << "]}";

	return bHasBlendshapes;
}

bool ListBlendshapesFrom(const sfbx::AnimationLayer* Layer) {
	bool bHasBlendshapes = false;
	const auto& CurveNodes = Layer->getAnimationCurveNodes();
	for (size_t CurveNodeIndex = 0; CurveNodeIndex < CurveNodes.size(); ++CurveNodeIndex) {
		const auto& CurveNode = CurveNodes[CurveNodeIndex];
		const auto& CurveName = CurveNode->getName();
		const auto& Curves = CurveNode->getAnimationCurves();
		if (sfbx::ObjectSubClass::BlendShapeChannel == CurveNode->getAnimationTarget()->getSubClass()) {
			std::cout << CurveNode->getAnimationTarget()->getChild()->getName() << std::endl;
			bHasBlendshapes = true;
		}
	}

	return bHasBlendshapes;
}


inline bool CompareCharactersCaseInsesitive(char a, char b) {
    return std::tolower(a) == std::tolower(b);
}
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

bool Wandel(const std::string& Filepath, WandelMode Mode) {
	sfbx::DocumentPtr doc = sfbx::MakeDocument(Filepath);

	const auto& as = doc->getAnimationStacks();
	if (0 == as.size()) {
		std::cerr << "No animation stack present!" << std::endl;
		return false;
	}

	const auto& stack = as[0];
	const auto& Layers = stack->getAnimationLayers();
	if (0 == Layers.size()) {
		std::cerr << "No animation layer present!" << std::endl;
		return false;
	}

	bool bSuccess = true;
	const auto& Layer = Layers[0];
	switch(Mode) {
		case WandelMode::List:
			bSuccess = ListBlendshapesFrom(Layer);
		break;
		case WandelMode::Export:
			bSuccess = ExportBlendshapesFrom(Layer);
		break;
		default:
			std::cerr << "Unknown mode :/" << std::endl;
			bSuccess = false;
	}

	return bSuccess;
}

}}