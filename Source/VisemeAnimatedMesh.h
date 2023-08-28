#pragma once

#include <map>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "PhonemeIdentification.h"

struct Vector2D
{
	float X{ 0.0f }, Y{ 0.0f };
};

struct Viseme
{
	std::vector<Vector2D> Vertices;
};

class VisemeAnimatedMesh
{
public:
	VisemeAnimatedMesh(const PhonemeIdentification* phonemeIdentifier);
	void Initialise(const std::string& filePath);
	bool IsInitialised() const;
private:

	void InitialiseVisemes(const nlohmann::json& visemesNode);
	void InitialisePhonemeAssociations(const nlohmann::json& associationsNode);
	void InitialiseAnimationSettings(const nlohmann::json& settingsNode);
	
	void AddViseme(const std::string& symbol, const std::string& filePath);

	const std::string& VisemeRootDirectory() const;
	const std::map<std::string, Viseme>& Visemes() const;
	const std::map<std::string, std::string>& PhonemeToVisemeAssociations() const;
	float StateTransitionTime() const;


	std::string m_VisemeRootDirectory;
	std::map<std::string, Viseme> m_Visemes;
	std::map<std::string, std::string> m_PhonemeToVisemeAssociations;
	Viseme m_StateToDisplay;
	float m_StateTransitionTime { 0.25f };
	const PhonemeIdentification* m_PhonemeIdentifier { nullptr };
};
